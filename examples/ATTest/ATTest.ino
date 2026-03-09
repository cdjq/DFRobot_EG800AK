/*!
 * @file ATTest.ino
 * @brief This example use the serial port to send AT command to control the AIR720UH
 *        With initialization completed, we can enter AT command to AIR720UH directly
 *        Common AT commands : 
 *        AT+CPIN? : Check SIM card
 *        AT+CSQ   : Check signal quality
 *        Get the AT command table in Resource folder :Luat 4G模块AT命令手册V4.3.3.pdf
 *        Note  : The AT command must end with CRLF
 * @copyright Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [PLELES]<feng.yang@dfrobot.com>
 * @version V1.0
 * @date 2026-3-5
 */
#include "DFRobot_EG800AK.h"
DFRobot_EG800AK EG800AK;    
void setup() {
    /*Please make sure  this Serial baud rate is same as the module's serial baud rate(default is 115200).*/
    Serial.begin(115200);
    while(!Serial);
    Serial.println("EG800AK Init.....");
    if(!EG800AK.begin()){
        Serial.println("EG800AK Init failed");
        while(1);
    }
    Serial.println("For example, if you type AT\\r\\n, OK\\r\\n will be responsed!");
    Serial.println("Enter your AT command :");
}

void loop() {
    while(Serial1.available()){
        Serial.write(Serial1.read());
    }
    while(Serial.available()){
        Serial1.write(Serial.read());
    }
}
