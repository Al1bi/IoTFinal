#ifndef WiFiController_H
#define WiFiController_H

#include <WiFi.h>
#include <WiFiManager.h>

class WiFiController {
private:  
  const char* ssid;
  const char* password;
  WiFiManager wiFiManager;

  void initializeWiFi();
  void setUpCredentials();
  void waitForWiFiConnection();
  void showIpAddress();

public:
  WiFiController(const char* _ssid, const char* _password);
  WiFiController();
  ~WiFiController();
  void connect(); 
  void autoConnect();
};

#endif
