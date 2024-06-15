#include "AmazonCredentials.h"

AmazonCredentials::AmazonCredentials (const char* rootCA, const char* certificate, const char* privateKey)
  : rootCA(rootCA), certificate(certificate), privateKey(privateKey) {}

const char* AmazonCredentials::getRootCA() {
  return rootCA;
}

const char* AmazonCredentials::getCertificate() {
  return certificate;
}

const char* AmazonCredentials::getPrivateKey() {
  return privateKey;
}
 
