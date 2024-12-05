/* LIBRARIES INCLUDED */
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include "RTClib.h"
#include <TimerOne.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>

/* Definition of variables */
#define PRESSED  0
#define NOTPRESSED 1
#define DEDUCT 2
#define ADD 3
#define NEXT 4
#define DHTPIN 5
#define DHTTYPE DHT22 // NEED TO CHANGE LATER //
#define BULB 6
#define HUMIDIFIER 7
#define FAN 8
#define TRAY_LEFT 9
#define TRAY_RIGHT 10
#define BUZZER 11
#define reset A0

// Serial Communication //
SoftwareSerial espSerial(12, 13);

/* Declaration of variable */
DateTime now;
DateTime end;

unsigned int    a = 2003;
unsigned short  b = 0;
unsigned short  c = 0;
unsigned short  d = 0;
unsigned short  e = 0;
unsigned short  f = 0;
unsigned short  n = 1;

unsigned long count;
unsigned long count1;

bool            motor_direction = 0;
bool            millis_flag = 1;
bool            Home = 1;
int             rotation_duration = 30;
int             first_upload = 5;
float           humidity;
float           humid_ref = 57.0;
float           humid_ref_tol = 2.0;
float           humid_ref_tres = 3;
float           temperature;
float           temp_ref = 37.7;
float           temp_ref_tol = 0.2;
float           temp_ref_tres = 0.3;
unsigned short  SPIN_time = 2;
unsigned short  SPIN_hour = 2;
unsigned short  SPIN_minute = 59;
unsigned short  SPIN_second = 59;
unsigned short  incub_days_total = 6;
unsigned short  incub_days_left = 6;

float temp_ref_holder;
float humid_ref_holder;
float temp_ref_tol_holder;
float humid_ref_tol_holder;
float temp_ref_tres_holder;
float humid_ref_tres_holder;
unsigned short  SPIN_time_holder;
unsigned short  SPIN_hour_holder = 2;
unsigned short  incub_days_left_holder;
int   incub_days_total_holder;
int   rotation_duration_holder;

short int   SPIN_hour_address = 55;
short const motor_dir_address = 33;
short const temp_ref_address = 0;
short const humid_ref_address = 10;
short const temp_ref_tol_address = 20;
short const humid_ref_tol_address = 30;
short const SPIN_time_address = 40;
short const incub_days_total_address = 50;
short const temp_ref_tres_address = 60;
short const humid_ref_tres_address = 70;
short const incub_days_left_address = 80;
short const startday_year_address = 81;
short const startday_month_address = 82;
short const startday_day_address = 83;
short const startday_hour_address = 84;
short const startday_minute_address = 85;
short const startday_second_address = 86;
int         rotation_duration_address = 100;
int         first_upload_address = 200;
String slogan = "CHICKEN BORNER";

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 20, 4);
DHT dht(DHTPIN, DHTTYPE);

byte customChar[] = {
  B00100,
  B00100,
  B01110,
  B10001,
  B10001,
  B10001,
  B01110,
  B00100
};

byte customChar1[] = {
  B00100,
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100
};

byte arrow[8] = {
  0b01000,
  0b01100,
  0b01110,
  0b01111,
  0b01110,
  0b01100,
  0b01000,
  0b00000
};

void  ft_initialize(void)
{
  digitalWrite(TRAY_LEFT, LOW);
  digitalWrite(TRAY_RIGHT, LOW);

  rtc.begin();
  now = rtc.now();
  dht.begin();
  lcd.init();
  lcd.backlight();

  lcd.setCursor(1, 0);
  lcd.print("###EGG HATCHING###");
  lcd.setCursor(4, 1);
  lcd.print("###MACHINE###");
  lcd.setCursor(0, 2);
  lcd.print("by IDP GROUP 4.");
  delay(500);
  lcd.setCursor(1, 3);
  for (int i = 0; i < 17; i++)
  {
    lcd.setCursor(i + 1, 3);
    lcd.print(slogan[i]);
    delay(100);
  }
  delay(20);
  first_upload = EEPROM.read(first_upload_address);
  delay(20);

  if (first_upload == 255)
  {
    EEPROM.write(first_upload_address, 10);
    delay(20);
    EEPROM.put(temp_ref_address, temp_ref);
    delay(20);
    EEPROM.put(temp_ref_tol_address, temp_ref_tol);
    delay(20);
    EEPROM.put(humid_ref_address, humid_ref);
    delay(20);
    EEPROM.put(humid_ref_tol_address, humid_ref_tol_address);
    delay(20);
    EEPROM.put(temp_ref_tres_address, temp_ref_tres);
    delay(20);
    EEPROM.put(humid_ref_tres_address, humid_ref_tres);
    delay(20);
    EEPROM.write(incub_days_total_address, incub_days_total);
    delay(20);
    EEPROM.write(SPIN_time_address, SPIN_time);
    delay(20);
    EEPROM.write(SPIN_hour_address, SPIN_hour);
    delay(20);
    EEPROM.write(incub_days_left_address, incub_days_left);
    delay(20);
    EEPROM.write(SPIN_time_address, 2);
    delay(20);
    EEPROM.write(rotation_duration_address, rotation_duration);
  }
  delay(20);
  EEPROM.get(temp_ref_address, temp_ref_address);
  delay(20);
  EEPROM.get(temp_ref_tol_address, temp_ref_tol_address);
  delay(20);
  EEPROM.get(humid_ref_address, humid_ref_address);
  delay(20);
  EEPROM.get(humid_ref_tol_address, humid_ref_tol_address);
  delay(20);
  EEPROM.get(temp_ref_tres_address, temp_ref_tres_address);
  delay(20);
  EEPROM.get(humid_ref_tres_address, humid_ref_tres_address);

  incub_days_total = EEPROM.read(incub_days_total_address);
  delay(20);
  SPIN_time = EEPROM.read(SPIN_time_address);
  delay(20);
  SPIN_hour = EEPROM.read(SPIN_hour_address);
  delay(20);
  rotation_duration = EEPROM.read(rotation_duration_address);
  delay(20);
  incub_days_left = EEPROM.read(incub_days_left_address);
  delay(20);
  lcd.clear();
}

void  ft_LED_blink(void)
{
  if (SPIN_second > 0)
    SPIN_second--;  
}

void ft_reset()
{
  wdt_enable(WDTO_15MS);
  while(1);
}

void  setup()
{
  // digitalWrite(reset, HIGH);
  // pinMode(reset, OUTPUT);
  pinMode(NEXT, INPUT_PULLUP);
  pinMode(ADD,INPUT_PULLUP);
  pinMode(DEDUCT,INPUT_PULLUP);
  pinMode(FAN,OUTPUT);
  pinMode(BULB,OUTPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(TRAY_LEFT,OUTPUT);
  pinMode(TRAY_RIGHT,OUTPUT);
  pinMode(HUMIDIFIER,OUTPUT);
  ft_initialize();
  lcd.createChar(0,customChar);
  lcd.createChar(1,customChar1);
  lcd.createChar(10,arrow);

  a = EEPROM.read(startday_year_address) + 1792;
  delay(20);
  b = EEPROM.read(startday_month_address);
  delay(20);
  c = EEPROM.read(startday_day_address);
  delay(20);
  d = EEPROM.read(startday_hour_address); 
  delay(20);
  e= EEPROM.read(startday_minute_address); 
  delay(20);
  f= EEPROM.read(startday_second_address);
  delay(20);

  delay(20);
  DateTime startday(2021,7,5,2,48,0); 
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(ft_LED_blink); // blinkLED to run every 0.15 seconds
  Serial.begin(115200);
  espSerial.begin(9600); // Serial Communication //
}

void  ft_read_temphumid(void)
{
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  lcd.setCursor(6, 0);
  lcd.print(temperature);
  lcd.setCursor(7, 1);
  lcd.print(humidity);  
}

void  ft_buzzer()
{
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
  delay(40);
}

void  ft_save_param(void)
{
  delay(20);
  EEPROM.put(temp_ref_address, temp_ref);
  delay(20);
  EEPROM.put(temp_ref_tol_address, temp_ref_tol);
  delay(20);
  EEPROM.put(humid_ref_address, humid_ref);
  delay(20);
  EEPROM.put(humid_ref_tol_address, humid_ref_tol_address); //here changes motor direction
  delay(20);
  EEPROM.put(temp_ref_tres_address, temp_ref_tres);
  delay(20);
  EEPROM.put(humid_ref_tres_address, humid_ref_tres);
  delay(20);
  EEPROM.put(incub_days_total_address, incub_days_total);
  delay(20);
  incub_days_left = incub_days_total;
  delay(20);
  EEPROM.write(incub_days_left_address,incub_days_left);
  delay(20);

  DateTime startday(a,b,c,d,e,f);
  startday = rtc.now();

  EEPROM.write(startday_year_address,startday.year());
  delay(20);
  EEPROM.write(startday_month_address,startday.month());
  delay(20);
  EEPROM.write(startday_day_address,startday.day());
  delay(20);
  EEPROM.write(startday_hour_address,startday.hour());
  delay(20);
  EEPROM.write(startday_minute_address,startday.minute());
  delay(20);
  EEPROM.write(startday_second_address,startday.second());
  delay(20);

  EEPROM.write(SPIN_time_address, SPIN_time);
  delay(20);
  EEPROM.write(rotation_duration_address, rotation_duration);
  delay(20);
  SPIN_hour = SPIN_time - 1;
  delay(20);
  EEPROM.write(SPIN_hour_address, SPIN_hour);
  delay(20);

  a = EEPROM.read(startday_year_address) + 1792;
  delay(20);
  b = EEPROM.read(startday_month_address) ;
  delay(20);
  c = EEPROM.read(startday_day_address);
  delay(20);
  d = EEPROM.read(startday_hour_address); 
  delay(20);
  e= EEPROM.read(startday_minute_address); 
  delay(20);
  f= EEPROM.read(startday_second_address);
  delay(20);
  rotation_duration_address = rotation_duration;

  if(SPIN_time)
  {
    SPIN_second = 59;
  }
  SPIN_time = 2;
  SPIN_second = 5;
  SPIN_minute = 0;
  SPIN_hour = 0;
}

void  ft_temp_regulation(void)
{
  if (temperature == temp_ref)
  {
    digitalWrite(FAN, LOW);
    digitalWrite(BULB, LOW);
  }
  else if ((temperature >= (temp_ref - temp_ref_tol)) && (temperature <= (temp_ref + temp_ref_tol)))
  {
    digitalWrite(FAN, LOW);
    digitalWrite(BULB, LOW);
    lcd.setCursor(16, 0);
    lcd.write(0);
  }
  ft_read_temphumid();
  if (temperature > (temp_ref + temp_ref_tol))
  {
    digitalWrite(FAN, HIGH);
    digitalWrite(BULB, LOW);

    if (temperature >= (temp_ref + temp_ref_tol + temp_ref_tres))
    {
      // ft_buzzer();
      digitalWrite(BUZZER, HIGH);
      lcd.setCursor(16, 0);
      lcd.write(1);
      delay(100);
      digitalWrite(BUZZER, LOW);
      lcd.setCursor(16, 0);
      lcd.print(" ");
      delay(100);
    }
    lcd.setCursor(16, 0);
    lcd.write(1);
  }
  ft_read_temphumid();
  if (temperature < (temp_ref - temp_ref_tol - 0.1))
  {
    digitalWrite(FAN, LOW);
    digitalWrite(BULB, HIGH);

    if (temperature <= (temp_ref - temp_ref_tol - temp_ref_tres - 0.1))
    {
      ft_buzzer();
      lcd.setCursor(16, 0);
      lcd.write(1);
      delay(100);
      lcd.setCursor(16, 0);
      lcd.print(" ");
      delay(100);
    }
    lcd.setCursor(16, 0);
    lcd.write(1);
  }
  if (digitalRead(BULB) == HIGH)
  {
    espSerial.print("%"); // Serial Communication //
    espSerial.print(1); // Serial Communication //
    delay(200);
  }
  else
  {
    espSerial.print("%"); // Serial Communication //
    espSerial.print(0); // Serial Communication //
    delay(200);
  }
  if (digitalRead(FAN) == HIGH)
  {
    espSerial.print("^"); // Serial Communication //
    espSerial.print(1); // Serial Communication //
    delay(200);
  }
  else
  {
    espSerial.print("^"); // Serial Communication //
    espSerial.print(0); // Serial Communication //
    delay(200);
  }
}


void  ft_humid_regulation()
{
  if ((humidity >= (humid_ref - humid_ref_tol)) && (humidity <= (humid_ref + humid_ref_tol)))
  {
    digitalWrite(HUMIDIFIER, LOW);
    // digitalWrite(FAN, LOW);
    lcd.setCursor(16, 1);
    lcd.write(0);
  }
  ft_read_temphumid();
  if (humidity > (humid_ref + humid_ref_tol + 0.1))
  {
    digitalWrite(HUMIDIFIER, LOW);
    // digitalWrite(FAN, HIGH);
    // delay(10000);
    if (humidity >= (humid_ref + humid_ref_tol + humid_ref_tres + 0.1))
    {
      ft_buzzer();
      lcd.setCursor(16, 1);
      lcd.write(1);
      delay(100);
      lcd.setCursor(16, 1);
      lcd.print(" ");
      delay(100);
    }
    lcd.setCursor(16, 1);
    lcd.write(1);
  }
  ft_read_temphumid();
  if (humidity < (humid_ref - humid_ref_tol))
  {
    digitalWrite(HUMIDIFIER, HIGH);
    // digitalWrite(FAN, LOW);
    if (humidity <= (humid_ref - humid_ref_tol - humid_ref_tres - 0.1))
    {
      ft_buzzer();
      lcd.setCursor(16, 1);
      lcd.write(1);
      delay(100);
      lcd.setCursor(16, 1);
      lcd.print(" ");
      delay(100);
    }
    lcd.setCursor(16,1);
    lcd.write(1);
  }
  if (digitalRead(HUMIDIFIER) == HIGH)
  {
    espSerial.print("&"); // Serial Communication //
    espSerial.print(1); // Serial Communication //
    delay(200);
  }
  else
  {
    espSerial.print("&"); // Serial Communication //
    espSerial.print(0); // Serial Communication //
    delay(200);
  }
}

void milsecdelay()
{

    if (millis_flag){
      millis_flag=0;
      count =millis();
    }

    count1=millis();
    if ((count1-count)>=50)
    {
      // action
      millis_flag=1;
    }
}

void  ft_discard_param()
{
  temp_ref = temp_ref_holder;
  humid_ref = humid_ref_holder;
  temp_ref_tol = temp_ref_tol_holder;
  humid_ref_tol = humid_ref_tol_holder;
  temp_ref_tres = temp_ref_tres_holder;
  humid_ref_tres = humid_ref_tres_holder;
  SPIN_time = SPIN_time_holder;
  incub_days_total = incub_days_total_holder;
  incub_days_left = incub_days_left_holder;
  SPIN_hour = SPIN_hour_holder;
  rotation_duration = rotation_duration_holder;
}
void  ft_menu()
{
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("ENTERING MENU");
  lcd.setCursor(4, 1);
  lcd.print("HOLD ON...");
  temp_ref_holder = temp_ref;
  humid_ref_holder = humid_ref;
  temp_ref_tol_holder = temp_ref_tol;
  humid_ref_tol_holder = humid_ref_tol;
  temp_ref_tres_holder = temp_ref_tres;
  humid_ref_tres_holder = humid_ref_tres;
  SPIN_time_holder = SPIN_time;
  incub_days_total_holder = incub_days_total;
  incub_days_left_holder = incub_days_left;
  SPIN_hour_holder = SPIN_hour;
  rotation_duration_holder = rotation_duration;

  delay(200);
  lcd.clear();

  do
  {
    lcd.setCursor(4, 1);
    lcd.print(">>");
    lcd.setCursor(1, 0);
    lcd.print("Set Temp: ");
    lcd.setCursor(6, 1);
    lcd.print(temp_ref);
    lcd.setCursor(10, 1);
    lcd.print((char) 223);
    lcd.setCursor(11, 1);
    lcd.print("C");

    lcd.setCursor(2, 2);
    lcd.print("Temp TOL Value:");
    lcd.setCursor(6, 3);
    lcd.print(temp_ref_tol);
    lcd.setCursor(10, 3);
    lcd.print((char) 223);
    lcd.setCursor(11, 3);
    lcd.print("C");

    if (digitalRead(ADD) == PRESSED && (temp_ref < 99))
    {
      temp_ref += 0.1;
      delay(200);
    }
    if (digitalRead(DEDUCT) == PRESSED && (temp_ref > 2))
    {
      temp_ref -= 0.1;
      delay(200);
    }
  }
  while (digitalRead(NEXT) == NOTPRESSED);

  do
  {
  }
  while (digitalRead(NEXT) == PRESSED); // debouncing of the menubutton pressing
  lcd.setCursor(4, 1);
  lcd.print("  "); // special select item character removal

  do
  {
    lcd.setCursor(4, 3);
    lcd.print(">>");
    lcd.setCursor(2, 2);
    lcd.print("Temp TOL Value: ");
    lcd.setCursor(6,3);
    lcd.print(temp_ref_tol);
    lcd.setCursor(10,3);
    lcd.print((char) 223);
    lcd.setCursor(11,3);
    lcd.print("C");

    if (digitalRead(ADD) == PRESSED && (temp_ref_tol < 5))
    {
      temp_ref_tol += 0.1;
      delay(200);
    }
    if (digitalRead(DEDUCT) == PRESSED && (temp_ref_tol > 0))
    {
      temp_ref_tol -= 0.1;
      delay(200);
    }
  } 
  while (digitalRead(NEXT) == NOTPRESSED);

  do
  {
  }
  while (digitalRead(NEXT) == PRESSED);
  lcd.clear();

  do
  {
    lcd.setCursor(4, 1);
    lcd.print(">>");
    lcd.setCursor(3, 0);
    lcd.print("Set Humidity:");
    lcd.setCursor(6, 1);
    lcd.print(humid_ref);
    lcd.setCursor(11, 1);
    lcd.print("% RH");

    lcd.setCursor(1, 2);
    lcd.print("Humidity Tolerance:");
    lcd.setCursor(6, 3);
    lcd.print(humid_ref_tol);
    lcd.setCursor(10, 3);
    lcd.print("% RH");

    if (digitalRead(ADD) == PRESSED && (humid_ref < 99))
    {
      humid_ref += 0.1;
      delay(200);
    }
    if (digitalRead(DEDUCT) == PRESSED && (humid_ref > 5))
    {
      humid_ref -= 0.1;
      delay(200);
    }
  }
  while (digitalRead(NEXT) == NOTPRESSED);

  do
  {
  }
  while (digitalRead(NEXT) == PRESSED); // debouncing of the menubutton pressing
  lcd.setCursor(4, 1);
  lcd.print("  "); // special select item character removal

  do
  {
    lcd.setCursor(4, 3);
    lcd.print(">>");
    lcd.setCursor(1, 2);
    lcd.print("Humidity Tolerance:");
    lcd.setCursor(6,3);
    lcd.print(humid_ref_tol);
    lcd.setCursor(10,3);
    lcd.print("% RH");

    if (digitalRead(ADD) == PRESSED && (humid_ref_tol < 99))
    {
      humid_ref_tol += 0.1;
      delay(200);
    }
    if (digitalRead(DEDUCT) == PRESSED && (humid_ref_tol > 0))
    {
      humid_ref_tol -= 0.1;
      delay(200);
    }
  } 
  while (digitalRead(NEXT) == NOTPRESSED);

  do
  {
  }
  while (digitalRead(NEXT) == PRESSED);
  lcd.clear();

  do
  {
    lcd.setCursor(4,1);
    lcd.print(">>");// special select item character
    lcd.setCursor(1,0);
    lcd.print("Temp_Diff Treshold:");
    lcd.setCursor(6,1);
    lcd.print(temp_ref_tres);
    lcd.setCursor(10,1);
    lcd.print((char) 223);
    lcd.setCursor(11,1);
    lcd.print("C");

    lcd.setCursor(0,2);
    lcd.print("Humid_Diff Treshold:");
    lcd.setCursor(6,3);
    lcd.print(humid_ref_tres);
    lcd.setCursor(10,3);
    lcd.print("% RH");

    if (digitalRead(ADD) == PRESSED)
    {
      temp_ref_tres += 0.1;
      delay(200);
    }
    if (digitalRead(DEDUCT) == PRESSED)
    {
      temp_ref_tres -= 0.1;
      delay(200);
    }
  }
  while(digitalRead(NEXT)== NOTPRESSED);

  do
  {
  }
  while (digitalRead(NEXT) == PRESSED); // debouncing of the menubutton pressing
  lcd.setCursor(4, 1);
  lcd.print("  "); // special select item character removal
  
  do
  {
    lcd.setCursor(4,3);
    lcd.print(">>");// special select item character
    lcd.setCursor(0,2);
    lcd.setCursor(0,2);
    lcd.print("Humid_Diff Treshold:");
    lcd.setCursor(6,3);
    lcd.print(humid_ref_tres);
    lcd.setCursor(10,3);
    lcd.print("% RH");
    
    if (digitalRead(ADD) == PRESSED)
    { 
      humid_ref_tres += 0.1;
      delay(200);
    }
    if (digitalRead(DEDUCT) == PRESSED)
    { 
      humid_ref_tres -= 0.1;
      delay(200);
    }
  }
  while(digitalRead(NEXT) == NOTPRESSED);

  do
  {
  }
  while (digitalRead(NEXT) == PRESSED);
  lcd.clear();

  do
  {
    if (incub_days_total > 50)
      incub_days_total = 21;
    lcd.setCursor(4,2);
    lcd.print(">>");
    lcd.setCursor(0,0);
    lcd.print("Set Incubation_Time:");
    lcd.setCursor(6,2);
    lcd.print(incub_days_total);
    lcd.setCursor(9,2);
    lcd.print("Days");
    
    if (digitalRead(ADD) == PRESSED && incub_days_total < 46)
    { 
      incub_days_total++;
      delay(200);
    }
    if (digitalRead(DEDUCT) == PRESSED && incub_days_total > 1)
    { 
      incub_days_total--;
      delay(200);
    }
    if (incub_days_total < 10)
    {
      lcd.setCursor(7, 2);
      lcd.print(" ");
    }
  }
  while (digitalRead(NEXT) == NOTPRESSED);

  do
  {
  }
  while (digitalRead(NEXT) == PRESSED);
  lcd.clear();

  do
  {
    lcd.setCursor(4, 2);
    lcd.print(">>");
    lcd.setCursor(0, 0);
    lcd.print("Time Between Spins:");
    lcd.setCursor(6,2);
    lcd.print(SPIN_time);
    lcd.setCursor(9,2);
    lcd.print("Hour/s");

    if (digitalRead(ADD) == PRESSED && SPIN_time < 13)
    {
      SPIN_time++;
      delay(100);
    }
    if (digitalRead(DEDUCT) == PRESSED && SPIN_time > 0)
    {
      SPIN_time--;
      delay(100);
    }
    if (SPIN_time < 10)
    {
      lcd.setCursor(7, 2);
      lcd.print(" ");
    }
  }
  while (digitalRead(NEXT) == NOTPRESSED);

  do
  {
  }
  while (digitalRead(NEXT) == PRESSED);
  lcd.clear();

  do
  {
    lcd.setCursor(4, 2);
    lcd.print(">>");
    lcd.setCursor(0, 0);
    lcd.print("Time During Rot:");
    lcd.setCursor(6, 2);
    lcd.print(rotation_duration);
    lcd.setCursor(8, 2);
    lcd.print("s");

    if (rotation_duration < 10)
    {
      lcd.setCursor(7, 2);
      lcd.print(" ");
    }
    if (digitalRead(ADD) == PRESSED && rotation_duration < 100)
    {
      rotation_duration++;
      if (rotation_duration < 10)
      {
        lcd.setCursor(7, 2);
        lcd.print(" ");
      }
      delay(200);
    }
    if (digitalRead(DEDUCT) == PRESSED && rotation_duration > 1)
    {
      rotation_duration--;
      // if (rotation_duration < 10)
      // {
        lcd.setCursor(7, 2);
        lcd.print(" ");
      // }
      delay(200);
    }
  }
  while (digitalRead(NEXT) == NOTPRESSED);

  do
  {
  }
  while (digitalRead(NEXT) == PRESSED);
  lcd.clear();

  do
  {
    lcd.setCursor(0, 0);
    lcd.print("Press 'NEXT' to save");
    lcd.setCursor(2,1);
    lcd.print(" any changes or");
    lcd.setCursor(0,2);
    lcd.print("any key to continue");
  }
  while (digitalRead(NEXT) == NOTPRESSED && digitalRead(ADD) == NOTPRESSED && digitalRead(DEDUCT) == NOTPRESSED);

  if (!digitalRead(NEXT))
  {
    lcd.clear();
    ft_save_param();

    lcd.clear();
    lcd.print("SAVING...");
    lcd.setCursor(3, 3);
    for (int i = 0; i < 17; i++)
    {
      lcd.setCursor(i + 1, 1);
      lcd.write(byte(10));
      lcd.setCursor(i + 1, 2);
      lcd.write(byte(10));
      delay(60);
    }
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("SAVED SUCCESSFULLY!!");
    delay(500);
  }
  else
  {
    lcd.clear();
    ft_discard_param();
    lcd.setCursor(0,0);
    lcd.print("changes discarded");
    delay(300);
  }
  lcd.clear();
}

void  loop()
{
  now = rtc.now();
  
  lcd.setCursor(15, 2);
  lcd.print(SPIN_second);
  lcd.setCursor(0, 0);
  lcd.print("Temp= ");
  lcd.setCursor(6, 0);
  lcd.print(temperature);
  espSerial.print(" "); // Serial Communication //
  espSerial.print(temperature); // Serial Communication //
  delay(200);
  espSerial.print(","); // Serial Communication //
  espSerial.print(humidity); // Serial Communication //
  delay(200);
  lcd.setCursor(12, 0);
  lcd.print((char) 223);
  lcd.setCursor(13, 0);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humid= ");
  lcd.setCursor(7, 1);
  lcd.print(humidity);
  lcd.setCursor(13, 1);
  lcd.print("%");

  if (SPIN_time == 0)
  {
    lcd.setCursor(0, 2);
    lcd.print("RotTime= ");
    lcd.setCursor(9, 2);
    lcd.print("00");
    lcd.setCursor(11, 2);
    lcd.print(":");
    lcd.setCursor(12, 2);
    lcd.print("00");
    lcd.setCursor(14, 2);
    lcd.print(":");
    lcd.setCursor(15, 2);
    lcd.print("00");
  }
  else
  {
    lcd.setCursor(0, 2);
    lcd.print("RotTime= ");
    lcd.setCursor(9, 2);
    lcd.print(SPIN_hour);
    espSerial.print("!"); // Serial Communication //
    espSerial.print(SPIN_hour); // Serial Communication //
    delay(200);
    lcd.setCursor(11, 2);
    lcd.print(":");
    lcd.setCursor(12, 2);
    lcd.print(SPIN_minute);
    espSerial.print("@"); // Serial Communication //
    espSerial.print(SPIN_minute); // Serial Communication //
    delay(200);
    lcd.setCursor(14, 2);
    lcd.print(":");
    lcd.setCursor(15, 2);
    lcd.print(SPIN_second);
    espSerial.print("#"); // Serial Communication //
    espSerial.print(SPIN_second); // Serial Communication //
    delay(200);
  }

  lcd.setCursor(0, 3);
  lcd.print("CountDown= ");
  lcd.setCursor(11, 3);
  lcd.print(incub_days_left);
  espSerial.print("$"); // Serial Communication //
  espSerial.print(incub_days_left); // Serial Communication //
  delay(200);

  if (incub_days_left < 10)
  {
    lcd.setCursor(12, 3);
    lcd.print(" ");
  }
  lcd.setCursor(14, 3);
  lcd.print("Days");

  lcd.setCursor(15, 2);
  lcd.print(SPIN_second);
  if (SPIN_second < 10)
  {
    lcd.setCursor(16, 2);
    lcd.print(" ");
  }
  if (SPIN_minute < 10)
  {
    lcd.setCursor(13, 2);
    lcd.print(" ");
  }
  if (SPIN_hour < 10)
  {
    lcd.setCursor(10, 2);
    lcd.print(" ");
  }

  if (SPIN_second == 0 && SPIN_minute)
  {
    SPIN_minute -= 1;
    SPIN_second = 59;
  }
  if (SPIN_minute == 0 && SPIN_hour)
  {
    SPIN_hour -= 1;
    SPIN_minute = 59;
  }
  if (SPIN_second == 0 && SPIN_minute == 0 && SPIN_hour == 0)
  {
    delay(20);
    motor_direction = EEPROM.read(motor_dir_address);
    delay(20);
    espSerial.print("*"); // Serial Communication //
    espSerial.print(1); // Serial Communication //
    delay(200);
    if (motor_direction)
    {
      motor_direction = 0;
      delay(20);
      EEPROM.write(motor_dir_address, motor_direction);
      delay(20);
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("TRAY ROTATE RIGHT");
      digitalWrite(TRAY_LEFT, LOW);
      digitalWrite(TRAY_RIGHT, HIGH);
      delay(rotation_duration * 1000);
      digitalWrite(TRAY_RIGHT, LOW);
      digitalWrite(TRAY_LEFT, LOW);
      lcd.clear();
    }
    else
    {
      motor_direction = 1;
      delay(20);
      EEPROM.write(motor_dir_address, motor_direction);
      delay(20);
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("TRAY ROTATE LEFT");
      digitalWrite(TRAY_RIGHT, LOW);
      digitalWrite(TRAY_LEFT, HIGH);
      delay(rotation_duration * 1000);
      digitalWrite(TRAY_RIGHT, LOW);
      digitalWrite(TRAY_LEFT, LOW);
      lcd.clear();
    }
    SPIN_time = EEPROM.read(SPIN_time_address);
    SPIN_hour = SPIN_time - 1;
    SPIN_minute = 59;
    SPIN_second = 59;
    ft_reset();
  }
  else
  {
    espSerial.print("*"); // Serial Communication //
    espSerial.print(0); // Serial Communication //
    delay(200);
  }
  lcd.setCursor(15, 2);
  lcd.print(SPIN_second);

  ft_temp_regulation();
  ft_humid_regulation();

  if ((temperature >= (temp_ref + temp_ref_tol + temp_ref_tres)) || (temperature <= (temp_ref - temp_ref_tol - temp_ref_tres - 0.1))
    || (humidity >= (humid_ref + humid_ref_tol + humid_ref_tres + 0.1)) || (humidity <= (humid_ref - humid_ref_tol - humid_ref_tres - 0.1)))
  {
    espSerial.print("`"); // Serial Communication //
    espSerial.print(1); // Serial Communication //
    delay(200);
  }
  else
  {
    espSerial.print("`"); // Serial Communication //
    espSerial.print(0); // Serial Communication //
    delay(200);
  }

  lcd.setCursor(15, 2);
  lcd.print(SPIN_second);

  if (digitalRead(NEXT) == PRESSED)
    ft_menu();
  lcd.setCursor(15, 2);
  lcd.print(SPIN_second);

  if (!incub_days_left)
  {
    ft_buzzer();
    lcd.setCursor(0, 3);
    lcd.print("                   ");
    delay(10);
    lcd.setCursor(0, 3);
    lcd.print("!!! EGG HATECHED !!!");
    delay(500);
    lcd.setCursor(0, 3);
    lcd.print("                    ");
    delay(10);
    espSerial.print("?"); // Serial Communication //
    espSerial.println(1); // Serial Communication //
    delay(200);
  }
  else
  {
    espSerial.print("?"); // Serial Communication //
    espSerial.println(0); // Serial Communication //    
    delay(200);
  }
  lcd.setCursor(15, 2);
  lcd.print(SPIN_second);

  DateTime startday(a, b, c, d, e, f);
  delay(20);

/* This code from function below is for demonstration which allows the day value to go in seconds instead of seconds */
  // while (n < 50)
  // {
  //   if (incub_days_total >= n)
  //   {
  //     if (rtc.now() == (startday + TimeSpan(0, 0, 0, n)))
  //     {
  //       incub_days_left = incub_days_total - n;
  //       EEPROM.write(incub_days_left_address, incub_days_left);
  //       delay(10);
  //     }
  //   }
  //   n++;
  // }

/*  This code from function below is the proper day duration instead of seconds */

  for (unsigned short n=1; n<50; n++)
  {
    if (incub_days_total >= n)
    {
      if  ((rtc.now() == ( startday + TimeSpan(n,0,0,0)))||(rtc.now() == ( startday + TimeSpan(n,0,30,0)))||(rtc.now() == ( startday + TimeSpan(n,1,0,0)))||(rtc.now() == ( startday + TimeSpan(n,2,0,0)))||(rtc.now() == ( startday + TimeSpan(n,5,0,0)))||(rtc.now() == ( startday + TimeSpan(n,10,0,0)))) 
      {
        incub_days_left = incub_days_total - n;
        EEPROM.write(incub_days_left_address, incub_days_left);
        delay(10);
        }
    }
}

  // while (n < 50)
  // {
  //   if (incub_days_total >= n)
  //   {
  //     if (rtc.now() == (startday + TimeSpan(n, 0, 0, 0)) || rtc.now() == (startday + TimeSpan(n, 0, 30, 0)) || rtc.now() == (startday + TimeSpan(n, 1, 0, 0))
  //       || rtc.now() == (startday + TimeSpan(n, 2, 0, 0)) || rtc.now() == (startday + TimeSpan(n, 5, 0, 0)) || rtc.now() == (startday + TimeSpan(n, 10, 0, 0)))
  //     {
  //       incub_days_left = incub_days_total - n;
  //       EEPROM.write(incub_days_left_address, incub_days_left);
  //       delay(10);
  //     }
  //   }
  //   n++;
  // }
}