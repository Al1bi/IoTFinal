#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "MQTTConfig.h"

class MQTTManager{
private:
  PubSubClient* mqttClient;
  WiFiClientSecure wiFiClient;
  MQTTConfig* config;
  void (*callback)(const char *, byte *, unsigned int);

  void setCredentials();
  void setServer();
  void setCallback();
  void waitForBrokerConnection();

public:
  MQTTManager(MQTTConfig* config, void (*callback)(const char *, byte *, unsigned int));
  ~MQTTManager();

  void connect();
  const char* getSubscribeTopic();
  bool getStatusWiFiClient();
  void publish(char* outputBuffer);
  void loop();
};

#endif
 
