#include <ESP8266WiFi.h>
#include <ESP8266WebServerSecure.h>
#include <DNSServer.h>
#include "view.h"

const char* ssid = "DOOR-NETWORK";
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
 dnsServer.start(DNS_PORT, "porta.cefet-rj.br", Ip);
 
 server.setRSACert(new  BearSSL::X509List(serverCert), new BearSSL::PrivateKey(serverKey));
 server.on("/", [](){view.home();});
 server.begin();
 Serial.println("HTTPS server started");
}

void loop(void){
 server.handleClient();
 dnsServer.processNextRequest();
}
