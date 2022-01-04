#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <stdlib.h>
using namespace std;

const string profile_file_name = "profile.csv";
const string post_file_name = "post.csv";
const string friendship_file_name = "friendship.csv";

class Profile
{
protected:
    string id;
    string password;
};

class User : private Profile
{
private:
    vector<string> friendList;
    vector<string> friendRequest;
    friend class Instance;
    friend class Database;

public:
    User(string _id, string _password)
    {
        id = _id;
        password = _password;
    };
    string getID()
    {
        return id;
    }
    bool isFriend(string userID)
    {
        for (int i = 0; i < friendList.size(); i++)
        {
            if (friendList[i] == userID)
            {
                return true;
            }
        }
        return false;
    }
    void showFriends()
    {
        if (friendList.size() == 0)
        {
            cout << "You have no friends!" << endl;
        }
        else
        {
            for (int i = 0; i < friendList.size(); i++)
            {
                cout << i + 1 << ". " << friendList[i] << endl;
            }
        };
    };
    void showRequestedFriends()
    {
        if (friendRequest.size() == 0)
        {
            cout << "You have no requests!" << endl;
        }
        else
        {
            for (int i = 0; i < friendRequest.size(); i++)
            {
                cout << i + 1 << ". " << friendRequest[i] << endl;
            }
        };
    };
    void requestFriend(string id)
    {
        friendRequest.push_back(id);
    }
    string getFriendID(string _index)
    {
        int index = stoi(_index) - 1;
        return friendRequest[index];
    }
    void addFriend(string id)
    {
        friendList.push_back(id);
    };
    void eraseFriendRequest(string _index)
    {
        int index = stoi(_index) - 1;
        vector<string>::iterator it = friendRequest.begin() + index;
        friendRequest.erase(it);
    };
    void eraseFriend(string _index)
    {
        int index = stoi(_index) - 1;
        vector<string>::iterator it = friendList.begin() + index;
        friendList.erase(it);
    };
    int verifyPassword(string _password)
    {
        return (password == _password);
    }
    int numberOfFriendRequests()
    {
        return friendRequest.size();
    }
    void print()
    {
        cout << id << endl;
    };
    void deleteFriendIfExists(string userID)
    {
        vector<string>::iterator it;
        for (int i = 0; i < friendList.size(); i++)
        {
            if (friendList[i] == userID)
            {
                it = friendList.begin() + i;
                friendList.erase(it);
            }
        }
        for (int i = 0; i < friendRequest.size(); i++)
        {
            if (friendRequest[i] == userID)
            {
                it = friendRequest.begin() + i;
                friendRequest.erase(it);
            }
        }
    };
    string saveableForm()
    {
        return id + "," + password + "," + "1";
    }
    ~User()
    {
        cout << "Deleted User" << endl;
    }
};

class Admin : private Profile
{
public:
    Admin(string _id, string _password)
    {
        id = _id;
        password = _password;
    };
    int verify(string _id, string _password)
    {
        if (id != _id)
        {
            return 0;
        }
        else if (password != password)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
    void print()
    {
        cout << id << endl;
    };
    string saveableForm()
    {
        return id + "," + password + "," + "0";
    };
    ~Admin()
    {
        cout << "Deleted admin" << endl;
    }
};

class Post
{
private:
    int post_id;
    string author;
    string text;

public:
    Post(int _post_id, string _author, string _text)
    {
        post_id = _post_id;
        author = _author;
        text = _text;
    };
    string getAuthor()
    {
        return author;
    };
    int getPostID()
    {
        return post_id;
    }
    void print()
    {
        cout << "------------------" << endl;
        cout << "Post ID: " << post_id << endl;
        cout << "Author: " << author << endl;
        cout << text << endl;
    };
    string saveableForm()
    {
        return to_string(post_id) + "," + author + "," + text;
    };
    ~Post()
    {
        cout << "Deleted post" << endl;
    }
};

class Pair
{
private:
    string first;
    string second;

public:
    Pair(string _first, string _second)
    {
        first = _first;
        second = _second;
    };
    string saveableForm()
    {
        string saveable = first + "," + second;
        return saveable;
    }
    ~Pair()
    {
        cout << "Deleted pair" << endl;
    }
};

class Database
{
private:
    map<string, User *> userMap;
    User *loggedinUser;
    Admin *admin;
    vector<Post *> postList;
    set<Pair *> friendRelationships;
    friend class Instance;

public:
    Database(const string profile_file_name, const string post_file_name, const string friendship_file_name)
    {
        //initialize profiles
        string line;
        string data[3];
        ifstream inputFile;
        ofstream outputFile;
        inputFile.open(profile_file_name);
        while (getline(inputFile, line))
        {
            string buffer = "";
            int count = 0;
            for (int i = 0; i < line.length(); i++)
            {
                if (line[i] == ',')
                {
                    data[count] = buffer;
                    buffer = "";
                    count++;
                }
                else
                {
                    buffer += line[i];
                }
            }
            data[count] = buffer;
            if (stoi(data[2]) == 1)
            {
                User *newUser = new User(data[0], data[1]);
                userMap[data[0]] = newUser;
            }
            else
            {
                admin = new Admin(data[0], data[1]);
            }
        }
        inputFile.close();

        //Initialize posts
        inputFile.open(post_file_name);
        while (getline(inputFile, line))
        {
            string buffer = "";
            int count = 0;
            for (int i = 0; i < line.length(); i++)
            {
                if (line[i] == ',')
                {
                    data[count] = buffer;
                    buffer = "";
                    count++;
                }
                else
                {
                    buffer += line[i];
                }
            }
            data[count] = buffer;
            Post *post = new Post(stoi(data[0]), data[1], data[2]);
            postList.push_back(post);
        }
        inputFile.close();

        //Initialize friends
        inputFile.open(friendship_file_name);
        while (getline(inputFile, line))
        {
            string buffer = "";
            int count = 0;
            for (int i = 0; i < line.length(); i++)
            {
                if (line[i] == ',')
                {
                    data[count] = buffer;
                    buffer = "";
                    count++;
                }
                else
                {
                    buffer += line[i];
                }
            }
            data[count] = buffer;

            userMap[data[0]]->addFriend(data[1]);
            userMap[data[1]]->addFriend(data[0]);
        }
        inputFile.close();
    };
    void setLoggedinUser(string id)
    {
        loggedinUser = userMap[id];
    };
    int verify(string id, string password)
    {
        if (userMap.find(id) == userMap.end())
        {
            //User doesn't exist!
            return 2;
        }
        else
        {
            if (userMap[id]->verifyPassword(password))
            {
                setLoggedinUser(id);
                //Login Successful!
                return 1;
            }
            else
            {
                //wrong password
                return 0;
            };
        }
    };
    void addNewPost(Post *newPost)
    {
        postList.push_back(newPost);
    };
    void deletePost(string postIDToDelete)
    {
        int postID = stoi(postIDToDelete);
        int index = -1;
        for (int i = 0; i < postList.size(); i++)
        {
            if (postList[i]->getPostID() == postID)
            {
                index = i;
                break;
            }
        }
        if (index != -1)
        {
            vector<Post *>::iterator it = postList.begin() + index;
            postList.erase(it);
            cout << "Post with the id " << postIDToDelete << " deleted successfully" << endl;
        }
        else
        {
            cout << "Post with the id " << postIDToDelete << " doesn't exist, or you are not the author." << endl;
        }
    }
    void deleteUser(string userID)
    {
        map<string, User *>::iterator it;
        if (userMap.find(userID) == userMap.end())
        {
            cout << "The user doesn't exist!" << endl;
        }
        else
        {
            User *deletedUser = userMap[userID];
            for (it = userMap.begin(); it != userMap.end(); it++)
            {
                it->second->deleteFriendIfExists(userID);
            }
            userMap.erase(userID);
            cout << "The user with id " << userID << " was deleted successfully." << endl;
        }
    };
    void save(const string profile_file_name, const string post_file_name, const string friendship_file_name)
    {
        //loop through userMap, save pairs into set
        fstream outputFile;
        string profileData = "";
        string postData = "";
        map<string, User *>::iterator mapIterator;
        for (mapIterator = userMap.begin(); mapIterator != userMap.end(); mapIterator++)
        {
            string first, second, temp;
            User *self = mapIterator->second;
            profileData += self->saveableForm() + "\n";

            first = self->getID();
            for (int i = 0; i < self->friendList.size(); i++)
            {
                second = self->friendList[i];
                if (first < second)
                {
                    Pair *newPair = new Pair(first, second);
                    friendRelationships.insert(newPair);
                }
            }
        };
        //loop through set and save it into friendship.csv file
        set<Pair *>::iterator setIterator;
        string friendshipData = "";
        for (setIterator = friendRelationships.begin(); setIterator != friendRelationships.end(); ++setIterator)
        {
            Pair *currentPair = *setIterator;
            friendshipData += currentPair->saveableForm() + "\n";
        }
        outputFile.open(friendship_file_name);
        friendshipData = friendshipData.substr(0, friendshipData.length() - 1);
        outputFile << friendshipData << endl;
        outputFile.close();
        //loop through userMap and save it into profile.csv file
        outputFile.open(profile_file_name);
        profileData += admin->saveableForm();
        outputFile << profileData << endl;
        outputFile.close();
        //loop through postList and save it into post.csv file
        for (int i = 0; i < postList.size(); i++)
        {
            postData += postList[i]->saveableForm() + "\n";
        }
        postData = postData.substr(0, postData.length() - 1);
        outputFile.open(post_file_name);
        outputFile << postData << endl;
        outputFile.close();
        cout << "Saved instance" << endl;
        cout << "--------------" << endl;
    }
    ~Database()
    {
        //delete usermap
        map<string, User *>::iterator it;
        for (it = userMap.begin(); it != userMap.end(); it++)
        {
            delete it->second;
        }
        //delete admin
        delete admin;
        //delete post vector
        for (int i = 0; i < postList.size(); i++)
        {
            delete postList[i];
        }
        //delete pair set
        set<Pair *>::iterator setIterator;
        for (setIterator = friendRelationships.begin(); setIterator != friendRelationships.end(); ++setIterator)
        {
            delete *setIterator;
        }
        cout << "Deleted database" << endl;
    }
};

class Instance
{
private:
    Database *database;

public:
    Instance(Database *_database)
    {
        database = _database;
    };
    char loginPage()
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
    int login()
    {
        string id, password;
        int isLoggedin = 0;
        char retry = 'y';

        while (isLoggedin < 1)
        {
            cout << "========================================" << endl;
            cout << "Enter ID: " << endl;
            cin >> id;
            cout << "Enter password: " << endl;
            cin >> password;
            int verificationStatus = database->verify(id, password);
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
    void signUp()
    {
        string id, password, verification;
        cout << "Enter your id" << endl;
        cin >> id;
        //check if id exists
        if (database->userMap.find(id) != database->userMap.end())
        {
            cout << "The user already exists!" << endl;
        }
        else
        {
            cout << "Enter your password" << endl;
            cin >> password;
            cout << "Verify your password" << endl;
            cin >> verification;
            if (password != verification)
            {
                cout << "password does not match!" << endl;
            }
            else
            {
                User *newUser = new User(id, password);
                database->userMap[id] = newUser;
            }
        }
    };
    void adminLogin()
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
    int getNewPostID()
    {
        int last = database->postList.size() - 1;
        return database->postList[last]->getPostID() + 1;
    };
    void userShowFeed()
    {
        int feedCount = 0;
        cout << "------------------" << endl;
        cout << "My Feed" << endl;
        for (int i = database->postList.size() - 1; i >= 0; i--)
        {
            string author = database->postList[i]->getAuthor();
            if ((database->loggedinUser->isFriend(author)) || (author == database->loggedinUser->getID()))
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
    void userShowFriendList()
    {
        cout << "------------------" << endl;
        cout << "My Friends" << endl;
        cout << "------------------" << endl;
        database->loggedinUser->showFriends();
    };
    void userRequestFriend()
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
            database->userMap[friendToRequest]->requestFriend(database->loggedinUser->getID());
            cout << "Friend requested!" << endl;
        }
    };
    void userAcceptFriend()
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
    void userUnfriend()
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
    int userShowMyPosts()
    {
        int myPostCount = 0;
        cout << "------------------" << endl;
        cout << "My Posts" << endl;
        for (int i = database->postList.size() - 1; i >= 0; i--)
        {
            string author = database->postList[i]->getAuthor();
            if (author == database->loggedinUser->getID())
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
    void userPost()
    {
        Post *newPost;
        string newPostText;
        cout << "------------------" << endl;
        cout << "Enter a sentence you want to post: " << endl;
        cin.ignore();
        getline(cin, newPostText);

        newPost = new Post(getNewPostID(), database->loggedinUser->getID(), newPostText);
        database->addNewPost(newPost);
        cout << "Post added successfully!" << endl;
    };
    void userDeletePost()
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
    void printUserPromptMessage()
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
    void userPrompt()
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
    void adminShowPosts()
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
    void adminShowUsers()
    {
        map<string, User *>::iterator it;
        cout << "------------------" << endl;
        cout << "Users" << endl;

        for (it = database->userMap.begin(); it != database->userMap.end(); it++)
        {
            it->second->print();
        }
    };
    void adminDeletePost()
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
    void adminDeleteUser()
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
    void printAdminPromptMessage()
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
    void adminPrompt()
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

    void startApp()
    {
        char loginType;
        bool appRunning = true;
        while (appRunning)
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
    void save(const string profile_file_name, const string post_file_name, const string friendship_file_name)
    {
        database->save(profile_file_name, post_file_name, friendship_file_name);
    }
    ~Instance()
    {
        cout << "Deleted Instance" << endl;
    }
};

int main()
{
    //initialize
    Database database(profile_file_name, post_file_name, friendship_file_name);

    //start application
    Instance instance(&database);
    instance.startApp();
    //save
    instance.save(profile_file_name, post_file_name, friendship_file_name);
    //clear memory

    return 0;
}