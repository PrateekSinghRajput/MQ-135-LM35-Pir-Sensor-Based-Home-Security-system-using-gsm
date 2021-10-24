#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
SoftwareSerial sim(2, 3);

LiquidCrystal_I2C lcd(0x27, 16, 2);
String number = "+918830584864"; // give your phone number

int Temp;


int tempPin = A0;
int redLed = 7;
int greenLed = 8;
int yellowLed = 8;
int motionPin = 10;
int gasPin = A1;
int buzzerPin = 9;
int buttonPin = 11;

int buttonState = 0;
int percentValue = 0;
int sensorValue1 = 0;
int sensorValue2 = 0;
int sensorValue3 = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Wait few seconds");
  delay(5000); // delay for gsm network stable
  lcd.clear();
  lcd.print("System Started");
  Serial.println("System Started...");

  sim.begin(9600);
  delay(250);
  sim.print("AT+CNMI=2,2,0,0,0\r");
  delay(1000);

  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {

  ReadButton();
  tempRead();
  readGas();
  readMotion();
  delay(300);
  lcd.clear();
}

void ReadButton()
{
  buttonState = digitalRead(11);

  if (buttonState == 1)
  {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, HIGH);
    digitalWrite(yellowLed, HIGH);
    sim.println("AT+CMGF=1");
    delay(1000);
    sim.println("AT+CMGS=\"" + number + "\"\r");
    delay(1000);
    String SMS = "Hi, Emergency Alarm detected.";
    sim.println(SMS);
    delay(100);
    sim.println((char)26);
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Emergency Alarm");
    delay(300);
    lcd.clear();
    delay(300);
    lcd.setCursor(0, 0);
    lcd.print("Emergency Alarm");
    delay(300);
    lcd.clear();
    delay(300);
    lcd.setCursor(0, 0);
    lcd.print("Emergency Alarm");
    delay(300);
  }
  else  if (buttonState == 0)
  {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, LOW);
  }

}

void gasMessage()
{
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000);
  String SMS = "Hi, Gas is detected.";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);
  delay(1000);
}

void fireMessage()
{
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000);
  String SMS = "Hi, Fire is detected.";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);
  delay(1000);
}

void thiefMessage()
{
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000);
  String SMS = "Hi, Thief is detected.";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);
  delay(1000);
}

void tempRead()
{
  sensorValue1 = analogRead(tempPin);
  float milivolts = ( sensorValue1 / 1024.0) * 5000;

  Temp = milivolts / 10;
  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print(Temp);
  lcd.write(0xdf);      // for dgree sign
  lcd.print("C ");

  if (Temp > 40)
  {
    digitalWrite(yellowLed, HIGH);
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(redLed, HIGH);
    fireMessage();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fire Detected");
    delay(300);
    lcd.clear();
    delay(300);
    lcd.setCursor(0, 0);
    lcd.print("Fire Detected");
    delay(300);
    lcd.clear();
    delay(300);
    lcd.setCursor(0, 0);
    lcd.print("Fire Detected");
    delay(300);
  }
  else if (Temp < 40)
  {
    digitalWrite(yellowLed, LOW);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(redLed, LOW);
  }
}

void readMotion()
{
  sensorValue2 = digitalRead(motionPin);
  if (sensorValue2 == 1)
  {
    digitalWrite(redLed, HIGH);
    digitalWrite(buzzerPin, HIGH);

    thiefMessage();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Thief Detected");
    delay(300);
    lcd.clear();
    delay(300);
    lcd.setCursor(0, 0);
    lcd.print("Thief Detected");
    delay(300);
    lcd.clear();
    delay(300);
    lcd.setCursor(0, 0);
    lcd.print("Thief Detected");
    delay(300);
  }
  else if (sensorValue2 == 0)
  {
    digitalWrite(redLed, LOW);
    digitalWrite(buzzerPin, LOW);

  }
}


void readGas()
{
  sensorValue2 = analogRead(gasPin);
  percentValue = map(sensorValue2, 0, 600, 0, 100);
  lcd.setCursor(0, 1);
  lcd.print("Gas : ");
  lcd.print(percentValue);
  lcd.print(" %");
  if (percentValue > 50)
  {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(redLed, HIGH);
    gasMessage();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gas Detected");
    delay(300);
    lcd.clear();
    delay(300);
    lcd.setCursor(0, 0);
    lcd.print("Gas Detected");
    delay(300);
    lcd.clear();
    delay(300);
    lcd.setCursor(0, 0);
    lcd.print("Gas Detected");
    delay(300);
  }
  else if (percentValue < 50)
  {
    digitalWrite(greenLed, HIGH);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(redLed, LOW);
  }
}
