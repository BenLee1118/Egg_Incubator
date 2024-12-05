#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define BLYNK_PRINT Serial    
#define BLYNK_TEMPLATE_ID "TMPL6i5aLKSsY"
#define BLYNK_TEMPLATE_NAME "Egg Incubator"
#define BLYNK_AUTH_TOKEN "csTti0NEHCdKXKjlKyFVMz1JCLQwcA1l"
#define RESET D1

WidgetLED Bulb(V7);
WidgetLED Humidifier(V8);
WidgetLED Fan(V9);
WidgetLED Motor(V10);
WidgetLED Buzzer(V11);
WidgetLCD lcd(V12);

char auth[] = "csTti0NEHCdKXKjlKyFVMz1JCLQwcA1l";
char ssid[] = "Ben";
char pass[] = "11111111";

BLYNK_WRITE(V14) 
{
  int pinValue = param.asInt();
  digitalWrite(RESET, pinValue);
}

void setup()
{
  digitalWrite(RESET, HIGH);
  Serial.begin(9600);
  Serial1.begin(115200);
  pinMode(RESET, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() 
{
  char message_1[14]; // Array to store the formatted message
  char message_2[8];
  sprintf(message_1, "Not Yet Hatch");
  sprintf(message_2, "Hatched");
  // run over and over
  if (Serial.available()) 
  {
    Serial.write(Serial.read());
    String receivedData = Serial.readStringUntil('\n');
    delay(100);
    float Index_1 = receivedData.indexOf(',');
    float Index_2 = receivedData.indexOf('!');
    float Index_3 = receivedData.indexOf('@');
    float Index_4 = receivedData.indexOf('#');
    float Index_5 = receivedData.indexOf('$');
    float Index_6 = receivedData.indexOf('*');
    float Index_7 = receivedData.indexOf('%');
    float Index_8 = receivedData.indexOf('^');
    float Index_9 = receivedData.indexOf('&');
    float Index_10 = receivedData.indexOf('`');
    float Index_11 = receivedData.indexOf('?');

      if (Index_1 != -1 || Index_2 != -1 || Index_3 != -1 || Index_4 != -1 || Index_5 != -1
        || Index_6 != -1 || Index_7 != -1 || Index_8 != -1 || Index_9 != -1 || Index_10 != -1 || Index_11 != -1) 
      {
        float receivedTemperature = receivedData.substring(0, Index_1).toFloat();
        float receivedHumidity = receivedData.substring(Index_1 + 1, Index_2).toFloat();
        unsigned short receivedHour = receivedData.substring(Index_2 + 1, Index_3).toInt();
        unsigned short receivedMinute = receivedData.substring(Index_3 + 1, Index_4).toInt();
        unsigned short receivedSecond = receivedData.substring(Index_4 + 1, Index_5).toInt();
        unsigned short receivedDay = receivedData.substring(Index_5 + 1, Index_6).toInt();
        unsigned short receivedMotor = receivedData.substring(Index_6 + 1, Index_7).toInt();
        unsigned short receivedBulb = receivedData.substring(Index_7 + 1, Index_8).toInt();
        unsigned short receivedFan = receivedData.substring(Index_8 + 1, Index_9).toInt();
        unsigned short receivedHumidifier = receivedData.substring(Index_9 + 1, Index_10).toInt();
        unsigned short receivedBuzzer = receivedData.substring(Index_10 + 1, Index_11).toInt();
        unsigned short receivedSentence = receivedData.substring(Index_11 + 1).toInt();
        // Do something with the received values
        Blynk.run(); // Initiates Blynk
          delay(1000);
          Blynk.virtualWrite(V6, receivedTemperature);
          Blynk.virtualWrite(V5, receivedHumidity);
          Blynk.virtualWrite(V0, receivedHour);
          Blynk.virtualWrite(V1, receivedMinute);
          Blynk.virtualWrite(V2, receivedSecond);
          Blynk.virtualWrite(V3, receivedDay);
        if (receivedMotor == 1)
        Motor.on();
        else
          Motor.off();
        if (receivedBulb == 1)
          Bulb.on();
        else
          Bulb.off();
        if (receivedFan == 1)
          Fan.on();
        else
          Fan.off();
        if (receivedHumidifier == 1)
          Humidifier.on();
        else
          Humidifier.off();
        if (receivedBuzzer == 1)
          Buzzer.on();
        else
          Buzzer.off();
        if (receivedSentence == 1)
        {
          lcd.clear();
          lcd.print(0,0,message_2);
        }
        else
        {
          lcd.clear();
          lcd.print(0,0,message_1);
        }
        delay(1000);
        lcd.clear();
        }
        Serial.println(receivedData);
      }
}