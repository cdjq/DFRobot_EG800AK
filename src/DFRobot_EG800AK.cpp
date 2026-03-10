#include <DFRobot_EG800AK.h>
#include <Update.h>
bool DFRobot_EG800AK::begin(HardwareSerial &serial, uint32_t baud)
{
  if(&serial != &Serial1){
    DBG("Only Serial1 is supported for now. Please use Serial1 or use begin(uint32_t baud) instead.");
    return false;
  }
  DFRobot_SIMcore::begin(serial, baud);
  if(!setBaudRate((eBaudRate_t)baud)){
    DBG("Failed to set baud rate during begin.");
    return false;
  }
  return true;
}

bool DFRobot_EG800AK::begin(uint32_t baud) {
    return begin(Serial1, baud);
}

bool  DFRobot_EG800AK::setBaudRate(eBaudRate_t rate)
{
  uint32_t currentBaud = _checkBaudRate();
  if (currentBaud == 0) {
    DBG("[setBaudRate] Failed to detect current baud rate.");
    return false;
  }
  //DBG("[setBaudRate] Success, current baud rate: " + String(currentBaud));
  
  char cmd[128];
  bool ret = false;
  int  count = 2;

  snprintf(cmd, sizeof(cmd), "AT+IPR=%lu\r\n", (uint32_t)rate);
  
  if(check_send_cmd(cmd,"OK")){
    setRate(rate); 
    return true;
  }

  DBG("[setBaudRate] Failed to set baud rate");
  return false;
}

uint32_t DFRobot_EG800AK::_checkBaudRate(void)
{
  const uint32_t baudsToTry[] = {4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600, 1000000};
  uint32_t currentBaud = 0;
  
  for(int i = 0; i < sizeof(baudsToTry)/sizeof(baudsToTry[0]); i++){
    DFRobot_SIMcore::begin(Serial1, baudsToTry[i]);         // Update hardware baud rate
    // Clear buffer
    while(Serial1.available()) {
        Serial1.read(); 
    }
    // Try sending command
    if(_checkModuleWorking()){
      currentBaud = baudsToTry[i];
      //DBG("[_checkBaudRate] Detected working baud rate: " + String(currentBaud));
      return currentBaud;
    }
  }
  DBG("[_checkBaudRate] Failed to detect a valid baud rate. Module might be unresponsive.");
  return 0;
}



bool DFRobot_EG800AK::_checkModuleWorking(void)
{ 
  if(check_send_cmd("AT\r\n","OK"))
    return true;
    else return false;
}
bool  DFRobot_EG800AK::checkSIMStatus(void)
{
  if(!_checkModuleWorking()) return false;
  bool ret = false;
  int count = 2;
  do{
    delay(300);
    ret = check_send_cmd("AT+CPIN?\r\n","READY");
  }while(!ret && count --);
  return ret;
}

int   DFRobot_EG800AK::checkSignalQuality(void)
{
  int count = 2;
  char *data = NULL;
  String str;
  do{
    send_cmd("AT+CSQ\r\n");
    str = get_String();
    data = strstr(str.c_str(), "+CSQ: ");
  }while(!data && count --);

  if(data != NULL){
    int signalQuality = atoi(data + 6);
    if(signalQuality == 99){
      return 0;
    }
    return signalQuality;
  }else{
    return 0;
  }
}

bool  DFRobot_EG800AK::openNetwork(Protocol ptl,const char *host, int port)
{
  closeNetwork();
  char cmd[128];
  if(ptl == TCP){
    snprintf(cmd, sizeof(cmd), "AT+QIOPEN=1,0,\"TCP\",\"%s\",%d,0,1,1\r\n", host, port);
  }else if(ptl == UDP){
    snprintf(cmd, sizeof(cmd), "AT+QIOPEN=1,0,\"UDP\",\"%s\",%d,0,1,1\r\n", host, port);
  }else{
    DBG("No such mode!");
    return false;
  }
  if(!check_send_cmd(cmd,"+QIOPEN: 0,0")) return false;
  return true;
}

bool  DFRobot_EG800AK::send(String str)
{
  char cmd[128];
  snprintf(cmd, sizeof(cmd), "AT+QISEND=0,%d\r\n", str.length());
  if(check_send_cmd(cmd,">"))
  {
    if(check_send_cmd(str.c_str(),"SEND OK")){
      return true;
    }else{
      return false;
    }
  }
  else{
    return false;
  }
}

bool DFRobot_EG800AK::_mqttIsConnected()
{
  if(check_send_cmd("AT+QMTCONN?\r\n","+QMTCONN: 1,3")) return true;
  else return false;
}

bool DFRobot_EG800AK::mqttConnect(const char* iot_client, const char* iot_username, const char* iot_key)
{
  if(_mqttIsConnected()){
    return true;
  }
  char cmd[128];
  snprintf(cmd, sizeof(cmd), "AT+QMTCONN=1,\"%s\",\"%s\",\"%s\"\r\n", iot_client, iot_username, iot_key);
  if(check_send_cmd(cmd,"+QMTCONN: 1,0,0")){
    return true;
  }else{
    return false;
  }
}

bool DFRobot_EG800AK::mqttDisconnect()
{
  if(_mqttIsConnected()){
    if(check_send_cmd("AT+QMTDISC=1\r\n","+QMTDISC: 1,0")) return true;
    else return false;
  }else{
    return true;
  }
}

bool DFRobot_EG800AK::_mqttIsipOpen(const char *host, int port)
{
  char revBuf[64];
  snprintf(revBuf,sizeof(revBuf),"+QMTOPEN: 1,\"%s\",%d", host, port);
  if(check_send_cmd("AT+QMTOPEN?\r\n",revBuf)){
    return true;
  }else{
    return false;
  }
}
bool DFRobot_EG800AK::mqttIPClose(const char *host, int port)
{
   if(_mqttIsipOpen(host, port)){
     if(check_send_cmd("AT+QMTCLOSE=1\r\n","+QMTCLOSE: 1,0")) return true;
     else return false;
   }else{
     return true;
   }
}

bool DFRobot_EG800AK::mqttIPOpen(const char *host, int port)
{
  if(_mqttIsipOpen(host, port)){
    return true;
  }
  char cmd[128];
  snprintf(cmd, sizeof(cmd), "AT+QMTOPEN=1,\"%s\",%d\r\n", host, port);
  if(check_send_cmd(cmd, "+QMTOPEN: 1,0")){
    return true;
  }else{
    return false;
  }
}

bool DFRobot_EG800AK::mqttPub(const char* topic, eQos_t qos, String str)
{
    char cmd[128];
    int msgID = 0;

    if (qos == eQos0) {
        msgID = 0; 
    } else {
        msgID = mqtt_msg_id++; 
        if (mqtt_msg_id == 0) {
            mqtt_msg_id = 1; 
        }
    }
    snprintf(cmd, sizeof(cmd), "AT+QMTPUB=1,%d,%d,0,\"%s\"\r\n", msgID, (int)qos,topic);
    if(!check_send_cmd(cmd,">")) return false;

    String tempStr = str + "\x1A";
    if (qos == eQos0) {
        if(!check_send_cmd(tempStr.c_str(), "+QMTPUB: 1,0,0")) return false;
    } else {
        char expectResp[32];
        snprintf(expectResp,sizeof(expectResp),"+QMTPUB: 1,%d,0", msgID);
        if(!check_send_cmd(tempStr.c_str(), expectResp)) return false;
    }
    return true;
}

bool DFRobot_EG800AK::mqttSub(const char* topic, eQos_t qos)
{
   char cmd[128];
   int msgID = 0;
   if(qos == eQos0){
     msgID = 1;
   } else { 
      msgID = mqtt_msg_id++;  
      if (mqtt_msg_id == 0) {
          mqtt_msg_id = 1; 
      }
   }

   snprintf(cmd, sizeof(cmd), "AT+QMTSUB=1,%d,\"%s\",%d\r\n", msgID, topic, (int)qos);
   if(qos == eQos0){
      if(!check_send_cmd(cmd,"+QMTSUB: 1,1,0,0")) return false;
   }else { 
      char expectResp[32];
      snprintf(expectResp,sizeof(expectResp),"+QMTSUB: 1,%d,0,%d", msgID, (int)qos);
      if(!check_send_cmd(cmd, expectResp)) return false;
   }
   return true;
}

bool DFRobot_EG800AK::recvMsg(const char* iot_topic, char* buf, int maxlen)
{ 
  String str;
  while(str.length() == 0){
    str = get_String();
    delay(10);
  }
  char *data = strstr(str.c_str(), "+QMTRECV:");
  if(data != NULL){
    data = strstr(data, iot_topic); 
    if(data != NULL){
      while(*data != ',' && *data != '\0') {
        data++;
      }
      if(*data == ',') {
        data++; 
        if(*data == '\"') {
          data++; 
        }
        int i = 0;
        while(*data != '\"' && i < (maxlen - 1)) {
          buf[i++] = *data++;
        }
        buf[i] = '\0'; 
        return true; 
      }
    }
  }
  return false;
}

bool DFRobot_EG800AK::recvMsg(char* buf, int maxlen)
{
   String str;
   while(str.length() == 0){
     str = get_String();
     delay(10);
   }
   char *data = strstr(str.c_str(), "+QIURC: \"recv\",");
   if(data != NULL){
      int i = 0;
      data = strstr(data, "\r\n");
      if(data != NULL){
         data += 2; 
         while(*data != '\0' && *data != '\r' && i < (maxlen - 1)){
            buf[i++] = *data++;
         }
         buf[i] = '\0';
         return true;
      }
   }
   return false;
}

bool DFRobot_EG800AK::mqttUNSUB(const char* topic)
{
   char cmd[128];
   snprintf(cmd, sizeof(cmd), "AT+QMTUNS=1,1,\"%s\"\r\n",topic);
   if(!check_send_cmd(cmd,"+QMTUNS: 1,1,0")) return false;
   return true;
}
bool  DFRobot_EG800AK::httpConnect(const char *Host,int port,eHttpRevMode_t mode)
{
  char cmd[128];
  httpDisconnect();
  delay(5);
  snprintf(cmd, sizeof(cmd), "AT+QIOPEN=1,0,\"TCP\",\"%s\",%d,0,%d\r\n", Host,port,mode);
  if(!check_send_cmd(cmd,"+QIOPEN: 0,0")){
    return false;
  }
  return true;
}

bool  DFRobot_EG800AK::httpDisconnect(void)
{
    if(check_send_cmd("AT+QICLOSE=0\r\n","OK")){
      return true;
    }
    return false;
}

char *DFRobot_EG800AK::httpPost(const char *Host ,int port,const char *jsonBody,eHttpRevMode_t mode)
{
  if(!httpConnect(Host, port, mode)){
    return NULL;
  }

  char httpBuf[512];
  int  bodyLen = strlen(jsonBody);
  int totalLen = snprintf(httpBuf, sizeof(httpBuf),
      "POST /post HTTP/1.1\r\n"
      "Host: %s\r\n"
      "Content-Type: application/json\r\n"
      "Content-Length: %d\r\n"
      "Connection: close\r\n"
      "\r\n"
      "%s",
      Host,
      bodyLen,
      jsonBody);

  if(totalLen <= 0 || totalLen >= (int)sizeof(httpBuf)){
    DBG("[httpPost] request build failed or too long");
    return NULL;
  }

  if(!check_send_cmd("AT+QISEND=0\r\n",">")){
    DBG("[httpPost] wait '>' failed");
    return NULL;
  }


  String payload = String(httpBuf) + "\x1A";
  if(!check_send_cmd(payload.c_str(),"SEND OK")){
    DBG("[httpPost] send failed");
    return NULL;
  }

  // give module a short time to emit URC/response, then capture whole buffer
  delay(200);
  String data = get_String();
  if(data.length() == 0){
    DBG("[httpPost] no response data");
    return NULL;
  }
  
  String expected = String(jsonBody);
  expected.replace("\\", "\\\\");
  expected.replace("\"", "\\\"");
  bool jsonEchoOk = (data.indexOf(expected) >= 0);
  bool http200 = (data.indexOf("HTTP/1.1 200 OK") >= 0);

  if(!http200){
    DBG("[httpPost] HTTP 200 not found");
    return NULL;
  }
  if(!jsonEchoOk){
    DBG("[httpPost] echoed JSON not matched");
    return NULL;
  }
  if(http200 && jsonEchoOk){
    DBG("[httpPost] success");
  }

  return strdup(data.c_str());
}


char *DFRobot_EG800AK::httpGet(const char *Host,int port,const char *path,eHttpRevMode_t mode)
{
  if(!httpConnect(Host, port, mode)){
    DBG("[httpGet] httpConnect failed");
    return NULL;
  }

  char httpBuf[256];
  int totalLen = snprintf(httpBuf, sizeof(httpBuf),
      "GET /%s HTTP/1.1\r\n"
      "Host: %s\r\n"
      "Connection: close\r\n"
      "\r\n",
      path, Host);

  if(totalLen <= 0 || totalLen >= (int)sizeof(httpBuf)){
    DBG("[httpGet] request build failed or too long");
    return NULL;
  }

  if(!check_send_cmd("AT+QISEND=0\r\n",">")){
    DBG("[httpGet] wait '>' failed");
    return NULL;
  }

  String payload = String(httpBuf) + "\x1A";
  if(!check_send_cmd(payload.c_str(),"SEND OK")){
    DBG("[httpGet] SEND OK not received");
    return NULL;
  }

  // give module a short time to emit URC/response, then capture whole buffer
  delay(500); 
  String data = get_String();
  if(data.length() == 0){
    DBG("[httpGet] no response data");
    return NULL;
  }
  
  bool http200 = (data.indexOf("HTTP/1.1 200 OK") >= 0 || data.indexOf("+QIURC: \"recv\",0") >= 0);
  if(!http200){
    DBG("[httpGet] HTTP 200 not found or failed");
    // Depending on your need, you could still return data here, but we return NULL on non-200.
    return NULL;
  }
  
  return strdup(data.c_str());
}


bool  DFRobot_EG800AK::httpOTABegin()
{ 
  int chunkLen = 1024;
  if(!check_send_cmd("AT+QIRD=0,1024\r\n","+QIRD: 1024\r\n")){
    Serial.println("[httpOTABegin] wait +QIRD failed");
    return false;
  }
  // check_send_cmd will match until the end of "+QIRD: 1024\r\n", 
  // so the very next bytes in the Serial1 buffer are "HTTP/1.1 200..."
  char *buf = (char *)malloc(chunkLen);
  if(!buf) {
     DBG("[httpOTABegin] malloc failed");
    return false;
  }

  int revLen = 0;
  uint32_t t = millis();
  while(revLen < chunkLen && (millis() - t < 5000)){ 
    if(Serial1.available() > 0){
      buf[revLen++] = Serial1.read();
      t = millis();
    }
  }
  // wait for a short time to ensure all data is received and discarded
  cleanBuffer();                                         

  // Find \r\n\r\n which indicates the end of HTTP headers
  int headerEnd = -1;
  for(int i = 0; i < revLen - 3; i++){
    if(buf[i] == '\r' && buf[i+1] == '\n' && buf[i+2] == '\r' && buf[i+3] == '\n'){
      headerEnd = i + 4;
      break;
    }
  }

  if(headerEnd == -1){
    DBG("[httpOTABegin] Header end not found in first chunk");
    free(buf);
    return false;
  }

  String headers = "";
  for(int i = 0; i < headerEnd; i++){
    headers += buf[i];
  }
  if(headers.indexOf("HTTP/1.1 200 OK") < 0 ) {
    DBG("[httpOTABegin] HTTP 200 OK not found");
    free(buf);
    return false;
  }
  int index = headers.indexOf("Content-Length:");
  if(index < 0){
    DBG("[httpOTABegin] Content-Length not found");
    free(buf);
    return false;
  }
  index += 15;
  _contentLength = headers.substring(index).toInt();
  DBG("[httpOTABegin] Content-Length: " + String(_contentLength));
  if(_contentLength == 0){
    DBG("[httpOTABegin] Content-Length is 0");
    free(buf);
    return false;
  }

  if (!Update.begin(_contentLength, U_FLASH)) {
    DBG("[OTA] Update.begin failed: " + String(Update.errorString()));
    free(buf);
    return false;
  }
  int payloadLenInChunk = revLen - headerEnd;
  if(payloadLenInChunk > 0){
    size_t written = Update.write((uint8_t *)(buf + headerEnd), payloadLenInChunk);
    if(written != payloadLenInChunk) {
      DBG("[OTA] write failed: " + String(Update.errorString()));
      Update.abort();
      free(buf);
      return false;
    }
    _contentLength -= payloadLenInChunk;
    DBG("[OTA] Written first chunk " + String(payloadLenInChunk) + " bytes, remaining: " + String(_contentLength));
  }

  free(buf);
  return true;
}

int DFRobot_EG800AK::_readBinaryTo(char *buf, int len)
{ 
  char cmd[64], resp[64];
  snprintf(cmd, sizeof(cmd), "AT+QIRD=0,%d\r\n", len);
  snprintf(resp, sizeof(resp), "+QIRD: %d\r\n", len);
  
  if(!check_send_cmd(cmd, resp)){
    DBG("[readBinaryTo] wait +QIRD failed");
    return 0;
  }

  int revLen = 0;
  uint32_t t = millis();
  while(revLen < len && (millis() - t < 5000)){ 
    if(Serial1.available() > 0){
      buf[revLen] = Serial1.read();
      revLen++;
      t = millis(); 
    }
  }

  cleanBuffer();

  return revLen;
}

bool DFRobot_EG800AK::httpOTAWriteToFlash()
{ 
  int remainingLength = _contentLength;
  int retryCount = 0; 

  while (remainingLength > 0) {
    int writeLen = (remainingLength > 1024) ? 1024 : remainingLength;
    char buf[1024];

    int revLen = _readBinaryTo(buf, writeLen);                      // Read binary data into buf
    
    if (revLen == writeLen) {
      size_t written = Update.write((uint8_t *)buf, revLen);       // Write data to flash 
      if (written != revLen) {
        DBG("[OTA] write failed: " + String(Update.errorString()));
        Update.abort();
        return false;
      }
      
      remainingLength -= revLen;
      retryCount = 0; 
      DBG("[OTA] Written " + String(revLen) + " bytes, remaining: " + String(remainingLength));
      
    } else {
      DBG("[OTA] readBinaryTo failed or length mismatch. Retrying...");
      retryCount++;
      if (retryCount >= 3) {
        DBG("[OTA] Too many retries. Aborting.");
        Update.abort();
        return false;
      }
    }
    yield();
  }
  if (Update.end(true)) {
    DBG("[OTA] Update Success! Please restart ESP32.");
    return true;
  } else {
    DBG("[OTA] Update End Failed: " + String(Update.errorString()));
    return false;
  }
}


bool  DFRobot_EG800AK::closeNetwork(void)
{
  if(check_send_cmd("AT+QICLOSE=0\r\n","OK")){
    return true;
  }else{
    return false;
  }
}

char *DFRobot_EG800AK::queryModuleVersionID(void)
{
  send_cmd("AT+CGMR\r\n");
  String str = get_String();
  if(str.length() == 0) return NULL;
  
  char *copy = (char *)malloc(str.length() + 1);
  if(!copy) return NULL;
  strcpy(copy, str.c_str());
  char *line = strtok(copy, "\r\n");
  char *ret = NULL;
  while(line){
    char *s = line;
    while(*s == ' ') s++; 
    if(strlen(s) > 0 && strstr(s, "AT+CGMR") == NULL && strstr(s, "OK") == NULL){
      ret = (char *)malloc(strlen(s) + 1);
      if(ret) strcpy(ret, s);
      break;
    }
    line = strtok(NULL, "\r\n");
  }
  free(copy);
  return ret;
}


char *DFRobot_EG800AK::querySIMCardICCID()
{
  static char buff[30];
  memset(buff,0,sizeof(buff));
  send_cmd("AT+ICCID\r\n");
  String str = get_String();
  char * data = strstr(str.c_str(),"+ICCID:");
  if(data != NULL){
    data += 8; 
    int i = 0;
    while(*data != '\r' && i < 29){
      buff[i++] = *data++;
    }
    buff[i] = '\0';
    return buff;
  }
  return NULL;
}

bool DFRobot_EG800AK::reset(void)
{
  if(check_send_cmd("AT+RESET\r\n","OK")){
    return true;
  }
  return false;
}
bool DFRobot_EG800AK::factoryDataReset(void)
{
  if(check_send_cmd("AT+RSTSET\r\n","OK")){
    return true;
  }
  return false;
}

bool DFRobot_EG800AK::setNtpRetransParam(int cnt, int interval)
{
  char cmd[128];
  if(cnt < 1 || cnt > 10 || interval < 5 || interval > 60){
     DBG("Invalid parameters");
     return false;
  }
  snprintf(cmd, sizeof(cmd), "AT+QCFG=\"ntp\",%d,%d\r\n", cnt, interval);
  if(!check_send_cmd(cmd,"OK")){
    return false;
  }
  return true;
}

bool DFRobot_EG800AK::openNtpSynchronization(const char *host)
{
  char cmd[128];
  snprintf(cmd, sizeof(cmd), "AT+QNTP=1,\"%s\",123,1\r\n", host);
  if(check_send_cmd(cmd, "+QNTP:")){
    return true;
  }
  return false;
}

bool DFRobot_EG800AK::activePDPContext(void)
{
  if(_isPDPContextActive()){
    return true;
  }
  if(check_send_cmd("AT+QIACT=1\r\n","OK")){
    return true;
  }
  return false;
}
bool DFRobot_EG800AK::_isPDPContextActive(void)
{
  if(check_send_cmd("AT+QIACT?\r\n", "+QIACT: 1,1")){
    return true;
  }
  return false;
}


char *DFRobot_EG800AK::getCLK(void) 
{ 
  static char clkBuf[30];
  memset(clkBuf, 0, sizeof(clkBuf));
  char *data = NULL; 
  
  send_cmd("AT+CCLK?\r\n"); 
  String str = get_String(); 
  if((data = strstr(str.c_str(), "+CCLK:")) != NULL) { 
    data += 8;
    int i = 0;  
    while(*data != '\"' && i < 29){  
      clkBuf[i++] = *data++;
    } 
    clkBuf[i] = '\0';  
    return clkBuf;
  } 
  return NULL; 
}

char *DFRobot_EG800AK::getLocalIP(void)
{
  static char ipBuf[20]; 
  memset(ipBuf, 0, sizeof(ipBuf));
  char *buf = NULL;
  send_cmd("AT+CGPADDR\r\n"); 
  String str = get_String();
  char *data = NULL;
  if((data = strstr(str.c_str(), "+CGPADDR:")) != NULL){
    data += 13;
    int i = 0;
    while(*data != '\"' && i < 19){
      ipBuf[i++] = *data++;
    }
    ipBuf[i] = '\0';
    return ipBuf;
  }
  return NULL;
}

