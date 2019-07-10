#ifndef __VIEW_H__
#define __VIEW_H__

#include <ESP8266WebServerSecure.h>
#include "WString.h"

class View{
	private:
		ESP8266WebServerSecure* server;
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
};


#endif
