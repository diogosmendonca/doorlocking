#include "view.h"
#include "control.h"
#include <climits>
#include <vector>

View::View(ESP8266WebServerSecure* server){
	this->server = server;	
}

void View::home(){
  login_page();
}

void View::menu(){
  menu_admin();
}

void View::menu_user(){
  server->send(200, "text/html", readFile("/menu_user.html"));
}

void View::menu_admin(){
  server->send(200, "text/html", readFile("/menu_admin.html"));
}

void View::login_page(){
  server->send(200, "text/html", readFile("/login.html"));
}

void View::login_handler(){
  bool authentication = false;
  if(server->hasArg("login") && server->hasArg("password")){
    authentication = authenticate(server->arg("login"), server->arg("password"));
  }
  
}

void View::list_users(){
  server->send(200, "text/html", readFile("/list_users.html"));
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
  
}

void View::deactivate_user_handler(){
  
}

void View::view_logs_handler(){
  
}

void View::open_door_handler(){
  
}
