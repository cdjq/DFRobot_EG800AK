/*!
 * @file basic.ino
 * @brief Example of basic operations from DFRobot_EG800AK.
 * @copyright  Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [PLELES] (feng.yang@dfrobot.com)
 * @version V1.0
 * @date 2026-3-5
 * @url https://github.com/DFRobot/DFRobot_FRN20
 */
#include "DFRobot_EG800AK.h"
DFRobot_EG800AK EG800AK;
char *testBuf;
void setup() {
 /*Please make sure  this Serial baud rate is same as the module's serial baud rate(default is 115200).*/
  Serial.begin(921600);
  while(!Serial);
  Serial.println("EG800AK Init.....");
  if(!EG800AK.begin(921600)){
      Serial.println("EG800AK Init failed");
      while(1);
  }
  Serial.println("Check SIM card......");
  if(EG800AK.checkSIMStatus()){                                             //Check SIM card
      Serial.println("SIM card READY");
  }else{
      Serial.println("SIM card ERROR, Check if you have insert SIM card and restart EG800AK");
      while(1);
  }

   Serial.println("Get signal quality......");
   delay(500);
   Serial.print("signalStrength =");
   int signalStrength;
   if((signalStrength = EG800AK.checkSignalQuality()) != 0){                 //Check signal quality
      Serial.println(signalStrength);
   }else{
    Serial.println(" NO signalStrength");  
  }
  delay(500);
 
  Serial.print("Local IP:");                                                  //Get local IP
  if((testBuf = EG800AK.getLocalIP()) != NULL ){
      Serial.println(testBuf);
  }else{
      Serial.println("failed to read Local IP");
  }
  delay(500);

  Serial.print("CLK:");                                                       //Get CLK
  if((testBuf = EG800AK.getCLK()) != NULL){
    Serial.println(testBuf);
  }else{
    Serial.println("failed to read CLK");
  }
  delay(500);
  
  Serial.print("ICCID:");                                                      //Get ICCID
  if((testBuf = EG800AK.querySIMCardICCID()) != NULL){
    Serial.println(testBuf);
  }else{
    Serial.println("failed to read ICCID");
  }
  delay(500);


  Serial.print("ModouleVersionID:");                                          //Get ModuleVersionID  
  if((testBuf = EG800AK.queryModuleVersionID()) != NULL){
    Serial.println(testBuf);
  }else{
    Serial.println("failed to read ModuleVersionID");
  }
  delay(500);

}

void loop() {
  // put your main code here, to run repeatedly:

}
