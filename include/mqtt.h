#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <mqtt.h>

#include "Sensors.h"
#include "ServoMotor.h"

class MQTTManager {
public:
    MQTTManager(LDR &ldr, ServoMotor &servo, DHTSensor &dht);
    void initialize();
    void communicate();
    void sendData();
    bool isConnected();

private:
    WiFiClient espClient;
    PubSubClient client;
    LDR& ldr;
    ServoMotor& servo;
    DHTSensor& dht;
    
    // MQTT arguments and topics
    const char* mqttServer = "broker.hivemq.com";
    const int mqttPort = 1883;

    const char* clientID = "MediboxClient";
    
    const char* ldrTopic = "medibox/ldr";
    const char* tsTopic = "medibox/sampling";
    const char* tuTopic = "medibox/sending";
    const char* minAngleTopic = "medibox/minAngle";
    const char* gammaTopic = "medibox/gamma";
    const char* tmedTopic = "medibox/idealTemp";


    unsigned long lastSent = 0;
    unsigned long now = 0;

    // void connectWiFi();
    void reconnectMQTT();
    void callback(char* topic, byte* payload, unsigned int length);
    void publishData();
    void publishLDR();

    // void parseConfig(const JsonDocument& doc);
};


#endif