/*!
 * @file setNTP.ino
 * @brief Example of setting NTP time from DFRobot_EG800AK.
 * @copyright  Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [PLELES] (feng.yang@dfrobot.com)
 * @version V1.0
 * @date 2026-3-5
 */
#include "DFRobot_EG800AK.h"
#define NTP_SERVER_IP        "pool.ntp.org"
DFRobot_EG800AK EG800AK;
void setup() {
  /*Please make sure  this Serial baud rate is same as the module's serial baud rate(default is 115200).*/
  Serial.begin(115200);
  while(!Serial);
  EG800AK.begin();
  Serial.println("EG800AK init......");
  if(EG800AK.setBaudRate(eBaud115200)){                       //Set baud rate for communication with module
    Serial.println("Baud rate set successfully");
  }else{
    Serial.println("Failed to set baud rate");
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

  Serial.println("active PDP context....");                     //Active PDP context
  if(EG800AK.activePDPContext()){
    Serial.println("PDP context actived");
  }else{
    Serial.println("PDP context active failed");
    while(1);
  }
  delay(500);

  Serial.println("set NTP retransmission parameters....");      //Set NTP retransmission parameters
  if(EG800AK.setNtpRetransParam(5, 10)){
    Serial.println("set NTP retransmission parameters succeeded");
  }else{
    Serial.println("set NTP retransmission parameters failed");
    while(1);
  }
  delay(500);

  Serial.println("open NTP synchronization....");               //Open NTP synchronization
  if(EG800AK.openNtpSynchronization(NTP_SERVER_IP)){
    Serial.println("NTP synchronization opened");
  }else{
    Serial.println("NTP synchronization open failed");
    while(1);
  }
}

void loop() {
  Serial.println("get NTP time....");
  char *buff = EG800AK.getCLK();                                 //Get the clock
  if(buff != NULL){
    Serial.println(buff);
  }else{
    Serial.println("get NTP time failed");
  }
  delay(2000);
}
