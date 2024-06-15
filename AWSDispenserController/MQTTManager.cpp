#include "MQTTManager.h" 

MQTTManager::MQTTManager(MQTTConfig* config, void (*callback)(const char *, byte *, unsigned int)): config(config), callback(callback) {
  mqttClient = new PubSubClient(wiFiClient);
}

void MQTTManager::setCredentials(){
  wiFiClient.setCACert(config->getCredentials()->getRootCA());
  wiFiClient.setCertificate(config->getCredentials()->getCertificate());
  wiFiClient.setPrivateKey(config->getCredentials()->getPrivateKey());
}

void MQTTManager::setServer(){
  mqttClient->setServer( config->getBrokerHost(), config->getBrokerPort());
}

void MQTTManager::setCallback(){
  mqttClient->setCallback(callback);
}

void MQTTManager::waitForBrokerConnection(){
  String messageConnection = "Connecting to " + String(config->getBrokerHost());
  Serial.print(messageConnection);

  if(mqttClient->connect(config->getClientId())){
    Serial.println("\nConnected!");
    mqttClient->subscribe(config->getSubscribeTopic());
  }
  else{
    Serial.println("Somethin went wrong");
  }
}

MQTTManager::~MQTTManager(){
  delete config;
}

void MQTTManager::connect(){
  setCredentials();
  setServer();
  setCallback();
  waitForBrokerConnection();
}

const char* MQTTManager::getSubscribeTopic(){
  return config->getSubscribeTopic();
}

bool MQTTManager::getStatusWiFiClient(){
  return mqttClient->connected();
}

void MQTTManager::publish(char* outputBuffer){
  mqttClient->publish(config->getPublishTopic(), outputBuffer);
}

void MQTTManager::loop(){
  mqttClient->loop();
}