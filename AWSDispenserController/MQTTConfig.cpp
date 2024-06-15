#include "MQTTConfig.h"

MQTTConfig::MQTTConfig(const char* brokerHost, int brokerPort, const char* clientId, const char* subscribeTopic, const char* publishTopic, AmazonCredentials* credentials)
  : brokerHost(brokerHost), brokerPort(brokerPort), clientId(clientId), subscribeTopic(subscribeTopic), publishTopic(publishTopic), credentials(credentials) {
}

MQTTConfig::~MQTTConfig(){
  delete credentials;
}

const char* MQTTConfig::getBrokerHost() {
  return brokerHost;
}

int MQTTConfig::getBrokerPort() {
  return brokerPort;
}

const char* MQTTConfig::getClientId() {
  return clientId;
}

const char* MQTTConfig::getSubscribeTopic() {
  return subscribeTopic;
}

const char* MQTTConfig::getPublishTopic() {
  return publishTopic;
}

AmazonCredentials* MQTTConfig::getCredentials() {
  return credentials;
}
 
