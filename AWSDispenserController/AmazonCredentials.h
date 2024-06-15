 #ifndef AMAZONCREDENTIALS_H
#define AMAZONCREDENTIALS_H


class AmazonCredentials {
private:
  const char* rootCA;
  const char* certificate;
  const char* privateKey;

public:
  AmazonCredentials (const char* rootCA, const char* certificate, const char* privateKey);

  const char* getRootCA();
  const char* getCertificate();
  const char* getPrivateKey();

};

#endif
