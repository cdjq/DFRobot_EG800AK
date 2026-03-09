/*!
 * @file tcpTest.ino
 * @brief Example of TCP communication using DFRobot_EG800AK.
 *        http://tcp.xnkiot.com/ is applied for testing, and the server will send back the data received from client.
 *        You can also build your own TCP server for testing, and change the serverIP and serverPort accordingly.
 * @copyright  Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [PLELES] (feng.yang@dfrobot.com)
 * @version V1.0
 * @date 2026-3-5
 */
#include "DFRobot_EG800AK.h"
#define serverIP        "8.135.10.183"
#define serverPort       35884
DFRobot_EG800AK EG800AK;
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
void setup() {
  /*Please make sure  this Serial baud rate is same as the module's serial baud rate(default is 115200).*/
  Serial.begin(115200);
  while(!Serial);
 
  Serial.println("EG800AK Init....");
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

  Serial.println("Connecting to network......");
  if(EG800AK.openNetwork(TCP, serverIP, serverPort)){           //Start up TCP connection
     Serial.println("Network connected");
  }else{
    Serial.println("Failed to connect network");
    while(1);
  }
}

void loop() {
  Serial.println();
  Serial.println(">>> Please input data to send:");
  String data = readSerial();
  Serial.println("Send data :");
  Serial.println(data);
  if(EG800AK.send(data)){                                      //Send data by TCP_Server
    Serial.println("Send data success");
  }else{
    Serial.println("Failed to send data");
    return;
  }
  delay(500);

  Serial.println("Waiting for response... (Blocking)");
  char recvBuf[128] = {0};
  while(1){
      if(EG800AK.recvMsg(recvBuf, sizeof(recvBuf))){
        Serial.print(">>> Received Target message: ");
        Serial.println(recvBuf);
        break; 
      }
      delay(10);
  }
  delay(2000);
}