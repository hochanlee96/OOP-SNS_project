#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <stdlib.h>

using namespace std;

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
    User(string _id, string _password);
    string getID();
    bool isFriend(string userID);
    void showFriends();
    void showRequestedFriends();
    void requestFriend(string id);
    string getFriendID(string _index);
    void addFriend(string id);
    void eraseFriendRequest(string _index);
    void eraseFriend(string _index);
    int verifyPassword(string _password);
    int numberOfFriendRequests();
    void print();
    void deleteFriendIfExists(string userID);
    string saveableForm();
    ~User();
};

class Admin : private Profile
{
public:
    Admin(string _id, string _password);
    int verify(string _id, string _password);
    void print();
    string saveableForm();
    ~Admin();
};

class Post
{
private:
    int post_id;
    string author;
    string text;

public:
    Post(int _post_id, string _author, string _text);
    string getAuthor();
    int getPostID();
    void print();
    string saveableForm();
    ~Post();
};

class Pair
{
private:
    string first;
    string second;

public:
    Pair(string _first, string _second);
    string saveableForm();
    ~Pair();
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
    Database(const string profile_file_name, const string post_file_name, const string friendship_file_name);
    void setLoggedinUser(string id);
    int verify(string id, string password);
    void addNewPost(Post *newPost);
    void deletePost(string postIDToDelete);
    void deleteUser(string userID);
    void save(const string profile_file_name, const string post_file_name, const string friendship_file_name);
    ~Database();
};

#endif