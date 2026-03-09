/*!
 * @file 4G_OTA.ino
 * @brief Example of OTA update by Http from DFRobot_EG800AK.
 *       This module only supports HTTP access and does not support HTTPS.
         HTTP is implemented via TCP simulation.
         A free Alibaba Cloud HTTP server is applied for,
         and the .bin file is uploaded to it for use.
         Valid access address: http://120.26.43.190/OTATest.ino.bin
 * @copyright  Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [PLELES] (feng.yang@dfrobot.com)
 * @version V1.0
 * @date 2026-3-5
 */

#include <DFRobot_EG800AK.h>
#define OTA_HOST_NAME  "120.26.43.190"
#define OTA_PORT        80
#define OTA_FILE_NAME  "OTATest.ino.bin"


DFRobot_EG800AK    EG800AK;

void setup() {
    int signalStrength;
    Serial.begin(115200);
    while(!Serial);
    Serial1.begin(115200);
    EG800AK.begin(Serial1);

    Serial.println("Check SIM card......");
    if(EG800AK.checkSIMStatus()){                                           //Check SIM card
        Serial.println("SIM card READY");
    }else{
        Serial.println("SIM card ERROR, Check SIM and restart module");
        while(1);
    }

    Serial.println("Get signal quality......");
    delay(500);
    Serial.print("signalStrength =");
    if((signalStrength = EG800AK.checkSignalQuality()) != 0){                 //Check signal quality
        Serial.println(signalStrength);
    }else{
        Serial.println(" NO signalStrength");  
    }
    delay(500);

 
    Serial.println("Start http Get file...");
    if(EG800AK.httpGet(OTA_HOST_NAME, OTA_PORT, OTA_FILE_NAME,eBufferMode)){     // Get bin file from server 
        Serial.println("http Get success");
    }else{
        Serial.println("http Get error");
        while(1);
    }

    Serial.println("Begin http OTA...");                                           //   Start OTA
    if(EG800AK.httpOTABegin()){
        Serial.println("http OTA begin success");
    }else{
        Serial.println("http OTA begin error");
        while(1);
    }

    Serial.println("http OTA process...");                                           //    OTA process
    if(EG800AK.httpOTAWriteToFlash()){
        Serial.println("http OTA write success");
    }else{
        Serial.println("http OTA write error");
    }
}


void loop() {
}
