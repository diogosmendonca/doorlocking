#include <ESP8266WiFi.h>
#include <ESP8266WebServerSecure.h>
#include <DNSServer.h>
#include <FS.h>
#include "model.h"
#include "control.h"
#include "view.h"

NetworkConfig config;
bool configMode = true;

//const byte DNS_PORT = 53;
BearSSL::ESP8266WebServerSecure server(443);
//DNSServer dnsServer;
View view(&server);


static const char serverCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)EOF";

static const char serverKey[] PROGMEM = R"EOF(
-----BEGIN PRIVATE KEY-----
-----END PRIVATE KEY-----
)EOF";


int status = WL_IDLE_STATUS;

void setup(void){
  Serial.begin(115200);
  SPIFFS.begin();
  
  if(SPIFFS.exists("/network.txt")){
    getNetworkConfig("/network.txt", config);
    configMode = false;

    // attempt to connect to Wifi network:
    WiFi.begin(config.ssid, config.pwd); //begin WiFi connection
    Serial.println("");
  
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(config.ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    WiFi.hostname(config.hostname);
    Serial.print("Hostname: ");
    Serial.println(WiFi.hostname());
  }else{
    getNetworkConfig("/config_network.txt", config);
    configMode = true;
    if (WiFi.softAP(config.ssid, config.pwd) == false) {
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
    WiFi.hostname(config.hostname);
    Serial.print("Hostname: ");
    Serial.println(WiFi.hostname());
  }

  pinMode(DOOR_PIN, OUTPUT);
  digitalWrite(DOOR_PIN, HIGH);

  //dnsServer.start(DNS_PORT, "doorlocking.app", ip);
  server.setRSACert(new  BearSSL::X509List(serverCert), new BearSSL::PrivateKey(serverKey));
  
  if(configMode){
    server.on("/", [](){view.config();});
    server.on("/config_handler", [](){view.config_handler();});
  }else{
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
    server.on("/css/main.e291f1bce8c43036d951.css", [](){view.large_file_handler("/main.css", "text/css", true);});
    server.on("/js/main.5b23c40006e7f193ce5b.js", [](){view.large_file_handler("/main.js", "application/javascript", true);});
  
    //here the list of headers to be recorded
    const char * headerkeys[] = {"User-Agent", "Cookie"} ;
    size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
    //ask server to track these headers
    server.collectHeaders(headerkeys, headerkeyssize);
  }
  
  server.begin();
  Serial.println("HTTPS server started");
  
}

void loop(void){
 server.handleClient();
 //dnsServer.processNextRequest();
}
