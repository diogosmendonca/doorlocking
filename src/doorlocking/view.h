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
    void menu();
		void menu_user();
    void menu_admin();
		void login_page();
    void login_handler();
	  void list_users();
    void register_user_page();
    void register_user_handler();
    void activate_user_handler();
    void deactivate_user_handler();
    void view_logs_handler();
};


#endif
