#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <stdlib.h>
#include "datastructures.h"
#include "instance.h"

using namespace std;

//Instance constructor
Instance::Instance(Database *_database)
{
    database = _database;
};

//What is sown in the login page
char Instance::loginPage()
{
    char loginType;
    cout << "========================================" << endl;
    cout << "Welcome to Hochan's SNS service!" << endl;
    cout << "Please login first!" << endl;
    cout << "Login   : \'l\'" << endl;
    cout << "Register: \'r\'" << endl;
    cout << "Admin   : \'a\'" << endl;
    cout << "Quit    : \'q\'" << endl;
    cin >> loginType;
    return loginType;
};

//login status
int Instance::login()
{
    string id, password;
    int isLoggedin = 0;
    char retry = 'y';

    while (isLoggedin < 1) //ask the user to login
    {
        cout << "========================================" << endl;
        cout << "Enter ID: " << endl;
        cin >> id;
        cout << "Enter password: " << endl;
        cin >> password;
        int verificationStatus = database->verify(id, password); //verify from the database about the inputs
        if (verificationStatus == 1)
        {
            isLoggedin = 1;
            cout << "Login Successfull!" << endl;
        }
        else
        {
            int wrongInput = 1;
            if (verificationStatus == 0)
            {
                cout << "========================================" << endl;
                cout << "Wrong password!" << endl;
                while (wrongInput)
                {
                    cout << "Do you want to retry? y/n" << endl;
                    cin >> retry;
                    if (retry == 'y')
                    {
                        wrongInput = 0;
                    }
                    else if (retry == 'n')
                    {
                        wrongInput = 0;
                        isLoggedin = 2;
                    }
                    else
                    {
                        cout << "Please enter y or n" << endl;
                    };
                };
            }
            else if (verificationStatus == 2)
            {
                //change to prompt user to sign up
                cout << "========================================" << endl;
                cout << "Wrong password!" << endl;
                while (wrongInput)
                {
                    cout << "Do you want to retry? y/n" << endl;
                    cin >> retry;
                    if (retry == 'y')
                    {
                        wrongInput = 0;
                    }
                    else if (retry == 'n')
                    {
                        wrongInput = 0;
                        isLoggedin = 2;
                    }
                    else
                    {
                        cout << "Please enter y or n" << endl;
                    };
                };
            }
        }
    }
    return isLoggedin;
};
void Instance::signUp() //create new account
{
    string id, password, verification;
    cout << "Enter your id" << endl;
    cin >> id;
    //check if id exists
    if (database->userMap.find(id) != database->userMap.end()) //there is already a user with this ID
    {
        cout << "The user already exists!" << endl;
    }
    else
    {
        cout << "Enter your password" << endl;
        cin >> password;
        cout << "Verify your password" << endl;
        cin >> verification;
        if (password != verification) //if the password does not match
        {
            cout << "password does not match!" << endl;
        }
        else
        {
            User *newUser = new User(id, password); //dynamically create a new user and store it in the user map
            database->userMap[id] = newUser;
        }
    }
};
void Instance::adminLogin() //login for admins
{
    string id, password;
    cout << "Enter admin id" << endl;
    cin >> id;
    cout << "Enter admin password" << endl;
    cin >> password;
    int verify = database->admin->verify(id, password);
    if (verify == 0)
    {
        cout << "ID does not match!" << endl;
    }
    else if (verify == 1)
    {
        cout << "Password does not match!" << endl;
    }
    else
    {
        cout << "Logged in as admin!" << endl;
    }
};
int Instance::getNewPostID() //get the biggest post id in the system and add one
{
    int last = database->postList.size() - 1;
    return database->postList[last]->getPostID() + 1;
};
void Instance::userShowFeed() //show user's feed
{
    int feedCount = 0;
    cout << "------------------" << endl;
    cout << "My Feed" << endl;
    for (int i = database->postList.size() - 1; i >= 0; i--) //for each post in the post list
    {
        string author = database->postList[i]->getAuthor();                                            //the author of this post
        if ((database->loggedinUser->isFriend(author)) || (author == database->loggedinUser->getID())) //if the author is the friend or the loggedin user
        {
            database->postList[i]->print();
            feedCount++;
        }
    }
    if (feedCount == 0)
    {
        cout << "There are no posts!" << endl;
    }
};
void Instance::userShowFriendList() //show friends of the user
{
    cout << "------------------" << endl;
    cout << "My Friends" << endl;
    cout << "------------------" << endl;
    database->loggedinUser->showFriends();
};
void Instance::userRequestFriend() //request friends
{
    string friendToRequest;
    cout << "------------------" << endl;
    cout << "Enter the ID of the user to request friend" << endl;
    cin >> friendToRequest;
    //check if already friend
    if (database->userMap.find(friendToRequest) == database->userMap.end())
    {
        cout << "The user does not exist!" << endl;
    }
    else if (database->loggedinUser->isFriend(friendToRequest))
    {
        cout << "Already friends!" << endl;
    }
    else
    {
        database->userMap[friendToRequest]->requestFriend(database->loggedinUser->getID()); //request friend to the other user
        cout << "Friend requested!" << endl;
    }
};
void Instance::userAcceptFriend() //accept friend requests
{
    string acceptedFriendID, friendToAccept;
    cout << "------------------" << endl;
    cout << "My Friend Requests" << endl;
    database->loggedinUser->showRequestedFriends();
    //enter the friend to accept
    cout << "------------------" << endl;
    cout << "Enter the number(not ID) to accept friend request." << endl;
    cin >> friendToAccept;
    //delete from requestedfriend list
    acceptedFriendID = database->loggedinUser->getFriendID(friendToAccept);
    database->loggedinUser->eraseFriendRequest(friendToAccept);
    //insert at the friendlist
    database->loggedinUser->addFriend(acceptedFriendID);
    database->userMap[acceptedFriendID]->addFriend(database->loggedinUser->getID());
    cout << "Accepted friend request!" << endl;
};
void Instance::userUnfriend() //unfriend
{
    string deletedFriendID, friendToDelete;
    userShowFriendList();
    //ask for the friend to delete
    cout << "------------------" << endl;
    cout << "Enter the number(not ID) to unfriend." << endl;
    cin >> friendToDelete;
    deletedFriendID = database->loggedinUser->getFriendID(friendToDelete);
    //delete from friendlist(other side as well)
    database->loggedinUser->eraseFriend(friendToDelete);
    database->userMap[deletedFriendID]->eraseFriend(friendToDelete);
    cout << "Unfriended " << deletedFriendID << " successfully." << endl;
};
int Instance::userShowMyPosts() //show posts that I uploaded
{
    int myPostCount = 0;
    cout << "------------------" << endl;
    cout << "My Posts" << endl;
    for (int i = database->postList.size() - 1; i >= 0; i--)
    {
        string author = database->postList[i]->getAuthor();
        if (author == database->loggedinUser->getID()) //check if I am the author
        {
            database->postList[i]->print();
            myPostCount++;
        }
    }
    if (myPostCount == 0)
    {
        cout << "There are no posts posted by me!" << endl;
    }
    return myPostCount;
};
void Instance::userPost() //create new post
{
    Post *newPost;
    string newPostText;
    cout << "------------------" << endl;
    cout << "Enter a sentence you want to post: " << endl;
    cin.ignore();
    getline(cin, newPostText);

    newPost = new Post(getNewPostID(), database->loggedinUser->getID(), newPostText); //dynamically create a new post and save
    database->addNewPost(newPost);
    cout << "Post added successfully!" << endl;
};
void Instance::userDeletePost() //delete my post
{
    string postIDToDelete;
    int myPostCount = userShowMyPosts();
    if (myPostCount > 0)
    {
        cout << "------------------" << endl;
        cout << "Enter the post id you want to delete." << endl;
        cin >> postIDToDelete;
        database->deletePost(postIDToDelete);
    };
};
void Instance::printUserPromptMessage() // the message shown to the user
{
    cout << " ----------------------------------------------------------------------------------" << endl;
    cout << "                             User Prompt                                           " << endl;
    cout << " ----------------------------------------------------------------------------------" << endl;
    cout << "| 1. Show Feed"
         << "       |  2. Show My Posts"
         << "     |  3. Show My Friends                 |" << endl;
    cout << "| 4. Add New Post"
         << "    |  5. Request Friend"
         << "    |  6. Accept Friend. # of requests: " << database->loggedinUser->numberOfFriendRequests() << " |" << endl;
    cout << "| 7. UnFriend"
         << "        |  8. Delete a post"
         << "     |  9. Logout                          |" << endl;
    cout << " ----------------------------------------------------------------------------------" << endl;
};
void Instance::userPrompt() // the logic to let user to choose actions
{
    bool appRunning = true;
    while (appRunning)
    {
        char input;
        int feedCount = 0;
        int myPostCount = 0;

        printUserPromptMessage();
        cin >> input;
        switch (input)
        {
        case '1':
            //showFeed
            userShowFeed();
            break;
        case '2':
            //show my posts
            //for each post, check if the author is me
            userShowMyPosts();
            break;
        case '3':
            //show friendlist
            userShowFriendList();
            break;
        case '4':
            //post
            userPost();
            break;
        case '5':
            //friend request
            userRequestFriend();
            break;
        case '6':
            //accept friend
            userAcceptFriend();
            break;
        case '7':
            //Unfriend
            userUnfriend();
            break;
        case '8':
            //delete post
            userDeletePost();
            break;
        //ask for the post to delete
        //delete the post
        case '9':
            //logout
            //clear loggedinUser
            database->loggedinUser = NULL;
            appRunning = false;
        default:;
        }
    }
};
void Instance::adminShowPosts() //show all posts
{
    int postCount = 0;
    cout << "------------------" << endl;
    cout << "Posts" << endl;
    for (int i = database->postList.size() - 1; i >= 0; i--)
    {
        database->postList[i]->print();
        postCount++;
    }

    if (postCount == 0)
    {
        cout << "There are no posts!" << endl;
    }
};
void Instance::adminShowUsers() //show all users
{
    map<string, User *>::iterator it;
    cout << "------------------" << endl;
    cout << "Users" << endl;

    for (it = database->userMap.begin(); it != database->userMap.end(); it++)
    {
        it->second->print();
    }
};
void Instance::adminDeletePost() //delete any post the admin wants
{
    string postIDtoDelete;
    int postCount = 0;
    cout << "------------------" << endl;
    cout << "Posts" << endl;
    for (int i = database->postList.size() - 1; i >= 0; i--)
    {
        database->postList[i]->print();
        postCount++;
    }

    if (postCount == 0)
    {
        cout << "There are no posts!" << endl;
    }
    else
    {
        cout << "Enter the post id to delete." << endl;
        cin >> postIDtoDelete;
        database->deletePost(postIDtoDelete);
    }
};
void Instance::adminDeleteUser() //delete any user the admin wants
{
    string userToDelete;
    map<string, User *>::iterator it;
    cout << "------------------" << endl;
    cout << "Users" << endl;

    for (it = database->userMap.begin(); it != database->userMap.end(); it++)
    {
        it->second->print();
    }

    cout << "Enter the user id to delete." << endl;
    cin >> userToDelete;
    database->deleteUser(userToDelete);
};
void Instance::printAdminPromptMessage() //the message shown for the admin to choose an action
{
    cout << " ----------------------------------------------------------------------------------" << endl;
    cout << "                             Admin Prompt                                           " << endl;
    cout << " ----------------------------------------------------------------------------------" << endl;
    cout << "| 1. Show Posts"
         << "       |  2. Show Users"
         << "     |  3. Delete Post                      |" << endl;
    cout << "| 4. Delete User"
         << "      |  5. Logout                                                |" << endl;
    cout << " ----------------------------------------------------------------------------------" << endl;
};
void Instance::adminPrompt() //the logic for admin to choose an action
{
    bool appRunning = true;
    while (appRunning)
    {
        char input;
        printAdminPromptMessage();
        cin >> input;
        switch (input)
        {
        case '1':
            //show Posts
            adminShowPosts();
            break;
        case '2':
            //show Users
            adminShowUsers();
            break;
        case '3':
            //Delete Post
            adminDeletePost();
            break;
        case '4':
            //Delete User
            adminDeleteUser();
            break;
        case '5':
            //logout
            appRunning = false;
        default:;
        }
    }
};

void Instance::startApp() //start the app
{
    char loginType;
    bool appRunning = true;
    while (appRunning) //while loop until the user wants to quit
    {
        loginType = loginPage();
        if (loginType == 'l')
        {
            int loginStatus = login();
            if (loginStatus == 1)
            {
                userPrompt();
            }
            else
            {
                appRunning = false;
            }
        }
        else if (loginType == 'r')
        {
            signUp();
        }
        else if (loginType == 'a')
        {
            adminLogin();
            adminPrompt();
        }
        else if (loginType == 'q')
        {
            cout << "Quitting..." << endl;
            appRunning = false;
        }
    }
};
void Instance::save(const string profile_file_name, const string post_file_name, const string friendship_file_name) //save the database into csv files
{
    database->save(profile_file_name, post_file_name, friendship_file_name);
}
Instance::~Instance() //destructor
{
    cout << "Deleted Instance" << endl;
};