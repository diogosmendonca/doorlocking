#include "control.h"

using namespace std;

String getValue(String data, char separator, int index){
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String readFile(String fileName){
  File rFile = SPIFFS.open(fileName,"r");
  String sFile = rFile.readString();
  rFile.close();
  return sFile;
}

String ramdomString(int strSize){
  String accessCode;
  for(int i = 0; i < strSize; i++){
    byte randomValue = random(0, 37);
    char letter = randomValue + 'a';
    if(randomValue > 26)
      letter = (randomValue - 26) + '0';
    accessCode += letter;
  }
  
  return accessCode;
}

bool authenticate(String username, String accessCode, User& u){
  vector<User> users;
  getUsers(users);
  bool authenticated = false;
  for(int i = 0; i < users.size(); i++){
    if((users[i].username == username) && 
       (users[i].userStatus == ACTIVE) && 
       (users[i].accessCode == accessCode)){
      authenticated = true;
      u = users[i];
    }
  }
  
  return authenticated;
}


void getUsers(vector<User>& users){
  File f = SPIFFS.open("/users.txt", "r");
  String line;
  User u;
  while (f.available()) {
    line = f.readStringUntil('\n');
    u.username = getValue(line, '|', 0);
    u.accessCode = getValue(line, '|', 1);
    u.userStatus = static_cast<StatusEnum>(getValue(line, '|', 2).toInt());
    u.type = static_cast<UserTypeEnum>(getValue(line, '|', 3).toInt());
    users.push_back(u);
  }
  f.close();
}

void retrieveUser(User& user){
  File f = SPIFFS.open("/users.txt", "r");
  String line, username;
  User u;
  while (f.available()) {
    line = f.readStringUntil('\n');
    username = getValue(line, '|', 0);
    if(username == user.username){
      user.accessCode = getValue(line, '|', 1);
      user.userStatus = static_cast<StatusEnum>(getValue(line, '|', 2).toInt());
      user.type = static_cast<UserTypeEnum>(getValue(line, '|', 3).toInt());
    }
  }
  f.close();
}

bool usernameExists(String username, vector<User> &users){
  bool alreadyExists = false;
  for(int i = 0; i < users.size(); i++){
    if(users[i].username == username){
        alreadyExists = true;
    }
  }
  return alreadyExists;
}

String registerUser(String username){
  String accessCode = ramdomString(10);
  File f = SPIFFS.open("/users.txt", "a");
  f.println(username + "|" + accessCode + "|" + ACTIVE + "|" + NORMAL_USER);
  f.close();
  return accessCode;
}


bool changeUserState(String username, enum StatusEnum state){
  String usersFile = readFile("/users.txt");
  File f;
  bool foundField = false;
  int fieldIndex;
  
  //find the user
  int usernameIndex = usersFile.indexOf(username);
  bool foundUser = usernameIndex >= 0;
  
  if(foundUser){
    
    //change its record
    //positioning the cursos in status field
    fieldIndex = usersFile.indexOf('|', usersFile.indexOf('|',usernameIndex)+1);
    
    if (fieldIndex > 0){
      foundField = true;
    
      //set the state
      usersFile.setCharAt(fieldIndex + 1, state == ACTIVE ? '1' : '0');

      //rewrite the whole file
      f = SPIFFS.open("/users.txt", "w");
      
      //write the new state
      f.print(usersFile);
      f.close();
    }
  }
  
  return foundUser && foundField;
}

bool createSession(User& user, UserSession& session){
  String sessionId = ramdomString(16);
  File f = SPIFFS.open("/sessions.txt", "a");
  
  session.username = user.username;
  session.sessionId = sessionId;
  session.sessionStatus = ACTIVE;
  
  f.println(session.sessionId + "|" + session.username + "|" + session.sessionStatus);
  f.close();
  return true;
}

bool retrieveSession(String sessionIdParam, UserSession& session){
  String sessionIdFile;
  bool sessionFound = false;

  File f = SPIFFS.open("/sessions.txt", "r");
  String line, username;
  User u;
  while (f.available()) {
    line = f.readStringUntil('\n');
    Serial.println("line = " + line);
    sessionIdFile = getValue(line, '|', 0);
    if(sessionIdParam == sessionIdFile){
      sessionFound = true;
      session.sessionId = sessionIdFile;
      session.username = getValue(line, '|', 1);
      session.sessionStatus = static_cast<StatusEnum>(getValue(line, '|', 2).toInt());
    }
  }
  f.close();
  
  return sessionFound;
}

bool invalidadeSessions(String username){
  return true;
}
