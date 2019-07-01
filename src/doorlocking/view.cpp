#include "view.h"
#include "control.h"

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
  
}

void View::list_users(){
  server->send(200, "text/html", readFile("/list_users.html"));
}

void View::register_user_page(){
  server->send(200, "text/html", readFile("/register_user.html"));
}

void View::register_user_handler(){
  
}

void View::activate_user_handler(){
  
}

void View::deactivate_user_handler(){
  
}

void View::view_logs_handler(){
  
}
