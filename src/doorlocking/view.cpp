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
  large_file_handler("/menu_user.html", "text/html", false);
  if(msg != NULL && msg != ""){
    sendVariable("msg", msg);
  }
}

void View::menu_admin(String msg){
  large_file_handler("/menu_admin.html", "text/html", false);
  if(msg != NULL && msg != ""){
    sendVariable("msg", msg);
  }
}

void View::login_page(String msg){
  large_file_handler("/login.html", "text/html", false);
  if(msg != NULL && msg != ""){
    sendVariable("msg", msg);
  }
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
        String headers  = "HTTP/1.1 200 OK\r\n" 
                          "Cache-Control: no-cache;\r\n"
                          "Set-Cookie: __Secure-Session-ID=" + session.sessionId + "; Secure; HttpOnly; SameSite=Strict; Max-Age=2147483647;\r\n";
        server->sendContent(headers);
        this->is_response_started = true;
        
        //redirect to menu page
        if(u.type == ADMIN){
          menu_admin("<div class='alert alert-success' role='alert'>Welcome " + u.username + "</div>");
        }else{
          menu_user("<div class='alert alert-success' role='alert'>Welcome " + u.username + "</div>");
        }
      }else{
        //redirect to login page, failed to create cookie.
        login_page("<div class='alert alert-danger' role='alert'>Authentication Failed</div>");
      }
    }else{
      //authentication fail

      //increment the fail counter for the user

      //redirect to login page
      login_page("<div class='alert alert-danger' role='alert'>Authentication Failed</div>");
    }
  }else{
    //redirect to login page, missing field in form
    login_page("<div class='alert alert-danger' role='alert'>Authentication Failed</div>");
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
    String headers  = "HTTP/1.1 200 OK\r\n" 
                          "Cache-Control: no-cache;\r\n"
                          "Set-Cookie: __Secure-Session-ID=0; Secure; HttpOnly; SameSite=Strict; Max-Age=0;\r\n";
    server->sendContent(headers);
    this->is_response_started = true;
    
    login_page("<div class='alert alert-success' role='alert'>Logout Successful</div>");  
  }else{
    menu("<div class='alert alert-danger' role='alert'>Unable to logout</div>");
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
    strUsersList += (users[i].userStatus == INACTIVE ? "<button class='button-error pure-button' onclick='location.href=\'/activate_user_handler?username=" + users[i].username + ""
                                         : "<button class='button-success pure-button' onclick='location.href=\'/deactivate_user_handler?username=" + users[i].username);
    strUsersList += "\''>";
    strUsersList += (users[i].userStatus == INACTIVE ? "Inactive" : "Active");
    strUsersList += "</button></td></tr>";
  }

  large_file_handler("/list_users.html", "text/html", false);
  if(msg != NULL && msg != ""){
    sendVariable("msg", msg);
  }
  if(strUsersList != NULL && strUsersList != ""){
    sendVariable("users", strUsersList);
  }
  
}

void View::register_user_page(){
  UserSession session;
  if(!is_authenticated(session)){
    login_page("");
    return;
  }
  large_file_handler("/register_user.html", "text/html", false);
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
      msg = "<div class='alert alert-success' role='alert'>User registred with sucess! Access Code: " + accessCode + "</div>";
      redirectPage = "/menu_admin.html";
    }else{
      msg = "<div class='alert alert-danger' role='alert'>Username already exists, chose other username.</div>";
      redirectPage = "/register_user.html";
    }
  }else{
    msg = "<div class='alert alert-danger' role='alert'>Inform a non blank username.</div>";
    redirectPage = "/register_user.html";
  }

  large_file_handler(redirectPage, "text/html", false);
  if(msg != NULL && msg != ""){
    sendVariable("msg", msg);
  }
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
      msg = "<div class='alert alert-success' role='alert'>User activated with success!</div>";
    }else{
      msg = "<div class='alert alert-danger' role='alert'>An problem occurred during user activation.</div>";
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
      msg = "<div class='alert alert-success' role='alert'>User deactivated with success!</div>";
      if(invalidadeSessions(username)){
        msg += "User sessions invalidated.";
      }else{
        msg += "No session invalidated.";
      }
    }else{
      msg = "<div class='alert alert-danger' role='alert'>An problem occurred during user deactivation.</div>";
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
  
  digitalWrite(DOOR_PIN, LOW);
  delay(500);
  digitalWrite(DOOR_PIN, HIGH);
  
  menu("<div class='alert alert-success' role='alert'>Door Succefully Opened</div>");
}

void View::large_file_handler(String fileName, String contentType, bool gziped){
  if(gziped){
    fileName += ".gz";
  }
  File f = SPIFFS.open(fileName,"r");
  int fileSize = f.size();
  
  String headers  = "";
  if(!this->is_response_started){
    headers  += "HTTP/1.1 200 OK\r\n";
  }else{
    this->is_response_started = false;
  }

  headers += "Content-Type: " + contentType +  "\r\n"; 

  if(gziped){
    headers += "Content-Encoding: gzip\r\n";
    headers += "Cache-Control: public, max-age=31536000, immutable\r\n";
    headers += "Content-length: " + String(fileSize) + "\r\n";
  }
  headers += "\r\n";
  Serial.println(headers);
  server->sendContent(headers);
  
  int chunkSize = 1024;
  char buf[chunkSize];
  int numberOfChunks = (fileSize / chunkSize) + 1;
  
  int count = 0;
  int remainingChunks = fileSize;
  int bytesSent = 0;
  for (int i = 1; i <= numberOfChunks; i++){
    Serial.println(i);
    if (remainingChunks - chunkSize < 0){
      chunkSize = remainingChunks;
    }
    f.read((uint8_t *)buf, chunkSize);
    remainingChunks = remainingChunks - chunkSize;
    server->sendContent_P(buf, chunkSize);
    bytesSent += chunkSize;
  }
  Serial.println("bytes sent:" + String(bytesSent));
  f.close();
  
}

void View::sendVariable(String variable, String content){
  String script = "<script language=\"javascript\">"
                    "document.getElementById('variableName').innerHTML=\"content\";"
                  "</script>";
  script.replace("variableName", variable);
  script.replace("content", content);
  Serial.println(script);
  server->sendContent(script);
}

void View::config_render(){
  Serial.print("Config Render");
  //String configHtml = readFile("/config.html");
  //server->send(200, "text/html", configHtml);
  large_file_handler("/config.html", "text/html", false);
}

void View::config_handler(){
  NetworkConfig config;
  if(server->hasArg("hostname") && server->hasArg("ssid") && server->hasArg("pwd")){
    config.hostname = server->arg("hostname");
    config.ssid = server->arg("ssid");
    config.pwd = server->arg("pwd");
    
    setNetworkConfig("/network.txt", config);
    large_file_handler("/config.html", "text/html", false);
    sendVariable("msg", "Config saved successfully! Reset your device to apply the new network configuration and access <a href='https://" + 
                          config.hostname + "'>https://" + 
                          config.hostname + "</a>\"");
  }else{
    large_file_handler("/config.html", "text/html", false);
    sendVariable("msg", "There are errors in your configuration. Fill all fields.");
  }

}
