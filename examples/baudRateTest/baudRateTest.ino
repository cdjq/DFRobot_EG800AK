/*!
 * @file baudRateTest.ino
 * @brief This example will set and test the baud rate of the 4G module (EG800AK).
 *        Steps:
 *        1. Auto-detect the module's current baud rate.
 *        2. Change the module and ESP32 baud rate to the new value and check.
 *        3. Enter AT transparent transmission mode in `loop()` for further testing.
 * @copyright  Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [PLELES] (feng.yang@dfrobot.com)
 * @version V1.0
 * @date 2026-03-06
 */

#include "DFRobot_EG800AK.h"
DFRobot_EG800AK EG800AK;
void setup() {
  // Initialize the native serial port for debugging output

  /*Please make sure  this Serial baud rate is same as the module's serial baud rate.*/
  Serial.begin(921600);
  Serial.println("Initializing baud rate test...");


  //Initialize the serial port connected to the EG800AK module with a default value
  EG800AK.begin();
  Serial.println("Setting  baud rate of the module to 921600 bps...");
  if(EG800AK.setBaudRate(eBaud921600)){
    Serial.println("Baud rate set to 921600 bps successfully.");
  }
  else{
    Serial.println("Failed to set baud rate to 921600 bps.");
    while(1);
  
  }

  
  Serial.println("\n==================================");
  Serial.println("--- Starting Baud Rate Test Please input AT command to test.---");
  Serial.println("==================================");

}

void loop() {
    // Step 4: Transparent transmission for AT testing
    while(Serial1.available()){
        Serial.write(Serial1.read());
    }
    while(Serial.available()){
        Serial1.write(Serial.read());
    }
}
