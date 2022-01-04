#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <stdlib.h>
#include "datastructures.h"

using namespace std;

User::User(string _id, string _password) //user constructor
{
    id = _id;
    password = _password;
};
string User::getID() //get userID
{
    return id;
};
bool User::isFriend(string userID) //check if the user is the friend with the userID in the argument
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
void User::showFriends() //print all the user's friends
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
void User::showRequestedFriends() //show all the users who sent the friend request to the user
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
void User::requestFriend(string id) //request friend to another user
{
    friendRequest.push_back(id);
}
string User::getFriendID(string _index) //get the id of the friend
{
    int index = stoi(_index) - 1;
    return friendRequest[index];
}
void User::addFriend(string id) //accept friend
{
    friendList.push_back(id);
};
void User::eraseFriendRequest(string _index) //erase the friend request
{
    int index = stoi(_index) - 1;
    vector<string>::iterator it = friendRequest.begin() + index;
    friendRequest.erase(it);
};
void User::eraseFriend(string _index) //unfriend
{
    int index = stoi(_index) - 1;
    vector<string>::iterator it = friendList.begin() + index;
    friendList.erase(it);
};
int User::verifyPassword(string _password) //verify if the password is correct
{
    return (password == _password);
}
int User::numberOfFriendRequests() //return the number of friend requests received
{
    return friendRequest.size();
}
void User::print() //print the id of the user
{
    cout << id << endl;
};
void User::deleteFriendIfExists(string userID) //delete the userID from the two lists
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
string User::saveableForm() //returns the string that is properly formated to be saved in the csv file
{
    return id + "," + password + "," + "1";
}
User::~User(){
    // cout << "Deleted User" << endl;
};

Admin::Admin(string _id, string _password) //admin constructor
{
    id = _id;
    password = _password;
};
int Admin::verify(string _id, string _password) //authenticate the admin
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
void Admin::print()
{
    cout << id << endl;
};
string Admin::saveableForm()
{
    return id + "," + password + "," + "0";
};
Admin::~Admin()
{
    // cout << "Deleted admin" << endl;
}

Post::Post(int _post_id, string _author, string _text) //constructor for a post
{
    post_id = _post_id;
    author = _author;
    text = _text;
};
string Post::getAuthor() //return the author
{
    return author;
};
int Post::getPostID() //return the post id of the post
{
    return post_id;
}
void Post::print() //print the post
{
    cout << "------------------" << endl;
    cout << "Post ID: " << post_id << endl;
    cout << "Author: " << author << endl;
    cout << text << endl;
};
string Post::saveableForm()
{
    return to_string(post_id) + "," + author + "," + text;
};
Post::~Post()
{
    // cout << "Deleted post" << endl;
}

Pair::Pair(string _first, string _second)
{
    first = _first;
    second = _second;
};
string Pair::saveableForm()
{
    string saveable = first + "," + second;
    return saveable;
}
Pair::~Pair()
{
    // cout << "Deleted pair" << endl;
}

Database::Database(const string profile_file_name, const string post_file_name, const string friendship_file_name)
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
            User *newUser = new User(data[0], data[1]); //for the line read, create a new user object
            userMap[data[0]] = newUser;
        }
        else
        {
            admin = new Admin(data[0], data[1]); // if it is the admin, create the admin object
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
        Post *post = new Post(stoi(data[0]), data[1], data[2]); //for each line read, create a new post object
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
        //add to friend list on both sides of the pair
        userMap[data[0]]->addFriend(data[1]);
        userMap[data[1]]->addFriend(data[0]);
    }
    inputFile.close();
};
void Database::setLoggedinUser(string id) //set loggedinUser as the user curretly logged in
{
    loggedinUser = userMap[id];
};
int Database::verify(string id, string password)
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
void Database::addNewPost(Post *newPost) //add new post
{
    postList.push_back(newPost);
};
void Database::deletePost(string postIDToDelete) //delete the post
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
        //if the post does not exist
        cout << "Post with the id " << postIDToDelete << " doesn't exist, or you are not the author." << endl;
    }
}
void Database::deleteUser(string userID) //delete the user
{
    map<string, User *>::iterator it;
    if (userMap.find(userID) == userMap.end()) // the user does not exist
    {
        cout << "The user doesn't exist!" << endl;
    }
    else
    {
        User *deletedUser = userMap[userID];
        for (it = userMap.begin(); it != userMap.end(); it++) //if user is deleted, delete the user id from all the friendlists of other users
        {
            it->second->deleteFriendIfExists(userID);
        }
        userMap.erase(userID);
        cout << "The user with id " << userID << " was deleted successfully." << endl;
    }
};
void Database::save(const string profile_file_name, const string post_file_name, const string friendship_file_name)
{
    //loop through userMap, save pairs into set
    fstream outputFile;
    string profileData = "";
    string postData = "";
    map<string, User *>::iterator mapIterator;
    //iterate through the map
    for (mapIterator = userMap.begin(); mapIterator != userMap.end(); mapIterator++)
    {
        string first, second, temp;
        User *self = mapIterator->second;
        profileData += self->saveableForm() + "\n";

        first = self->getID();
        for (int i = 0; i < self->friendList.size(); i++)
        {
            second = self->friendList[i];
            if (first < second) //avoid duplicate pairs
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
    //write into the file
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
Database::~Database()
{
    //delete usermap
    map<string, User *>::iterator it;
    for (it = userMap.begin(); it != userMap.end(); it++)
    {
        delete it->second;
    }
    cout << "Deleted Users" << endl;
    //delete admin
    delete admin;
    cout << "Deleted Admin" << endl;
    //delete post vector
    for (int i = 0; i < postList.size(); i++)
    {
        delete postList[i];
    }
    cout << "Deleted Posts" << endl;
    //delete pair set
    set<Pair *>::iterator setIterator;
    for (setIterator = friendRelationships.begin(); setIterator != friendRelationships.end(); ++setIterator)
    {
        delete *setIterator;
    }
    cout << "Deleted Pairs" << endl;
    cout << "Deleted database" << endl;
};