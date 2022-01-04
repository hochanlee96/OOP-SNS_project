#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <stdlib.h>
#include "datastructures.h"
#include "instance.h"
using namespace std;

//names of the csv files
const string profile_file_name = "profile.csv";
const string post_file_name = "post.csv";
const string friendship_file_name = "friendship.csv";

int main()
{
    //initialize
    Database database(profile_file_name, post_file_name, friendship_file_name);

    //start application
    Instance instance(&database);
    instance.startApp();
    //save and clear memory
    instance.save(profile_file_name, post_file_name, friendship_file_name);

    return 0;
}