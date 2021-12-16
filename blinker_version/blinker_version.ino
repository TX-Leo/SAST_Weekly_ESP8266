#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT

#include <Blinker.h>
#include <Servo.h>

Servo myservo;//定义舵机

char auth[] = "5b76962ec546";  //点灯Key279babef0a2c
char ssid[] = "TX";      //wifi名称
char pswd[] = "20020603wz";     //wifi密码

// 新建组件对象
BlinkerButton Button1("test");

int counter = 0;

void miotPowerState(const String & state)//电源类操作
{
    BLINKER_LOG("need set power state: ", state);
    /*******************在此开始开启Blinker的操作*********************/
    if (state == BLINKER_CMD_ON) {
  
    myservo.write(170);//收到“on”的指令后舵机旋转170度（待修改）
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();//反馈状态
    delay(1000);//延时1秒
    myservo.write(0);//舵机归零，回到垂直状态
        
    }
    /*******************在此结束开启Blinker的操作*********************/
    
    /*******************在此开始关闭Blinker的操作*********************/
    else if (state == BLINKER_CMD_OFF) {
      
     myservo.write(170);            //舵机偏转170°
     BlinkerMIOT.powerState("off");
     BlinkerMIOT.print();
     delay(1000);
     myservo.write(0);     
    }
    /*******************在此结束关闭Blinker的操作*********************/
}

// 按下按键即会执行该函数
void button1_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);
   /*******************在此开始开关灯的操作*********************/
      if (state=="on") 
    {
        myservo.write(170);//收到“on”的指令后舵机旋转170
        delay(1000);//延时
        myservo.write(0);//舵机归零垂直
        
    } else if(state=="press"||state=="tap")
    {
       myservo.write(170);//长按开关按键后舵机旋转170
       delay(1000);//延时
       myservo.write(0);//舵机归零垂直
    }
    /*******************在此结束开关灯的操作*********************/
}

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
    counter++;
 
}

void setup()
{
    // 初始化串口
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    /*******************在此开始开初始化的操作*********************/
    // 初始化舵机
    myservo.attach(2);//舵机的IO口，nodemcu的D4口
    myservo.write(0);//上电时舵机归零垂直
    /*******************在此结束初始化的操作*********************/
  
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);

    Button1.attach(button1_callback);
}

void loop() {
    Blinker.run();
}
