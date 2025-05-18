# Programming Assignment 2
Warushavithana N. D. 220678F

New functions are implemented in 
- LDR class in ```sensors.h``` and ```sensors.cpp``` for LDR input and handeling functions.
- DHT class in ```sensors.h``` and ```sensors.cpp``` for DHT temperature reading functions.
- Sampling rate and sending interval adjusting functions are implemented in sensors class as ```setInterval(float interval)``` and ```setAvgTime(float avgTime)```.
- MQTT implementation is done in mqtt library in mqtt.h and mqtt.cpp. LDR data is sent using the ```publishLDR()``` function with ```ldrtopic```.
- Light intensity visualization is done in nodered dashboard.
- Servo motor initialization and control functions are implemented in servomotor library in ```ServoMotor.h``` and ```servomotor.cpp```.
- Servo angle control is implemented using ```Servo::update```function and ```setServoAngle``` function.
- Sliders are added to the nodered dashboad for controlling servo minimum angle, gamma, and ideal temeperature values.
- The parameter updates are sent to ESP using mqtt out node and ESP sets the parameters using the following functions in ```callback```: 
    - ```ldr.setInterval(message.toFloat())``` 
    - ```ldr.setAvgTime(message.toFloat())```
    - ```servo.setServoMinAngle(message.toFloat())```
    - ```ldr.setGamma(message.toFloat())```
    - ```servo.setIdealTemp(message.toFloat())```             