#include <ESP8266WiFi.h>
#include <ESP8266WebServerSecure.h>
#include <DNSServer.h>
#include <FS.h>
#include "model.h"
#include "control.h"
#include "view.h"

NetworkConfig networkConfig;
bool apMode = true;

const byte DNS_PORT = 53;
BearSSL::ESP8266WebServerSecure server(443);
//DNSServer dnsServer;
View view(&server);
String css_file_name;  
String js_file_name;

WiFiEventHandler stationConnectedHandler;
WiFiEventHandler stationDisconnectedHandler;
WiFiEventHandler probeRequestPrintHandler;
WiFiEventHandler probeRequestBlinkHandler;


static const char serverCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDlzCCAn+gAwIBAgIJANHwaCDrWILFMA0GCSqGSIb3DQEBCwUAMGIxCzAJBgNV
BAYTAkJSMQswCQYDVQQIDAJSSjEXMBUGA1UEBwwOUmlvIGRlIEphbmVpcm8xETAP
BgNVBAoMCENFRkVUL1JKMRowGAYDVQQDDBFwb3J0YS5jZWZldC1yai5icjAeFw0x
OTA2MjIxNzQ2MTNaFw0yMDA2MjExNzQ2MTNaMGIxCzAJBgNVBAYTAkJSMQswCQYD
VQQIDAJSSjEXMBUGA1UEBwwOUmlvIGRlIEphbmVpcm8xETAPBgNVBAoMCENFRkVU
L1JKMRowGAYDVQQDDBFwb3J0YS5jZWZldC1yai5icjCCASIwDQYJKoZIhvcNAQEB
BQADggEPADCCAQoCggEBAOQtgqtqtkPIfpP4hKMEeo3lQGC0inUQw1Rpy1IA9Amf
nPn6OcJKy07VxUQu7s3fkZS0yF08gFWdNVTnlPrUMre9GY+sjRE8uX4FI4/H4K1U
2ffjQRIaJDfuh5NS2Qd0ot39TXjtjOaCs9t/Xy5uvWymX4K5pivadixgiYg6Y4JL
vvXHF4Ujwz3Mji7yf+vD2W1OQKnN2gXA4Eok6wZAeweqNEDsIJfAqYTrw8ZD3PyM
GmihStf//Mo7MaXDgU74fZonebjD5U7fUQ8NcqkNOyT8sTrI413uAhgrTSmzHYFc
mC4cqkbei+Cr005gzLWUDx9Hel2T2I2wY4nkZEXNXiUCAwEAAaNQME4wHQYDVR0O
BBYEFNFcgree1993xRf26TJ0b89nNfcCMB8GA1UdIwQYMBaAFNFcgree1993xRf2
6TJ0b89nNfcCMAwGA1UdEwQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEBAJr2hivH
/Snz2mRQ3LKnRQvxLWY6+Qxkh2mNhQQF16GZXTJi3nqi7j2ZCNev8CCAMZoxGGUf
X+21BMQrviE2qq8vEPkVutiCREKm3/EHM79e+dd7nQ2nBg4gRUbIfWoRhsMPGa8g
H/BfHdHe8WABzFbS/LUBsOWBiezdkaYLpLVZZgpQaqkxO9tmtN1cYE9RY+kXVQyg
bVmBGTAlN0F/zttqn4sqUrLtGfD/gqgNiAArqUaOXWYTQPNxhpLqebwaI4kmkhsa
7/hB/kRlIGP5P68aLzj1r8a1YvhVp+cIqfEc0rsZJtiMeuX5y0EeZXUliN3FQKE6
43IjpDENsDsiY2A=
-----END CERTIFICATE-----
)EOF";

static const char serverKey[] PROGMEM = R"EOF(
-----BEGIN PRIVATE KEY-----
MIIEvwIBADANBgkqhkiG9w0BAQEFAASCBKkwggSlAgEAAoIBAQDkLYKrarZDyH6T
+ISjBHqN5UBgtIp1EMNUactSAPQJn5z5+jnCSstO1cVELu7N35GUtMhdPIBVnTVU
55T61DK3vRmPrI0RPLl+BSOPx+CtVNn340ESGiQ37oeTUtkHdKLd/U147YzmgrPb
f18ubr1spl+CuaYr2nYsYImIOmOCS771xxeFI8M9zI4u8n/rw9ltTkCpzdoFwOBK
JOsGQHsHqjRA7CCXwKmE68PGQ9z8jBpooUrX//zKOzGlw4FO+H2aJ3m4w+VO31EP
DXKpDTsk/LE6yONd7gIYK00psx2BXJguHKpG3ovgq9NOYMy1lA8fR3pdk9iNsGOJ
5GRFzV4lAgMBAAECggEANXJvlCVn0hLJi3F/6AtphYk4fi5r7CXkCwbI6Elu4D4x
79/r3ZXVb37lOSZhK3n9yL5b3HMKgINVdbDVFbfUtXMm5J9/X5VAwcMEZG5mM1OX
xtfFay8SYsONoDWcvoFUkPj/R58lDO16oaSBMAgieKuz3ZQM12LRoW1vaah7nI4e
SZr57ra3lyuGSlVIfH/B75YA9/chv2xbuvrAP6f70BzSzEwcM8KI7UbeOFIfWG6E
85bDik55FZMwN8KlBvjuqejQV0mfdIb5GAByqUJwn7qxRrxKZpZumeoobxyJZGW9
6OnRVIELSXq/hPQEo0sO7s4jFMBksC9UMqvlAy3AdQKBgQD23Dq5v32GLGq/BH6d
7X6E0WdlYdkf+c9vASwZEk2JnDNU+BoyAU95xuslnY39aOzvTekD1HVtYcS+2qIK
OQUdhpVztP2m7OoRMyWSps1e/4kWdY7Ay5taA+ioOVQS+zxG0QscPlZoSGQPBXe+
LwOMt4o+zTVHBrZ/D8fVmy6X5wKBgQDsoDTFzgsgPHjZMNSGl+Pdu84IZC//5R5d
xJpgrgVUTzh0K3AMtR1PKetHxZ0DrAFE/AGvVygc1xKtPsi7v8A30nlr7DpVKQA2
UgyebNw4XTqe/8TDlak0QWhPgEXRZDzLyoD/2DsH2msjMgV2qjC/4Hm2MAnUAa5b
x70j1gQoEwKBgQC+bk1vFOVEJ+eAy3rRYXAEDXXxo3XXUOJQA1t9S708tp5yoaGz
iYdBrc2gCtaJ8fM096kVJ0QeyRj5EbeGo03q5PNNewVhS8mU0xvc8YfkDMQ36x/g
YjDJU9KaUv8kVWK+Tmx/sSb+8zkczV79+06FsuCWab77t5X4lrfVdAyRaQKBgQDD
W2UWZg0DeUmgrtLXqDElkHcCsIx8bVILwlReO24E9D68klw48ZKYGBi6saj3LhwC
MzjE5WUlol82Dx60+xOcQrWiZ26lfmtBba4jYebS1g2e1bVCcJ44mUOgV1vNCz+y
VKb6EzBxRF+tuA+Qr+P9ljuGMdT7ZsPn3FkIbBoUZwKBgQDsOXoCaTDlRfd4puPJ
cxqCM+Zww8SlBJkdi0tNDVy703GzUNeROOVbbbCwv/pk/LAKhlgO8QZIeaRyn+HD
2LISVNJoxUwf/SIi0MP3fpXuFE0fvZ6M6D9lvXYNjJOnUh836wS67mIQhBaBKrhI
jbrZ63YkD0V/tYQ7XjekCrsM3g==
-----END PRIVATE KEY-----
)EOF";


int status = WL_IDLE_STATUS;

void setup(void){
  ESP.eraseConfig();
  ESP.wdtDisable();
  ESP.wdtFeed();
  
  pinMode(DOOR_PIN, OUTPUT);
  digitalWrite(DOOR_PIN, HIGH);

  //pinMode(RELAY,OUTPUT);
  //digitalWrite(RELAY, LOW);
  
  Serial.begin(115200);
  SPIFFS.begin();
  
  if(SPIFFS.exists("/network.txt")){
    getNetworkConfig("/network.txt", networkConfig);
    apMode = false;

    // attempt to connect to Wifi network:
    WiFi.begin(networkConfig.ssid, networkConfig.pwd); //begin WiFi connection
    Serial.println("");
  
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(networkConfig.ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    WiFi.hostname(networkConfig.hostname);
    Serial.print("Hostname: ");
    Serial.println(WiFi.hostname());
  }else{
    getNetworkConfig("/config_network.txt", networkConfig);
    apMode = true;
    Serial.println(networkConfig.ssid);
    Serial.println(networkConfig.pwd);
    WiFi.persistent(true);

    
    // Register event handlers.
    // Callback functions will be called as long as these handler objects exist.
    // Call "onStationConnected" each time a station connects
    stationConnectedHandler = WiFi.onSoftAPModeStationConnected(&onStationConnected);
    // Call "onStationDisconnected" each time a station disconnects
    stationDisconnectedHandler = WiFi.onSoftAPModeStationDisconnected(&onStationDisconnected);
    // Call "onProbeRequestPrint" and "onProbeRequestBlink" each time
    // a probe request is received.
    // Former will print MAC address of the station and RSSI to Serial,
    // latter will blink an LED.
    probeRequestPrintHandler = WiFi.onSoftAPModeProbeRequestReceived(&onProbeRequestPrint);
    
    if (WiFi.softAP(networkConfig.ssid, networkConfig.pwd, 1, 1) == false) {
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
    //WiFi.hostname(networkConfig.hostname);
    //Serial.print("Hostname: ");
    //Serial.println(WiFi.hostname());
    //dnsServer.start(DNS_PORT, "", Ip);
  }

  ESP.wdtFeed();

  
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
  server.on("/config", [](){view.config_render();});
  server.on("/config_handler", [](){view.config_handler();});

  ESP.wdtFeed();
  //serve static files gziped
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    if(dir.fileSize() && dir.fileName().endsWith(".gz")) {
      if(dir.fileName().endsWith(".css.gz")){
        css_file_name = dir.fileName().substring(0,dir.fileName().length()-3);
        server.on("/css" + css_file_name, [&css_file_name](){view.large_file_handler(css_file_name, "text/css", true);});
      }
      if(dir.fileName().endsWith(".js.gz")){
        js_file_name = dir.fileName().substring(0,dir.fileName().length()-3);
        server.on("/js" + js_file_name, [&js_file_name](){view.large_file_handler(js_file_name, "application/javascript", true);});
      }
    }
    ESP.wdtFeed();
  }
  //server.on("/css/main.c4772af9e119017605ea.css", [](){view.large_file_handler("/main.css", "text/css", true);});
  //server.on("/js/main.3909667b5c6799ff32be.js", [](){view.large_file_handler("/main.js", "application/javascript", true);});

  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);  

  ESP.wdtFeed();
  server.begin();
  Serial.println("HTTPS server started");
  ESP.wdtFeed();
  
}

void onStationConnected(const WiFiEventSoftAPModeStationConnected& evt) {
  Serial.print("Station connected: ");
  Serial.println(macToString(evt.mac));
  ESP.wdtFeed();
}

void onStationDisconnected(const WiFiEventSoftAPModeStationDisconnected& evt) {
  Serial.print("Station disconnected: ");
  Serial.println(macToString(evt.mac));
  ESP.wdtFeed();
}

void onProbeRequestPrint(const WiFiEventSoftAPModeProbeRequestReceived& evt) {
  Serial.print("Probe request from: ");
  Serial.print(macToString(evt.mac));
  Serial.print(" RSSI: ");
  Serial.println(evt.rssi);
  Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
  ESP.wdtFeed();
}

String macToString(const unsigned char* mac) {
  char buf[20];
  snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

void loop(void){
 server.handleClient();
 ESP.wdtFeed();
 //dnsServer.processNextRequest();
}
