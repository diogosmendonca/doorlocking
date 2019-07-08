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

String newAccessCode(){
  String accessCode;
  for(int i = 0; i < ACCESS_CODE_SIZE; i++){
    byte randomValue = random(0, 37);
    char letter = randomValue + 'a';
    if(randomValue > 26)
      letter = (randomValue - 26) + '0';
    accessCode += letter;
  }
  
  return accessCode;
}

bool authenticate(String login, String password){
  return false;
}


void getUsers(vector<User>& users){
  File f = SPIFFS.open("users.txt", "r");
  String line;
  User u;
  while (f.available()) {
    line = f.readStringUntil('\n');
    u.username = getValue(line, '|', 0);
    u.accessCode = getValue(line, '|', 1);
    u.userStatus = static_cast<StatusEnum>(getValue(line, '|', 2).toInt());
    users.push_back(u);
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
  String accessCode = newAccessCode();
  File f = SPIFFS.open("users.txt", "a");
  f.println(username + "|" + accessCode + "|" + ACTIVE);
  f.close();
  return accessCode;
}
