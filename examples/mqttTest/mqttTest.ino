/*!
 * @file mqttTest.ino
 * @brief Example of MQTT connection and communication using DFRobot_EG800AK.
 *        Login website (http://iot.dfrobot.com.cn) to register an account ,fill the following information based on your account
 *        ClientID : IOT
 *        Username : iGVh0HDvR(replace with your own username)
 *        Password : iGVh0NDDRz(replace with your own password)
 * @copyright  Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [PLELES] (feng.yang@dfrobot.com)
 * @version V1.0
 * @date 2026-3-5
 */
#include "DFRobot_EG800AK.h"
#define serverIP        "iot.dfrobot.com.cn"
#define IOT_PORT         1883
#define IOT_CLIENT      "IOT"
#define IOT_USERNAME    "iGVh0HDvR"
#define IOT_KEY         "iGVh0NDDRz"
#define IOT_TOPIC       "PxN0AHDvR"
String readSerial(){
    int i = 0;
    String result;
    while(1){
        while(Serial.available() > 0){
            char inChar = Serial.read();
            if(inChar == '\n'){
                result += inChar;
                while(Serial.read() >= 0);
                return result;
            }
            if(i == 50){
                Serial.println("The data is too long");
                result += '\0';
                while(Serial.read() >= 0);
                return result;
            }
            result += inChar;
            i++;
        }
    }
}
DFRobot_EG800AK EG800AK;
void setup() {
  /*Please make sure  this Serial baud rate is same as the module's serial baud rate(default is 115200).*/
  Serial.begin(115200);
  while(!Serial);
  if(EG800AK.begin()){
    Serial.println("EG800AK init success");
  }else{
    Serial.println("EG800AK init failed");
    while(1);
  }
 
  Serial.println("Check SIM card......");
  if(EG800AK.checkSIMStatus()){                                //Check SIM card
      Serial.println("SIM card READY");
  }else{
      Serial.println("SIM card ERROR, Check if you have insert SIM card and restart EG800AK");
      while(1);
  }
  
  
  Serial.println("Get signal quality......");
  delay(500);
  Serial.print("signalStrength =");
  int signalStrength;
  if((signalStrength = EG800AK.checkSignalQuality()) != 0){     //Check signal quality
    Serial.println(signalStrength);
  }else{
    Serial.println(" NO signalStrength");
    while(1);
  }
  delay(500);

  Serial.println("IP starting......");
  if(EG800AK.mqttIPOpen(serverIP, IOT_PORT)){                  //Open IP connection
    Serial.println("IP start OK");
  }else{
    Serial.println("Failed to start IP");
    EG800AK.mqttIPClose(serverIP, IOT_PORT);
    while(1);
  }
  delay(500);

  Serial.println("Connecting to MQTT server......");          //Connect to MQTT server
  if(EG800AK.mqttConnect(IOT_CLIENT, IOT_USERNAME, IOT_KEY)){
    Serial.println("MQTT server connected");
  }else{
    Serial.println("MQTT server connection failed");
    while(1);
  }
}

void loop() {
    Serial.println(">>> Please input data to publish:");        //publish data to topic
    String data = readSerial();                                 //wait for serial input
    if(EG800AK.mqttPub(IOT_TOPIC, eQos2, data.c_str())){
      Serial.println("Publish to topic success");
    }else{
      Serial.println("Publish to topic failed");
    }
    delay(2000);

    Serial.println("Subscribing to topic...");                   //Subscribe to topic
    if(EG800AK.mqttSub(IOT_TOPIC, eQos2)){
      Serial.println("Subscribe to topic success");
    }else{
      Serial.println("Subscribe to topic failed");
    }
    
    Serial.println("Waiting for Subtopic message... (Blocking)");
    char recvBuf[128] = {0};
    while (1) {
      if(EG800AK.recvMsg(IOT_TOPIC, recvBuf, sizeof(recvBuf))){
        Serial.print(">>> Received Target message: ");
        Serial.println(recvBuf);
        break; 
      }
      delay(10);
    }
    delay(2000);
}