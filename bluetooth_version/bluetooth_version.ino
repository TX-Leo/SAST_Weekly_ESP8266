#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Servo.h>

Servo myservo;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2000);
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
  myservo.attach(2);
  myservo.write(90);
}

void loop() 
{
  while(Serial.available())
  {
    char c = Serial.read();
    if(c == 'on'){
      digitalWrite(2,LOW);
      myservo.write(110);
      delay(1000);
      myservo.write(90);
      Serial.print("灯已开\n");
    }
    
    if(c == 'off'){
      digitalWrite(2,HIGH);
      myservo.write(70);
      delay(1000);
      myservo.write(90);
      Serial.print("灯已关\n");
    }
  }
}
