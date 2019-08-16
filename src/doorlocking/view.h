#ifndef __VIEW_H__
#define __VIEW_H__

#include <ESP8266WebServerSecure.h>
#include "WString.h"
#include "model.h"

#define DOOR_PIN 13

class View{
	private:
		ESP8266WebServerSecure* server;
    bool is_response_started = false;
    bool is_authenticated(UserSession& session);
	public:
		View(ESP8266WebServerSecure* server);
		void home();
    void menu(String msg);
		void menu_user(String msg);
    void menu_admin(String msg);
		void login_page(String msg);
    void login_handler();
    void logout_handler();
	  void list_users(String msg);
    void register_user_page();
    void register_user_handler();
    void activate_user_handler();
    void deactivate_user_handler();
    void view_logs_handler();
    void open_door_handler();
    void large_file_handler(String fileName, String contentType, bool gziped);    
    void sendVariable(String variable, String content);
    void config();
    void config_handler();
};


#endif
