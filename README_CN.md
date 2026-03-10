# DFRobot_EG800AK
- [English Version](./README.md)

## 概述
DFRobot_EG800AK 是一款 EG800AK 4G 通信模块，适用于 DFRobot EDGE101 主控板。该库封装了模块的 AT 指令，支持 4G 网络通信、HTTP/MQTT 协议、OTA 升级、NTP 校时等多种功能，方便用户快速集成 4G 通信能力到物联网项目中。

![正面svg效果图]
## 产品链接 (https://www.dfrobot.com.cn/)

```text
SKU: DFR0886
```

## 目录

  * [概要](#概要)
  * [安装](#安装)
  * [方法](#方法)
  * [兼容性](#兼容性)
  * [历史](#历史)
  * [创作者](#创作者)


## 概要
本库为 EG800AK 4G 模块提供了完整的软件支持，用户可通过简单的 API 实现网络连接、数据收发、远程升级、时间同步等功能。适用于远程监控、数据采集、智能设备联网等多种物联网应用场景。


## 安装
下载完EDGE101的SDK后，打开4G_MODULE例程即可

### Arduino 库安装

## 方法
### Arduino C++ 库

```C++
/**
 * @enum Protocol
 * @brief 网络协议类型
 */
enum Protocol {
    TCP    = 1,   // TCP 协议
    UDP    = 2,   // UDP 协议
};

/**
 * @enum eBaudRate_t
 * @brief 串口波特率
 */
typedef enum
{
  eBaud4800   = 4800,    // 4800bps
  eBaud9600   = 9600,   // 9600bps
  eBaud19200  = 19200,  // 19200bps
  eBaud38400  = 38400,  // 38400bps
  eBaud57600  = 57600,  // 57600bps
  eBaud115200 = 115200, // 115200bps
  eBaud230400 = 230400, // 230400bps
  eBaud460800 = 460800, // 460800bps
  eBaud921600 = 921600, // 921600bps
  eBaud1000000 = 1000000, // 1000000bps
}eBaudRate_t;

/**
 * @enum eHttpRevMode_t
 * @brief HTTP 数据接收模式
 */
typedef enum
{
  eBufferMode      = 0, // 缓冲区模式(可主动发送指令请求一定数量数据，OTA升级使用了这种模式)
  ePassthroughMode = 1, // 直传模式(一请求，数据会全部传回来)
  eTransparentMode = 2, // 透传模式
}eHttpRevMode_t;

/**
 * @enum eQos_t
 * @brief MQTT 消息服务质量
 */
typedef enum
{
  eQos0 = 0, // 最多一次
  eQos1 = 1, // 至少一次
  eQos2 = 2, // 仅一次
}eQos_t;


  /**
   * @fn begin
   * @brief 初始化模块
   * @param serial 硬件串口对象
   * @param baud 波特率
   * @return 初始化成功返回 true，失败返回 false
   */
  bool begin(HardwareSerial &serial = Serial1, uint32_t baud = 115200);
  /**
   * @fn begin
   * @brief 使用默认串口（Serial1）初始化模块
   * @param baud 波特率
   * @return 初始化成功返回 true，失败返回 false
   */
  bool begin(uint32_t baud);
  /**
   * @fn checkSIMStatus
   * @brief 查询 SIM 卡状态
   * @return PIN 未上锁返回 true，上锁返回 false
   */
  bool checkSIMStatus(void);

  /**
   * @fn checkSignalQuality
   * @brief 查询信号质量
   * @return 信号质量数值
   */
  int checkSignalQuality(void);

  /**
   * @fn getCLK
   * @brief 获取模块系统时间
   * @return 返回时间字符串
   */
  char *getCLK(void);

  /**
   * @fn getLocalIP
   * @brief 获取本地 IP 地址
   * @return 返回 IP 地址字符串
   */
  char *getLocalIP(void);

  /**
   * @fn queryModuleVersionID
   * @brief 查询模块版本号
   * @return 返回版本号字符串
   */
  char *queryModuleVersionID(void);
  
  /**
   * @fn querySIMCardICCID
   * @brief 查询 SIM 卡 ICCID 号
   * @return 返回 ICCID 字符串
   */
  char *querySIMCardICCID(void);

  /**
   * @fn reset
   * @brief 模块重启
   * @return 发送重启命令成功返回 true，失败返回 false
   */
  bool reset(void);

  /**
   * @fn factoryDataReset
   * @brief 模块恢复出厂设置并重启
   * @return 命令发送成功返回 true，失败返回 false
   */
  bool factoryDataReset(void);

  /**
   * @fn setBaudRate(eBaudRate_t rate)
   * @brief 设置主控与 4G 模块的串口波特率
   * @param rate 波特率类型，eBaudRate_t
   * @return 设置成功返回 true，失败返回 false
   */
  bool setBaudRate(eBaudRate_t rate);

  /**
   * @fn openNetwork(Protocol ptl, const char *host, int port)
   * @brief 建立网络连接
   * @param ptl 网络协议类型：TCP 或 UDP
   * @param host 远程服务器 IP 或域名
   * @param port 远程端口号
   * @return 连接成功返回 true，失败返回 false
   */
  bool openNetwork(Protocol ptl, const char *host, int port);

  /**
   * @fn closeNetwork
   * @brief 断开网络连接
   * @return 断开成功返回 true，失败返回 false
   */
  bool closeNetwork(void);

  /**
   * @fn httpConnect(const char *Host, int port, eHttpRevMode_t mode)
   * @brief 建立 HTTP 连接并设置数据接收模式
   * @param Host 域名或 IP
   * @param port 远程端口
   * @param mode 数据接收模式（缓冲区/透传/URC）
   * @return 连接成功返回 true，失败返回 false
   */
  bool httpConnect(const char *Host, int port = 80, eHttpRevMode_t mode = ePassthroughMode);

  /**
   * @fn httpDisconnect
   * @brief 断开 HTTP 连接
   * @return 断开成功返回 true，失败返回 false
   */
  bool httpDisconnect(void);

  /**
   * @fn httpPost()
   * @brief 发送 HTTP POST 请求
   * @param Host HTTP/HTTPS 地址
   * @param jsonBody JSON 数据体
   * @param port 远程端口
   * @param mode 数据接收模式
   * @return POST 成功返回响应内容，失败返回 NULL
   */
  char *httpPost(const char *Host ,int port = 80,const char *jsonBody="",eHttpRevMode_t mode = ePassthroughMode);
  
  /**
   * @fn httpGet()
   * @brief 发送 HTTP GET 请求
   * @param Host HTTP/HTTPS 地址
   * @param port 远程端口
   * @param path 请求路径，默认“/”
   * @param mode 数据接收模式
   * @return GET 成功返回响应内容，失败返回 NULL
   */
  char *httpGet(const char *Host, int port = 80,const char *path = "",eHttpRevMode_t mode = ePassthroughMode);

  /**
   * @fn httpOTABegin
   * @brief 开始 HTTP OTA 升级
   * @return 启动成功返回 true，失败返回 false
   */
  bool httpOTABegin(void);

  /**
   * @fn httpOTAWriteToFlash
   * @brief 将获取到的 bin 文件数据写入 flash
   * @return 写入成功返回 true，失败返回 false
   */
  bool httpOTAWriteToFlash(void);

  /**
   * @fn send(String str)
   * @brief 发送网络数据
   * @param str 待发送数据
   * @return 发送成功返回 true，失败返回 false
   */
  bool send(String str);

  /**
   * @fn mqttConnect(const char* iot_client, const char* iot_username, const char* iot_key)
   * @brief 连接 MQTT 服务器
   * @param iot_client MQTT 客户端 ID
   * @param iot_username MQTT 用户名
   * @param iot_key MQTT 密钥
   * @return 连接成功返回 true，失败返回 false
   */
  bool mqttConnect(const char* iot_client, const char* iot_username, const char* iot_key);

  /**
   * @fn mqttDisconnect
   * @brief 断开 MQTT 连接
   * @return 断开成功返回 true，失败返回 false
   */
  bool mqttDisconnect(void);

  /**
   * @fn mqttIPOpen(const char *host, int port)
   * @brief 建立 MQTT IP 连接
   * @param host 服务器 IP 或域名
   * @param port 端口号
   * @return 连接成功返回 true，失败返回 false
   */
  bool mqttIPOpen(const char *host, int port);

  /**
   * @fn mqttIPClose(const char *host, int port)
   * @brief 断开 MQTT IP 连接
   * @return 断开成功返回 true，失败返回 false
   */
  bool mqttIPClose(const char *host, int port);

  /**
   * @fn mqttSub(const char* topic, eQos_t qos)
   * @brief 订阅主题
   * @param topic 订阅的主题
   * @param qos 服务质量等级
   * @return 订阅成功返回 true，失败返回 false
   */
  bool mqttSub(const char* topic, eQos_t qos = eQos0);

  /**
   * @fn mqttPub(const char* topic, eQos_t qos, String str)
   * @brief 发布消息
   * @param topic 发布的主题
   * @param qos 服务质量等级
   * @param str 发布的消息内容
   * @return 发布成功返回 true，失败返回 false
   */
  bool mqttPub(const char* topic, eQos_t qos = eQos0,String str = "");

  /**
   * @fn mqttUNSUB(const char* topic)
   * @brief 取消订阅主题
   * @param topic 订阅的主题
   * @return 取消成功返回 true，失败返回 false
   */
  bool mqttUNSUB(const char* topic);

  /**
   * @fn recvMsg(const char* iot_topic, char* buf, int maxlen)
   * @brief 接收 MQTT 消息
   * @param iot_topic MQTT 主题
   * @param buf 存储接收消息的缓冲区
   * @param maxlen 缓冲区最大长度
   * @return 接收成功返回 true，失败返回 false
   */
  bool recvMsg(const char* iot_topic, char* buf, int maxlen);

  /**
   * @fn recvMsg(char* buf, int maxlen)
   * @brief 接收 HTTP 消息
   * @param buf 存储接收消息的缓冲区
   * @param maxlen 缓冲区最大长度
   * @return 接收成功返回 true，失败返回 false
   */
  bool recvMsg(char* buf, int maxlen);

  /**
   * @fn setNtpRetransParam(int cnt, int interval)
   * @brief 设置 NTP 重传参数
   * @param cnt 重传次数
   * @param interval 重传间隔
   * @return 设置成功返回 true，失败返回 false
   */
  bool setNtpRetransParam(int cnt = 5, int interval = 10);

  /**
   * @fn openNtpSynchronization(const char *host)
   * @brief 开启 NTP 同步
   * @param host NTP 服务器地址
   * @return 开启成功返回 true，失败返回 false
   */
  bool openNtpSynchronization(const char *host);
  
  /**
   * @fn activePDPContext
   * @brief 激活 PDP 上下文
   * @return 激活成功返回 true，失败返回 false
   */
  bool activePDPContext(void);

```
## 兼容性
| Platform | Work Well | Work Wrong | Untested | Remarks |
|----------|-----------|------------|----------|---------|
|DFRobot_EDGE101 | √ || | |

## 历史
- 2026/03/09 - 1.0.0 版本

## 创作者
Written by PLELES (feng.yang@dfrobot.com),2026.(Welcome to our [website](https://www.dfrobot.com/))
