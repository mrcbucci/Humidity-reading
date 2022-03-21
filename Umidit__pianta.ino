//Display LiquidCrystal I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

//Temperature sensor
#include "DHT.h"
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Transistor and botton
const int bottonPin = 7;
const int transistorPin = 2;

//Var
int numberScans = 9;
int sumScans = 0;
int scanning[9];
int partialH;
int h;


void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(bottonPin, INPUT);
  pinMode(transistorPin, OUTPUT);

  dht.begin();

  Serial.begin(9600);

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print(F("Welcome!"));
  delay(2500);
}

void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    
    Serial.println(F("Failed to read from DHT sensor!"));
    
    lcd.setCursor(0, 0);
    lcd.print(F("Temp: "));
    lcd.print("N.D.");
    lcd.print(F(" C"));
    lcd.setCursor(0, 1);
    lcd.print(F("Humidity: "));
    lcd.print("N.D.");
    lcd.print(F("%"));
    
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

  lcd.setCursor(0, 0);
  lcd.print(F("Temp: "));
  lcd.print(t);
  lcd.print(F(" C"));
  lcd.setCursor(0, 1);
  lcd.print(F("Humidity: "));
  lcd.print(h);
  lcd.print(F("%"));

  int bottonState = digitalRead(bottonPin);

  if (bottonState == HIGH) {
    lcd.clear();
    digitalWrite(transistorPin, HIGH);
    delay(1000);

    for (int i = 0; i <= numberScans; i++) {
      scanning[i] = analogRead(A0);
      lcd.setCursor(i, 0);
      lcd.print(F("."));
      Serial.println(scanning[i]);

      sumScans = sumScans + scanning[i];

      delay(500);
    }
    Serial.println(sumScans);

    digitalWrite(transistorPin, LOW);

    partialH = sumScans / (numberScans + 1);
    Serial.println(partialH);

    h = map(partialH, 400, 1015, 100, 0);
    h = constrain(h, 0, 100);
    sumScans = 0;


    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Detected: " + String(h) + "%");
    lcd.setCursor(0, 1);
    lcd.print(F("Limit: 10%"));
    delay(3000);
    lcd.clear();

    if (h < 10) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Water the plant!"));
      delay(2500);
      lcd.clear();
    }
  }

  delay(100);

}
