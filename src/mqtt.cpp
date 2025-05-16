#include "mqtt.h"

MQTTManager::MQTTManager(LDR &ldr, ServoMotor &servo, DHTSensor &dht)
    : ldr(ldr), 
    servo(servo), 
    dht(dht), 
    client(espClient) 
    {
    client.setServer(mqttServer, mqttPort);
    client.setCallback([this](char* topic, byte* payload, unsigned int length) {
        this->callback(topic, payload, length);
    });
}

void MQTTManager::initialize() {
    Serial.println("Connecting to mqtt..."); //debugging - ok
    reconnectMQTT();
    publishLDR();
}

void MQTTManager::reconnectMQTT() {
    while (!client.connected()) {
        if (client.connect(clientID)) {
            client.subscribe(ldrTopic); // Subscribe to the LDR topic
            client.subscribe(tsTopic); // Subscribe to the data topic
            client.subscribe(tuTopic); // Subscribe to the data topic
            client.subscribe(minAngleTopic); // Subscribe to the data topic
            client.subscribe(gammaTopic); // Subscribe to the data topic
            client.subscribe(tmedTopic); // Subscribe to the data topic
            Serial.println("Connected to MQTT broker"); // debugging - ok
        } else {
            delay(5000); // Wait 5 seconds before retrying
            Serial.print("Failed to connect to MQTT broker, rc="); // debugging
        } 
    }
}

void MQTTManager::communicate() {
    if (!client.connected()) {
        reconnectMQTT();
    }
    client.loop();
}

void MQTTManager::sendData() {
    now = millis(); // for non blocking delay
    if (now - lastSent >= ldr.avgTime * 60 * 1000) {
      publishLDR(); // publish data every ldr.avgTime minutes
      lastSent = now; // update last sent time
    }
}

void MQTTManager::publishLDR() {
    String payload = String(ldr.getCurrentAverage());

    // debugging
    Serial.println("Publishing LDR data...");
    Serial.println(payload);

    // publish the data to the MQTT broker
    client.publish(ldrTopic, payload.c_str());
}

void MQTTManager::publishData() {    
    String payload;
    payload = "{\"angle\":" + String(servo.getServoAngle()) + 
              ",\"ts\":" + String(ldr.interval) + 
              ",\"tu\":" + String(ldr.avgTime) + 
              ",\"gamma\":" + String(ldr.GAMMA) + 
              ",\"t_med\":" + String(servo.idealTemp) + 
              ",\"theta_offset\":" + String(servo.servoMinAngle) + "}";

    // debugging
    Serial.println("Publishing data...");
    Serial.println(payload);

    // publish the data to the MQTT broker
    client.publish(ldrTopic, payload.c_str());
}




void MQTTManager::callback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (int i = 0; i < length; i++) message += (char)payload[i]; // convert byte array to string
    
    Serial.print("Message arrived in topic "); // debugging
    Serial.print(topic); // debugging
    Serial.println(message); // debugging

    if (String(topic) == tsTopic) {
        ldr.setInterval(message.toFloat());
        Serial.println("Sampling interval set to: " + String(ldr.interval)); // debugging
    } else if (String(topic) == tuTopic) {
        ldr.setAvgTime(message.toFloat());
        Serial.println("Sending interval set to: " + String(ldr.avgTime)); // debugging
    } else if (String(topic) == minAngleTopic) {
        servo.setServoMinAngle(message.toFloat());
        Serial.println("Servo min angle set to: " + String(servo.servoMinAngle)); // debugging
    } else if (String(topic) == gammaTopic) {
        ldr.setGamma(message.toFloat());
        Serial.println("Gamma set to: " + String(ldr.GAMMA)); // debugging
    } else if (String(topic) == tmedTopic) {
        servo.setIdealTemp(message.toFloat());
        Serial.println("Ideal temperature set to: " + String(servo.idealTemp)); // debugging
    }
}