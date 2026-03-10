/*!
 * @file httpTest.ino
 * @brief Example of HTTP connection over a HTTP request.
 * @copyright  Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [PLELES] (feng.yang@dfrobot.com)
 * @version V1.0
 * @date 2026-3-5
 *
 */
#include <DFRobot_EG800AK.h>

#define HOST      "httpbin.org"
#define PORT       80
#define JSON_DATA "{\"a\":1,\"b\":2}"

DFRobot_EG800AK EG800AK;

void setup(){
    int signalStrength;
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
        while(1);
    }
    delay(500);

    Serial.print(" --------POST to host----------: ");       //Post data to server, and get response
    Serial.println(HOST);
    char *response=EG800AK.httpPost(HOST, PORT, JSON_DATA);   
    if(response){
        Serial.println("Post successed");
    }else{
        Serial.println("Fail to post");
    }
    Serial.println("Response:");
    Serial.println(response);


    Serial.print(" --------GET from host----------: ");     //Get data from server, and get response
    Serial.println(HOST);
    response=EG800AK.httpGet(HOST,PORT);
    if(response){
        Serial.println("Get successed");

    }else{
        Serial.println("Fail to get");
    }
    Serial.println("Response:");
    Serial.println(response);

    Serial.println("Disconnecting.............");           //Disconnect Http connection
    if(EG800AK.httpDisconnect()){
        Serial.println("Disconnected");
    }else{
        Serial.println("Failed to disconnect");   
    }                        
}

void loop() {
}
