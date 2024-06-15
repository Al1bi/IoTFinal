#include <ArduinoJson.h>

#include "WiFiController.h"
#include "AmazonCredentials.h"
#include "MQTTConfig.h"
#include "MQTTManager.h"
#include "Dispenser.h"

const char * MQTT_BROKER_HOST = "a2dmh18tmfqz8b-ats.iot.us-east-2.amazonaws.com";
const int MQTT_BROKER_PORT = 8883;

const char * MQTT_CLIENT_ID = "IoT-DISPENSER";               
const char * UPDATE_TOPIC = "$aws/things/dispensador/shadow/update";              
const char * UPDATE_DELTA_TOPIC = "$aws/things/dispensador/shadow/update/delta";  

const char ROOT_CA1[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

const char CERTIFICATE[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUfxZH9oMPXbGaQJDknH2CGqZu0dYwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI0MDYwMzE0MDUy
M1oXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAL2dDZ6u61UKICp/9ElN
4o8LT3LbvN/MqWT7OB1D45D8M+yFf92BCFxXddDl699fYWNxzcuPNEv6zCPn7CEr
Ha7/RdS0KCM++IwuvFIJUZcPl//nT8G+g38X7DVH8+H2Esa7Ll6xMqTozuF7RB+T
pOkJBJro1pn1dJ6IoOIblrWUbPYsqj+YPTn44Pe2CS3oiYqCL+iL96CihGZqrqqT
cQobDzLo1D6oIP08s4mPZBZyR5bWCz5cEoJxMXCtaD2//TQX5zasyUNPO+9RLweq
RgijP8XOCQ22cbr9Uooe0KWI2o3j2/avwdHjMwxaojKyWroImj23HRD3tFihuX/K
I+cCAwEAAaNgMF4wHwYDVR0jBBgwFoAUqLkh+E30P6wciG3abhpquXSztgEwHQYD
VR0OBBYEFNTlL7OIWOH4fib3OAtGB5Jb8+hMMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQB7E9k5b77Y7MByMbj22TJlO4T8
FjMuycXfvhJOf3Yd9Zap5K5wf+PSrkyFkh0uwjsNKpEdph4yE9IlsOHVz+rjWQJB
aG2G/afTw0VZzDzIp5F9z5wWTYL7ZVsf/f+ZULoWO9+UWyEeRQVdJHnthR+usGmz
v5hBUzFKQfCGHhuVm3X0uuS5CJSVdcNTFFulD+drkTEa5iZew5szhBYQnVuOlsiu
E6zaYGCXyZU8AzZ4+JE242/EfLOTf2JeBG0OEeVtGAJg+TYGPK6S3FDtrrCS/p4a
1vyAvl5PwktSLuj6h0/v9f6pu6aipwmokNGFInD+BIg6MY8YpDZtgk+cDfbl
-----END CERTIFICATE-----
)KEY";

const char PRIVATE_KEY[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEAvZ0Nnq7rVQogKn/0SU3ijwtPctu838ypZPs4HUPjkPwz7IV/
3YEIXFd10OXr319hY3HNy480S/rMI+fsISsdrv9F1LQoIz74jC68UglRlw+X/+dP
wb6DfxfsNUfz4fYSxrsuXrEypOjO4XtEH5Ok6QkEmujWmfV0noig4huWtZRs9iyq
P5g9Ofjg97YJLeiJioIv6Iv3oKKEZmquqpNxChsPMujUPqgg/TyziY9kFnJHltYL
PlwSgnExcK1oPb/9NBfnNqzJQ08771EvB6pGCKM/xc4JDbZxuv1Sih7QpYjajePb
9q/B0eMzDFqiMrJaugiaPbcdEPe0WKG5f8oj5wIDAQABAoIBAQCnIJgRpvHseclS
TIfgWNL57Ckx/vaieH1ScY/LU7xwXavSrPTAI2xgIALLhuz5bNOlWHjhFC8vTTbD
6zcZISy/w2ckjUrXjjh7LvJ0iJwuvuLl/1cwWSEEU+yyJeKT51Yzbmb3tc4CxuUa
n5CzGGsBsB4sAFCvUUdiOOE9ir2y+0S8lccJPDeXj+K10IHmNvyznnYPH5GvQ9IB
n5WSzWH4t7j+2ToySQJzNOOwPF/79/Q7oWmAxFZZtHnxRBAZKr7y/Qq7HkScjCdO
Jq12QsO/bkmpy2Pp3vfM7u1Pm8ebigNIbbZysgKoJjhKsjVgiuEIE0LArvaDJjNk
/Qype8IJAoGBAOV8EhR+feENg99e3niy56g8kJ0Fgp4Zb1b2eVCmZn6YMBgujY/e
/Kdul6eIiri+FBsPiCA4FAsVchJEbae4LrNOpg0qsIgezioO1jjoLtmCmDtuCppD
yCkF0voHbEIi08ftBYuVEGgkcqdd7DXxV1uzaOFiu17yNbLTDS2CY4pFAoGBANOF
ogsG9sT6BYmk6ufiefb39mooEAriDrGySbaa8sSeyUWHAOUH/b4ZAcFPi0vIJ5Lq
xanQosA2OI3rQeU4VOoXhga8Yd3nO1rJYvTTIqfr5dRpwGukHqCCivGJVfn6pTrk
r/+SLdoFLzOz4VMTQDZsRg4mK1R1N9W7GJnpe447AoGBAKP3VEvEXVix+WleNYL0
AUVZkNJAn6Uq8zyleuS/IDiPsL12dbs4TjmrcTxI+SUq2t7C6DZtGxUYS5r1vYu6
s9N1/Hsn51469H9kfnfGncLX5sF/zJ+QkrGM8cOoDhfrHnqYxf+HkGN4t1sSZrXc
LNjeha+UheA/G1McLSw5H7opAoGBALBC52lxACncfsjYmLdqzl+y+bA0eknXXo9N
FogITszx1J8PhPtV0Wibxg6kjls0C2YuJVEA/t5VUINSy7KLYpc/yvvJ93PGwHtC
3idN2c88kGSVSzdnp+3xWn80BmUtLIsLDm5WDcdkEpVvDBECNbwkNayMugbDNyfv
pOx81+0JAoGAWJtnRhka65MVR0dsiA9X+E1RJnSZhTkMmBg+TvfwPrugxZKBm0Na
Boo+2c9KWMd4g2V4fmdTW0wzCP2Rlh0Vxt8rTzDiNFpVZ+zK0gbvFhyWzAhr1nmT
NNGQXi/scxWTPR7zY+a11hzD/VTOG3zSOiloW8Td4Z5sDjq13ioGhqo=
-----END RSA PRIVATE KEY-----
)KEY";

WiFiController* wiFiController;
AmazonCredentials* amazonCredentials;
MQTTConfig* mqttConfig;
MQTTManager* mqttManager;

StaticJsonDocument<JSON_OBJECT_SIZE(64)> inputDoc;
StaticJsonDocument<JSON_OBJECT_SIZE(4)> outputDoc;

WeightSensor* cupWeightSensor = new WeightSensor(27, 26);
WeightSensor* tankWeightSensor = new WeightSensor(27, 26);
WaterPump* waterPump = new WaterPump(13, 12);
Led* led = new Led(25);
Dispenser* dispenser = new Dispenser(cupWeightSensor, tankWeightSensor, waterPump, led);

char outputBuffer[128];

bool dispense = false;
float glassWeight = 20;
float waterTarget = 0;

void callback(const char * topic, byte * payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) message += String((char) payload[i]);
  
  if (String(topic) == UPDATE_DELTA_TOPIC) {
    String messageTopic = "Message from topic " + String(topic) + ":" + message;
    Serial.println(messageTopic);

    DeserializationError err = deserializeJson(inputDoc, payload);
    if (!err) {

      String desiredState = inputDoc["state"]["pumpState"].as<String>();
      float desiredWater = inputDoc["state"]["waterTarget"].as<float>();

      if(strcmp(desiredState.c_str(), "dispense") == 0){
        waterTarget = desiredWater;
        dispense = true;
        dispenser->turnOnWaterPump();
        dispenser->turnOnLed();
        float weightInTank = dispenser->senseWeightInTheTank(10);
        reportState("dispense", desiredWater, weightInTank, "start");
      }else if(strcmp(desiredState.c_str(), "idle") == 0){
        waterTarget = 0;
        dispense = false;
        dispenser->turnOffWaterPump();
        dispenser->turnOffLed();
        float weightInTank = dispenser->senseWeightInTheTank(10);
        reportState("idle", 0, weightInTank, "reset");
      }
    }
  }
}

void reportState(const char* pumpState, const int waterTarget, const float remainingWater, const char* message){
  outputDoc["state"]["reported"]["pumpState"] = pumpState;
  outputDoc["state"]["reported"]["waterTarget"] = waterTarget;
  outputDoc["state"]["reported"]["remainingWater"] = remainingWater;
  outputDoc["state"]["reported"]["message"] = message;
  serializeJson(outputDoc, outputBuffer);
  mqttManager->publish(outputBuffer);
}

void setup() {
  Serial.begin(115200);

  wiFiController = new WiFiController();
  wiFiController->autoConnect();

  amazonCredentials = new AmazonCredentials(ROOT_CA1, CERTIFICATE, PRIVATE_KEY);
  mqttConfig = new MQTTConfig(MQTT_BROKER_HOST, MQTT_BROKER_PORT, MQTT_CLIENT_ID, UPDATE_DELTA_TOPIC, UPDATE_TOPIC, amazonCredentials);
  mqttManager = new MQTTManager(mqttConfig, callback);
  mqttManager->connect();
  
  float weightInTank = dispenser->senseWeightInTheTank(10);
  reportState("idle", 0, weightInTank, "first report");
}

unsigned long previousMillis = 0;

void loop() {

  if(mqttManager->getStatusWiFiClient()){

    unsigned long now = millis();

    if (now - previousMillis >= 100) {
      previousMillis = now;

      float weight = dispenser->senseWeightOnTheTray(5); 

      if(dispense && weight < 20){
        Serial.println("There is no glass in the dispenser");
        float weightInTank = dispenser->senseWeightInTheTank(10);
        reportState("dispense", waterTarget, weightInTank, "no glass");
      }

      float mlInTheGlass = weight - glassWeight;
      if(dispense) Serial.println(mlInTheGlass);

      if(dispense && mlInTheGlass >= waterTarget){
        Serial.println("Dispense successful");
        float weightInTank = dispenser->senseWeightInTheTank(10);
        reportState("dispense", waterTarget, weightInTank, "success");
      }
    }

    mqttManager->loop();

  } else {
    Serial.println("Something went wrong!");
    delay(2000);
    
    Serial.println("Restarting...");
    ESP.restart();
  }  

}
