#ifndef INSTANCE_H
#define INSTANCE_H
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <stdlib.h>
#include "datastructures.h"

using namespace std;
class Instance
{
private:
    Database *database;

public:
    Instance(Database *_database);
    char loginPage();
    int login();
    void signUp();
    void adminLogin();
    int getNewPostID();
    void userShowFeed();
    void userShowFriendList();
    void userRequestFriend();
    void userAcceptFriend();
    void userUnfriend();
    int userShowMyPosts();
    void userPost();
    void userDeletePost();
    void printUserPromptMessage();
    void userPrompt();
    void adminShowPosts();
    void adminShowUsers();
    void adminDeletePost();
    void adminDeleteUser();
    void printAdminPromptMessage();
    void adminPrompt();
    void startApp();
    void save(const string profile_file_name, const string post_file_name, const string friendship_file_name);
    ~Instance();
};

#endif