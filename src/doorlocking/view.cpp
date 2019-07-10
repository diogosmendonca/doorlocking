#include "view.h"
#include "control.h"
#include <climits>
#include <vector>

View::View(ESP8266WebServerSecure* server){
	this->server = server;	
}

void View::home(){
  login_page("");
}

void View::menu(String msg){
  menu_admin(msg);
}

void View::menu_user(String msg){
  String menuPage = readFile("/menu_user.html");
  menuPage.replace("##MSG##", msg);
  server->send(200, "text/html", menuPage);
}

void View::menu_admin(String msg){
  String menuPage = readFile("/menu_admin.html");
  menuPage.replace("##MSG##", msg);
  server->send(200, "text/html", menuPage);
}

void View::login_page(String msg){
  String loginPage = readFile("/login.html");
  loginPage.replace("##MSG##", msg);
  server->send(200, "text/html", loginPage);
}

void View::login_handler(){
  bool authentication = false;
  User u;
  String redirectPage;
  
  if(server->hasArg("username") && server->hasArg("accessCode")){
    if(authenticate(server->arg("username"), server->arg("accessCode"), u)){
      //create a new session
      
      //set a cookie in the header

      //redirect to menu page
      if(u.type == ADMIN){
        menu_admin("Welcome " + u.username);
      }else{
        menu_user("Welcome " + u.username);
      }
    }else{
      //authentication fail

      //increment the fail counter for the user

      //redirect to login page
      login_page("Authentication Failed");
    }
  }else{
    //missing field in form

    //redirect to login page
    login_page("Authentication Failed");
  }
  
}

void View::logout_handler(){
  login_page("");
}

void View::list_users(String msg){
  String strUsersList;
  vector<User> users;
  getUsers(users);

  for(int i = 0; i < users.size(); i++){
    strUsersList += "<tr><td>" + users[i].username + "</td><td>"; 
    strUsersList += (users[i].userStatus == ACTIVE ? "Active" : "Inactive"); 
    strUsersList += "</td><td><a href='"; 
    strUsersList += (users[i].userStatus == INACTIVE ? "/activate_user_handler?username=" + users[i].username 
                                         : "/deactivate_user_handler?username=" + users[i].username);
    strUsersList += "'>";
    strUsersList += (users[i].userStatus == INACTIVE ? "Activate" : "Deactivate");
    strUsersList += "</a></td></tr>";
  }
  String listUsersPage = readFile("/list_users.html");
  listUsersPage.replace("##USERS##", strUsersList);
  listUsersPage.replace("##MSG##", msg);
  server->send(200, "text/html", listUsersPage);
}

void View::register_user_page(){
  String page = readFile("/register_user.html");
  page.replace("##MSG##", "");
  server->send(200, "text/html", page);
}

/**
 * Register a new user, generating a new access code for the user.
 * 
 * @param String username - username passed as argument from the request
 * @return String accessCode - Access code showed to the user. 
 * @modifies user registry - Saves user to the users file.
 */
void View::register_user_handler(){
  vector<User> users;
  String username;
  String msg;
  String accessCode;
  String redirectPage;
  
  if(server->hasArg("username")){
    username = server->arg("username");
    getUsers(users);
    if(!usernameExists(username, users)){
      accessCode = registerUser(username);
      msg = "User registred with sucess! Access Code: " + accessCode;
      redirectPage = "/menu_admin.html";
    }else{
      msg = "Username already exists, chose other username.";
      redirectPage = "/register_user.html";
    }
  }else{
    msg = "Inform a non blank username.";
    redirectPage = "/register_user.html";
  }
  String page = readFile(redirectPage);
  page.replace("##MSG##", msg);
  server->send(200, "text/html", page);
}

void View::activate_user_handler(){
  String username;
  String msg;
  if(server->hasArg("username")){
    username = server->arg("username");
    if(changeUserState(username, ACTIVE)){
      msg = "User activated with success!";
    }else{
      msg = "An problem occurred during user activation.";
    }
  }
  this->list_users(msg);
}

void View::deactivate_user_handler(){
  String username;
  String msg;
  if(server->hasArg("username")){
    username = server->arg("username");
    if(changeUserState(username, INACTIVE)){
      msg = "User deactivated with success!";
    }else{
      msg = "An problem occurred during user deactivation.";
    }
  }
  this->list_users(msg);
}

void View::view_logs_handler(){
  String f = readFile("/users.txt");
  server->send(200, "text/plain", f);
}

void View::open_door_handler(){
  
}
