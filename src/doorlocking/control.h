#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <FS.h>
#include "model.h"
#include <vector>

#define MAX_INT 2147483647
#define ACCESS_CODE_SIZE 10

using namespace std;

String readFile(String fileName);

String newAccessCode();

bool authenticate(String login, String password);

String registerUser(String username);

void getUsers(vector<User> &users);

bool usernameExists(String username, vector<User> &users);

bool changeUserState(String username, enum StatusEnum state);

#endif
