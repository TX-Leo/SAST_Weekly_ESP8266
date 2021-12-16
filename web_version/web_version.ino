#include <ESP8266WiFi.h>        // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h>   //  ESP8266WebServer库
#include <Servo.h>
 
ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'
ESP8266WebServer esp8266_server(80);// 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）

char ssid[] = "TX";      //wifi名称
char pswd[] = "20020603wz";     //wifi密码
char html[]="<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>";   //所实现网页html代码

int flag = 0;
Servo myservo;//定义舵机

void setup(void){
  Serial.begin(115200);   // 启动串口通讯
  /************************在此开始你的初始化*******************************/ 
  pinMode(LED_BUILTIN, OUTPUT); //设置内置LED引脚为输出模式以便控制LED
  myservo.attach(2);//舵机的IO口，nodemcu的D4口
  myservo.write(0);//上电时舵机归零垂直（0°）
  /************************在此结束你的初始化*******************************/ 
  
  wifiMulti.addAP(ssid, pswd); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP(ssid, pswd); // ESP8266-NodeMCU再启动后会扫描当前网络

  Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。
  
  int i = 0;                                 
  while (wifiMulti.run() != WL_CONNECTED) {  // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
    delay(1000);                             // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
    Serial.print(i++); Serial.print(' ');    // 将会连接信号最强的那一个WiFi信号。
  }                                          // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
                                             // 此处while循环判断是否跳出循环的条件。
  
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // 通过串口监视器输出连接的WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // 通过串口监视器输出ESP8266-NodeMCU的IP
 
  esp8266_server.begin();                           // 启动网站服务
  esp8266_server.on("/", HTTP_GET, handleRoot);     // 设置服务器根目录即'/'的函数'handleRoot'
  esp8266_server.on("/LED", HTTP_POST, handleLED);  // 设置处理LED控制请求的函数'handleLED'
  esp8266_server.onNotFound(handleNotFound);        // 设置处理404情况的函数'handleNotFound'
 
  Serial.println("HTTP esp8266_server started");//  告知用户ESP8266网络服务功能已经启动
}
 
void loop(void){
  esp8266_server.handleClient();                     // 检查http服务器访问
}
 
/*设置服务器根目录即'/'的函数'handleRoot'
  该函数的作用是每当有客户端访问NodeMCU服务器根目录时，
  NodeMCU都会向访问设备发送 HTTP 状态 200 (Ok) 这是send函数的第一个参数。
  同时NodeMCU还会向浏览器发送HTML代码，以下示例中send函数中第三个参数，
  也就是双引号中的内容就是NodeMCU发送的HTML代码。该代码可在网页中产生LED控制按钮。 
  当用户按下按钮时，浏览器将会向NodeMCU的/LED页面发送HTTP请求，请求方式为POST。
  NodeMCU接收到此请求后将会执行handleLED函数内容*/
void handleRoot() {       
  /************************在此开始你的html*******************************/      
  esp8266_server.send(200, "text/html", html);
  /************************在此结束你的html*******************************/   
}
//处理LED控制请求的函数'handleLED'
void handleLED() {
  /************************在此开始你的代码*******************************/                         
  if(flag == 0){
    myservo.write(170);     //收到“on”的指令后舵机旋转170度
    delay(1000);//延时1秒
    myservo.write(0);     //舵机归零，回到垂直状态，不要修改！
    flag = 1;
  }
  else if(flag == 1){
    myservo.write(170);     //收到“off”的指令后舵机旋转170度
    delay(1000);//延时1秒
    myservo.write(0);     //舵机归零，回到垂直状态，不要修改！
    flag = 0;                         
  }
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));// 改变LED的点亮或者熄灭状态
  /************************在此结束你的代码*******************************/ 
  esp8266_server.sendHeader("Location","/");          // 跳转回页面根目录
  esp8266_server.send(303);                           // 发送Http相应代码303 跳转  
}
 
// 设置处理404情况的函数'handleNotFound'
void handleNotFound(){
  esp8266_server.send(404, "text/plain", "404: Not found"); // 发送 HTTP 状态 404 (未找到页面) 并向浏览器发送文字 "404: Not found"
}
