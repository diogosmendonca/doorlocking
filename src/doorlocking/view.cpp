#include "view.h"

View::View(ESP8266WebServerSecure* server){
	this->server = server;	
}

void View::home(){
	login();
}

void View::menu(){
	
}

void View::login(){
	String page = 
	"<!DOCTYPE html>"
	"<html lang='en'>"
	"<head>"
		"<title>Login</title>"
	"</head>"
	"<body>"
		"<h1>Login</h1>"
		"<form name='login' action='/login' method='POST'>"
			"<fieldset>"
				"<legend>Authentication</legend>"
				"<label for='username'>Username:</lable><br/>"
				"<input type='text' name='username' id='username'/><br/>"
				"<label for='access_code'>Acess Code:</lable><br/>"
				"<input type='text' name='access_code' id='access_code'/><br/>"
				"<button type='submit'>Enter</button>"
			"</fieldset>"
		"</form>"
	"</body>"
	"</html>";
  server->send(200, "text/html", page);
}
