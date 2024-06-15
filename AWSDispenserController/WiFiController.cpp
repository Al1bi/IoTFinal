#include  "WiFiController.h"

WiFiController::WiFiController(const char* ssid, const char* password): ssid(ssid), password(password) {}

WiFiController::WiFiController() {
  wiFiManager.setDebugOutput(false);
}

WiFiController::~WiFiController(){}

void WiFiController::initializeWiFi(){
  String messageNetwork = "Connecting to " + String(ssid);
  Serial.println(messageNetwork);
  WiFi.begin(ssid, password);
}

void WiFiController::setUpCredentials(){
  wiFiManager.resetSettings();
  Serial.println("Starting Web Portal");
  if(!wiFiManager.autoConnect("My Dispenser", "iotgrupo6")){
    Serial.println("Failed to connect");
    ESP.restart();
    delay(1000);
  }
  Serial.println("Connection succesful");
}

void WiFiController::waitForWiFiConnection(){
  while(WiFi.status() != WL_CONNECTED){
    delay(200);
    Serial.print(".");
  }
  Serial.println("\nConnection succesful");
}

void WiFiController::showIpAddress(){
  String messageNetwork = "IP address of the device: " + String(WiFi.localIP());
  Serial.println(messageNetwork);
}

void WiFiController::connect(){
  initializeWiFi();
  waitForWiFiConnection();
  showIpAddress();
}

void WiFiController::autoConnect(){
  setUpCredentials();
  showIpAddress();
}
