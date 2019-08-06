#include <ESP8266WiFi.h>
#include <ESP8266WebServerSecure.h>
#include <DNSServer.h>
#include <FS.h>
#include "view.h"

const char* ssid = "DOOR-NETWORK 3";
const char* password = "";
const byte DNS_PORT = 53;
BearSSL::ESP8266WebServerSecure server(443);
DNSServer dnsServer;
View view(&server);

static const char serverCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)EOF";

static const char serverKey[] PROGMEM = R"EOF(
-----BEGIN PRIVATE KEY-----
-----END PRIVATE KEY-----
)EOF";

void setup(void){
 Serial.begin(115200);
 SPIFFS.begin();
 if (WiFi.softAP(ssid, password) == false) {
  Serial.println("WiFi.softAP - error - exiting");
  return;
 }
 Serial.println("Configuring Wifi AP");
 IPAddress Ip(192, 168, 1, 1);
 IPAddress NMask(255, 255, 255, 0);
 if (WiFi.softAPConfig(Ip, Ip, NMask) == false) {
  Serial.println("WiFi.softAPConfig - error - exiting");
  return;
 }
 dnsServer.start(DNS_PORT, "doorlocking.app", Ip);
 
 server.setRSACert(new  BearSSL::X509List(serverCert), new BearSSL::PrivateKey(serverKey));

 //register the URLs and handlers
 server.on("/", [](){view.home();});
 server.on("/login", [](){view.login_page("");});
 server.on("/login_handler", [](){view.login_handler();});
 server.on("/logout_handler", [](){view.logout_handler();});
 server.on("/menu", [](){view.menu("");});
 server.on("/register_user", [](){view.register_user_page();});
 server.on("/register_user_handler", [](){view.register_user_handler();});
 server.on("/list_users", [](){view.list_users("");});
 server.on("/activate_user_handler", [](){view.activate_user_handler();});
 server.on("/deactivate_user_handler", [](){view.deactivate_user_handler();});
 server.on("/view_logs", [](){view.view_logs_handler();});
 server.on("/open_door", [](){view.open_door_handler();});

 //serve static files
 server.on("/css/main.d251a307bc6092ff3709.css", [](){view.large_file_handler("/main.css.gz", "text/css", true);});
 server.on("/js/main.8a2ee33b328d4d2d59cf.js", [](){view.large_file_handler("/main.js.gz", "application/javascript", true);});
 server.on("/webfonts/fa-solid-900.woff", [](){view.large_file_handler("/fa-solid-900.woff", "font/woff", false);});


 //here the list of headers to be recorded
 const char * headerkeys[] = {"User-Agent", "Cookie"} ;
 size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
 //ask server to track these headers
 server.collectHeaders(headerkeys, headerkeyssize);
 
 server.begin();
 Serial.println("HTTPS server started");
}

void loop(void){
 server.handleClient();
 dnsServer.processNextRequest();
}
