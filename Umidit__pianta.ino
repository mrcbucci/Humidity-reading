//Libreria LiquidCrystal I2C
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

//Libreria Sensore temperatura
#include "DHT.h"
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Attivazione tramite pulsate e transistor
const int pinPulsante = 7;
const int transistorPin = 2;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(pinPulsante, INPUT);
  pinMode(pinPulsante, OUTPUT);


  dht.begin();
  
  Serial.begin(115200);

  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print(F("Benvenuto!"));
  delay(2500);
}

void loop() {

  int acquaPianta = analogRead(A0);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Umidità: "));
  Serial.print(h);
  Serial.print(F("%  Temperatura: "));
  Serial.print(t);
  Serial.println(F("°C "));
  

  lcd.setCursor(0,0);
  lcd.print(F("Temp: "));
  lcd.print(t);
  lcd.print(F(" C"));

  lcd.setCursor(0,1);
  lcd.print(F("Umidita: "));
  lcd.print(h);
  lcd.print(F("%"));

  int statoPulsante = digitalRead(pinPulsante);
  
  if(statoPulsante == HIGH){
    lcd.clear();
    digitalWrite(transistorPin, HIGH);
    delay(1000);

  int valoriLetture[0];
  int numeroLetture = 10;
  for(int i=0; i=numeroLetture; i++) {
    valoriLetture[i] = analogRead(A0);
        lcd.setCursor(i,0);
        lcd.print(F("."));
    delay(500);
  }
    
/*    int a1 = analogRead(A0);
        lcd.setCursor(0,0);
        lcd.print(F("."));
    delay(500);
    int a2 = analogRead(A0);
        lcd.setCursor(1,0);
        lcd.print(F("."));
    delay(500);
    int a3 = analogRead(A0);
        lcd.setCursor(2,0);
        lcd.print(F("."));
    delay(500);
    int a4 = analogRead(A0);
        lcd.setCursor(3,0);
        lcd.print(F("."));
    delay(500);
    int a5 = analogRead(A0);
        lcd.setCursor(4,0);
        lcd.print(F("."));
    delay(500);
    int a6 = analogRead(A0);
        lcd.setCursor(5,0);
        lcd.print(F("."));
    delay(500);
    int a7 = analogRead(A0);
        lcd.setCursor(6,0);
        lcd.print(F("."));
    delay(500);
    int a8 = analogRead(A0);
        lcd.setCursor(7,0);
        lcd.print(F("."));
    delay(500);
    int a9 = analogRead(A0);
        lcd.setCursor(8,0);
        lcd.print(F("."));
    delay(500);
    int a10 = analogRead(A0);
        lcd.setCursor(9,0);
        lcd.print(F("."));
    delay(500);
*/

    digitalWrite(transistorPin, LOW);

    for (int i=0; i=numeroLetture; i++){
      int acquaPianta = acquaPianta + valoriLetture[i];
    }
    int umidita = acquaPianta/numeroLetture;
//    int acquaPianta = (a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10)/10;
    int h = map(umidita, 340, 1022, 100, 0);
    h = constrain(h, 0, 100);

    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Rilevato: " + String(h) + "%");
    lcd.setCursor(0,1);
    lcd.print(F("Limite : 10%"));
    delay(3000);
     if(h < 10){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("Innaffia la"));
    lcd.setCursor(0,1);
    lcd.print(F("pianta!"));
    delay(2500);
    lcd.clear();
   }
  }

  delay(50);

  }
