#ifndef MQTTCONFIG_H
#define MQTTCONFIG_H

#include "AmazonCredentials.h"

class MQTTConfig {
private:
  const char* brokerHost;
  int brokerPort;
  const char* clientId;
  const char* subscribeTopic;
  const char* publishTopic;
  AmazonCredentials* credentials;

public:
  MQTTConfig(const char* brokerHost, int brokerPort, const char* clientId, const char* subscribeTopic, const char* publishTopic, AmazonCredentials* credentials);
  ~MQTTConfig();

  const char* getBrokerHost();
  int getBrokerPort();
  const char* getClientId();
  const char* getSubscribeTopic();
  const char* getPublishTopic();
  AmazonCredentials* getCredentials();

};

#endif
 
