#include "view.h"
#include "control.h"
#include <climits>
#include <vector>

View::View(ESP8266WebServerSecure* server){
	this->server = server;	
}

void View::home(){
  UserSession session;
  if(!is_authenticated(session)){
    login_page("");
    return;
  }
  
  menu("");
}

void View::menu(String msg){
  UserSession session;
  User user;
  
  if(!is_authenticated(session)){
    login_page("");
    return;
  }
  
  user.username = session.username;
  retrieveUser(user);
  if(user.type == ADMIN){
    menu_admin(msg);
  }else{
    menu_user(msg);
  }
  
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
  UserSession session;
  bool isSessionCreated; 
  
  if(server->hasArg("username") && server->hasArg("accessCode")){
    if(authenticate(server->arg("username"), server->arg("accessCode"), u)){
      //create a new session
      isSessionCreated = createSession(u, session);
      
      //set a cookie in the header
      if(isSessionCreated){
        server->sendHeader("Cache-Control", "no-cache");
        server->sendHeader("Set-Cookie", "__Secure-Session-ID=" + session.sessionId + "; Secure; HttpOnly; SameSite=Strict; Max-Age=2147483647;");
        
        //redirect to menu page
        if(u.type == ADMIN){
          menu_admin("Welcome " + u.username);
        }else{
          menu_user("Welcome " + u.username);
        }
      }else{
        //redirect to login page, failed to create cookie.
        login_page("Authentication Failed");
      }
    }else{
      //authentication fail

      //increment the fail counter for the user

      //redirect to login page
      login_page("Authentication Failed");
    }
  }else{
    //redirect to login page, missing field in form
    login_page("Authentication Failed");
  }
  
}

//Check if header is present and correct
bool View::is_authenticated(UserSession& session) {
  String cookieName = "__Secure-Session-ID=";
  int cookieStartIndex, cookieEndIndex;
  bool sessionFound = false;
  String sessionId;
  
  Serial.println("Enter is_authenticated");
  if(server->hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server->header("Cookie");
    Serial.println(cookie);
    cookieStartIndex = cookie.indexOf(cookieName);
    if(cookieStartIndex != -1) {
      //get the session number
      cookieStartIndex += cookieName.length();
      cookieEndIndex = cookie.indexOf(";", cookieStartIndex);
      sessionId = cookie.substring(cookieStartIndex, cookieEndIndex);
      Serial.println("Session Id = " + sessionId);
      
      //retrieve the session object
      sessionFound = retrieveSession(sessionId, session);
      if(sessionFound){
        Serial.println( session.sessionStatus == ACTIVE ? "Authentication Successful" : "Authentication Failed - Session Inactive");
        return session.sessionStatus;
      }else{
        Serial.println("Session not found");
        return false;
      }
      
    }
  }
  Serial.println("Authentication Failed");
  return false;
}

void View::logout_handler(){
  UserSession session;
  if(!is_authenticated(session)){
    login_page("");
    return;
  }
  
  //invalidate the actual session
  if(invalidadeSession(session.sessionId)){
    //remove cookie
    server->sendHeader("Cache-Control", "no-cache");
    server->sendHeader("Set-Cookie", "__Secure-Session-ID=0; Secure; HttpOnly; SameSite=Strict; Max-Age=0;" );
    login_page("Logout Successful");  
  }else{
    menu("Unable to logout");
  }
  
}

void View::list_users(String msg){
  UserSession session;
  if(!is_authenticated(session)){
    login_page("");
    return;
  }
  
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
  UserSession session;
  if(!is_authenticated(session)){
    login_page("");
    return;
  }
  
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
  UserSession session;
  if(!is_authenticated(session)){
    login_page("");
    return;
  }
  
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
  UserSession session;
  if(!is_authenticated(session)){
    login_page("");
    return;
  }

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
  UserSession session;
  if(!is_authenticated(session)){
    login_page("");
    return;
  }
  
  String username;
  String msg;
  if(server->hasArg("username")){
    username = server->arg("username");
    if(changeUserState(username, INACTIVE)){
      msg = "User deactivated with success!";
      if(invalidadeSessions(username)){
        msg += "User sessions invalidated.";
      }else{
        msg += "No session invalidated.";
      }
    }else{
      msg = "An problem occurred during user deactivation.";
    }
  }
  this->list_users(msg);
}

void View::view_logs_handler(){
  UserSession session;
  if(!is_authenticated(session)){
    login_page("");
    return;
  }
  
  String f = readFile("/sessions.txt");
  server->send(200, "text/plain", f);
}

void View::open_door_handler(){
  UserSession session;
  if(!is_authenticated(session)){
    login_page("");
    return;
  }
  menu("Door Succefully Opened");
}
