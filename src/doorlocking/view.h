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
		void login();
	
};


#endif
