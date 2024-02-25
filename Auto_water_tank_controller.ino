#include<NewPing.h>
#include <LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>
int echo = 11;
int trig = 12;
NewPing ultrasonic(trig, echo, 200);
LiquidCrystal_I2C lcd(0x27, 20, 4);
SoftwareSerial mySerial(9, 10);
float low_tank_height = 1;
float height = 15;
float circumference = 47;
float radius = (circumference / 2 * 3.14);
float tank_volume;
float percentage;
void setup() {
  pinMode(echo, INPUT);
  pinMode(8, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(trig, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 1);
  lcd.println("IoT FXIANS");
  SendMessage("system turned on");
  Serial.begin(9600);
  mySerial.begin(9600);
  mySerial.println("AT+CMGF=1\r"); ;
  delay(1000);
  mySerial.println("AT+CNMI=2,2,0,0,0\r");
  delay(1000);
}

void loop() {
  volumes();
  if (percentage < 20) {

    motorOn();
    if (percentage > 90)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.println("motor off");
      digitalWrite(8, LOW);
    }
  }


}
void motorOn()
{
  int values = analogRead(A0);
  String motor_holder = "HIGH";
  if (motor_holder == "HIGH")
  {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.println("motor on");
    digitalWrite(8, HIGH);
    delay(5000);
    if (values > 0)
    {
      digitalWrite(8, HIGH);
      volumes();
    }
  }
  else
    lcd.clear();
  lcd.setCursor(0, 1);
  lcd.println("motor off");
  digitalWrite(8, LOW);
}
void SendMessage(String message)
{
  //Function()
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.println("Sending message");
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CSMP=17,167,0,0\r");
  delay(1000);
  mySerial.println("AT+CMGS=\"+91**********\"\r");
  delay(1000);
  mySerial.println(message);
  delay(100);
  mySerial.println((char)26);
  delay(1000);
}
void volumes()
{
  float distance = ultrasonic.ping_cm() + low_tank_height;
  delay(1000);
  tank_volume = 2 * 3.14 * radius * radius * (height - distance) * 0.001;
  percentage = 100 - ((distance / height) * 100);
  String str = "Distance:" + String(distance) + "\twater_height:" + String(height - distance) + "\tsyntex_volume:" + String(tank_volume) + "\tsyntex_Percent" + String(percentage);
  Serial.println(str);
}
