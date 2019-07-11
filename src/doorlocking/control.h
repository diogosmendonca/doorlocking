#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <FS.h>
#include "model.h"
#include <vector>

using namespace std;

String readFile(String fileName);

String ramdomString(int size);

bool authenticate(String username, String accessCode, User& u);

String registerUser(String username);

void getUsers(vector<User> &users);

void retrieveUser(User& user);

bool usernameExists(String username, vector<User> &users);

bool changeUserState(String username, enum StatusEnum state);

bool createSession(User& user, UserSession& session);

bool retrieveSession(String sessionIdParam, UserSession& session);

bool invalidadeSessions(String username);

#endif
