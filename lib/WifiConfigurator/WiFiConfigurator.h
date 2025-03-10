#ifndef WiFiConfigurator_h
#define WiFiConfigurator_h

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

class WiFiConfigurator {
public:
  WiFiConfigurator();
  void begin();
  void handleClient();

private:
  WebServer server;
  Preferences preferences;
  
  void setupWiFi();
  void handleRoot();
  String getPage();
};

#endif
