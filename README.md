# DFRobot_EG800AK
- [中文版](./README_CN.md)


## Overview
DFRobot_EG800AK is  based on the EG800AK 4G communication module, designed for use with DFRobot EDGE101 and similar mainboards. This library encapsulates the module's AT commands and supports 4G network communication, HTTP/MQTT protocols, OTA updates, NTP time synchronization, and more. It enables users to quickly integrate 4G connectivity into IoT projects with ease.




## Product Link  (https://www.dfrobot.com/)

```text
SKU: DFR0886
```
## Table of Contents
* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)


## Summary
This library provides comprehensive software support for the EG800AK 4G module. With simple APIs, users can achieve network connection, data transmission, remote firmware upgrade, and time synchronization. It is ideal for IoT applications such as remote monitoring, data acquisition, and smart device networking.

## Installation
To use this library, you need to install the Arduino IDE and the EDGE101 SDK. Then, you can import the library into your project.
## Methods
### Arduino C++ Library


```C++

/**
 * @enum Protocol
 * @brief Network protocol
 */
enum Protocol {
    TCP    = 1,
    UDP    = 2,
};

/**
 * @enum eBaudRate_t
 * @brief Baud rate
 */
typedef enum
{
  eBaud4800   = 4800,
  eBaud9600   = 9600,
  eBaud19200  = 19200,
  eBaud38400  = 38400,
  eBaud57600  = 57600,
  eBaud115200 = 115200,
  eBaud230400 = 230400,
  eBaud460800 = 460800,
  eBaud921600 = 921600,
  eBaud1000000 = 1000000,
}eBaudRate_t;

/**
 * @enum eHttpRevMode_t
 * @brief HTTP data receiving mode
 */
typedef enum
{
  eBufferMode      = 0,
  ePassthroughMode = 1,
  eTransparentMode = 2,        
}eHttpRevMode_t;

/**
 * @enum eQos_t
 * @brief Quality of service for application messages 
 */
typedef enum
{
  eQos0 = 0,
  eQos1 = 1,
  eQos2 = 2,
}eQos_t;

 /** 
   * @fn begin
   * @brief Initialize the module
   * @param serial HardwareSerial object
   * @param baud Baud rate
   * @return true if initialization is successful, false otherwise
   */
  bool begin(HardwareSerial &serial = Serial1, uint32_t baud = 115200);
  /**
   * @fn begin
   * @brief Initialize the module with default serial port (Serial1)
   * @param baud Baud rate
   * @return true if initialization is successful, false otherwise
   */

  bool begin(uint32_t baud);
  /**
   * @fn checkSIMStatus
   * @brief Query SIMS status
   * @return true means that the PIN lock is not turned on, false means that the PIN lock is on
   */
  bool checkSIMStatus(void);

  /**
   * @fn checkSignalQuality
   * @brief Check signal quality
   * @return Signal quality 0: less than or equal to -115dBm; 1： -111dBm; 2~30: -109~-53dBm: 31: greater than or equal to -51dBm; 99: Unknown or unpredictable
   */
  int checkSignalQuality(void);

  /**
   * @fn getCLK
   * @brief Get the clock
   * @return module system time
   */
  char *getCLK(void);

  /**
   * @fn getLocalIP
   * @brief Get a local IP address
   * @return IP address
   */
  char *getLocalIP(void);

  /**
   * @fn queryModuleVersionID
   * @brief Check module version ID
   * @return module version ID
   */
  char *queryModuleVersionID(void);
  
  /**
   * @fn querySIMCardICCID
   * @brief Check SIM card ICCID number
   * @return ICCID number
   */
  char *querySIMCardICCID(void);

  /**
   * @fn reset
   * @brief module reboot
   * @return true indicates that the restart command was sent successfully, false indicates that the restart command failed to be sent
   */
  bool reset(void);

  /**
   * @fn factoryDataReset
   * @brief module factory reset and reboot
   * @return true indicates that the command was sent successfully, false indicates that the command was sent unsuccessfully
   */
  bool factoryDataReset(void);

  /**
   * @fn setBaudRate(eBaudRate_t rate)
   * @brief Set the baud rate of the serial communication between the motherboard and the 4G module
   * @param baud : The baud rate type, eBaudrate_t
   * @n            eBaud9600   ：9600bps
   * @n            eBaud19200  ：19200bps
   * @n            eBaud38400  ：38400bps
   * @n            eBaud57600  ：57600bps
   * @n            eBaud115200 ：115200bps
   * @n            eBaud230400 ：230400bps
   * @n            eBaud460800 ：460800bps
   * @n            eBaud921600 ：921600bps
   * @n            eBaud1000000：1000000bps
   * @return true setting succeeded, false setting failed
   */
  bool setBaudRate(eBaudRate_t rate);

  /**
   * @fn openNetwork(Protocol ptl, const char *host, int port)
   * @brief Establish a network connection
   * @param ptl network connection mode: TCPUDP
   * @n     TCP
   * @n     UDP
   * @param host remote server IP address or domain name is acceptable
   * @param port remote service port
   * @return true indicates that the connection is successful, false means that the connection failed
   */
  bool openNetwork(Protocol ptl, const char *host, int port);

  /**
   * @fn closeNetwork
   * @brief Disconnect from the network
   * @return true indicates successful disconnection, false indicates disconnection failure
   */
  bool closeNetwork(void);

   /**
   * @fn httpConnect(const char *Host, int port, eHttpRevMode_t mode )
   * @brief Establish an HTTP connection and set the data receiving mode
   * @param Host domain or IP
   * @param port remote port
   * @param mode data receiving mode, including buffer mode, passthrough mode and URC mode
   * @n       eBufferMode:      Buffer mode
   * @n       ePassthroughMode: Passthrough mode
   * @n       eURCMode:         URC mode
   * @return true indicates that the connection is successful, false means that the connection failed
   */  
  bool httpConnect(const char *Host, int port = 80, eHttpRevMode_t mode = ePassthroughMode);

  /**
   * @fn httpDisconnect
   * @brief Disconnect from HTTP
   * @return true indicates successful disconnection, false indicates disconnection failure
   */
  bool httpDisconnect(void);

  /**
   * @fn httpPost()
   * @brief post http information
   * @param Host HTTP or HTTPS URL
   * @param jsonBody json body string
   * @param port remote port
   * @param mode data receiving mode, including buffer mode, passthrough mode and URC mode
   * @n       eBufferMode:      Buffer mode
   * @n       ePassthroughMode: Passthrough mode
   * @n       eURCMode:         URC mode
   * @return true indicates that the post succeeded, false means that the post failed
   */
  char *httpPost(const char *Host ,int port = 80,const char *jsonBody="",eHttpRevMode_t mode = ePassthroughMode);
  
  /**
   * @fn httpGet()
   * @brief get http information
   * @param Host HTTP or HTTPS URL
   * @param port remote port
   * @param path HTTP request path, default is "/"
   * @param mode data receiving mode, including buffer mode, passthrough mode and URC mode
   * @n       eBufferMode:      Buffer mode
   * @n       ePassthroughMode: Passthrough mode
   * @n       eURCMode:         URC mode
   * @return http information
   */
  char *httpGet(const char *Host, int port = 80,const char *path = "",eHttpRevMode_t mode = ePassthroughMode);

  
  /**
   * @fn httpOTABegin
   * @brief Begin HTTP OTA update
   * @return true indicates successful start, false indicates failure
   */
  bool httpOTABegin(void);

  /**
   * @fn httpOTAWriteToFlash
   * @brief Write the obtained bin file data to flash
   * @return true indicates successful writing, false indicates failure
   */
  bool httpOTAWriteToFlash(void);


  /**
   * @fn send(String str)
   * @brief Send network data
   * @param str data to be sent
   * @return true indicates successful sending, false indicates sending failure
   */
  bool send(String str);

  /**
   * @fn mqttConnect(const char* iot_client, const char* iot_username, const char* iot_key)
   * @brief Connect MQTT
   * @param iot_client MQTT client ID
   * @param iot_username MQTT username
   * @param iot_key MQTT key
   * @return true indicates successful connection, false indicates connection failure
   */
  bool mqttConnect(const char* iot_client, const char* iot_username, const char* iot_key);

  /**
   * @fn mqttDisconnect
   * @brief Disconnect MQTT
   * @return true indicates successful disconnection, false indicates disconnection failure
   */
  bool mqttDisconnect(void);

  /**
   * @fn    mqttIPOpen(const char *host, int port)
   * @brief Establish an IP connection to mqtt
   * @param IP or domain name of the host server
   * @param port number
   * @return true indicates that the connection is successful, false means that the connection failed
   */
  bool mqttIPOpen(const char *host, int port);

  /**
   * @fn    mqttIPClose(const char *host, int port)
   * @brief Disconnect the IP connection from mqtt
   * @return true indicates successful disconnection, false indicates disconnection failure
   */
  bool mqttIPClose(const char *host, int port);
  
   /**
   * @fn    mqttSub(const char* topic, eQos_t qos)
   * @brief Subscribe to topics
   * @param topic subscribed topics
   * @param qos Quality of service for application messages 
   * @n     eQos0：at most once delivery for application message 
   * @n     eQos1：at least once delivery for application message 
   * @n     eQos2：exactly once delivery for application message
   * @return true indicates that the subscription succeeded, false indicates that the subscription failed
   */
  bool mqttSub(const char* topic, eQos_t qos = eQos0);

  /**
   * @fn    mqttPub(const char* topic, eQos_t qos, String str)
   * @brief Publish the message
   * @param topic topic to be published
   * @param qos Quality of service for application messages 
   * @fn     eQos0：at most once delivery for application message 
   * @fn     eQos1：at least once delivery for application message 
   * @fn     eQos2：exactly once delivery for application message
   * @param str Published messages
   * @return true indicates successful publishing, false indicates failure of publishing
   */
  bool mqttPub(const char* topic, eQos_t qos = eQos0,String str = "");

  /**
   * @fn    mqttUNSUB(const char* topic)
   * @brief Unsubscribe from the topic
   * @param topic subscribed topics
   * @return true indicates successful cancellation, false indicates failure of cancellation
   */
  bool mqttUNSUB(const char* topic);

  /**
   * @fn recvMsg(const char* iot_topic, char* buf, int maxlen)
   * @brief Receive MQTT message
   * @param iot_topic MQTT topic
   * @param buf buffer to store the received message
   * @param maxlen maximum length of the buffer
   * @return true indicates successful receiving, false indicates failure of receiving
   */
  bool recvMsg(const char* iot_topic, char* buf, int maxlen);

  /**
   * @fn recvMsg(char* buf, int maxlen)
   * @brief Receive HTTP message
   * @param buf buffer to store the received message
   * @param maxlen maximum length of the buffer
   * @return true indicates successful receiving, false indicates failure of receiving
   */
  bool recvMsg(char* buf, int maxlen);

  /**
   * @fn setNtpRetransParam(int cnt, int interval)
   * @brief Set the NTP retransmission parameters
   * @param cnt number of retransmissions
   * @param interval retransmission interval
   * @return true indicates successful setting, false indicates failure of setting
   */
  bool setNtpRetransParam(int cnt = 5, int interval = 10);

  /**
   * @fn openNtpSynchronization(const char *host)
   * @brief Open NTP synchronization
   * @param host NTP server address
   * @return true indicates successful opening, false indicates failure of opening
   */
  bool openNtpSynchronization(const char *host);
  
  /**
   * @fn activePDPContext
   * @brief Activate PDP context
   * @return true indicates successful activation, false indicates activation failure
   */
  bool activePDPContext(void);

```

##  Compatibility
| Platform | Work Well | Work Wrong | Untested | Remarks |
|----------|-----------|------------|----------|---------|
| DFRobot_EDGE101 |  |√| | |


## History
- 2026/03/09 - Version 1.0.0 released.

## Credits
Written by PLELES (feng.yang@dfrobot.com),2026.(Welcome to our [website](https://www.dfrobot.com/))