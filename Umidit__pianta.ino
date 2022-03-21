//Libreria LiquidCrystal I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

//Libreria Sensore temperatura
#include "DHT.h"
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Attivazione tramite pulsate e transistor
const int pinPulsante = 7;
const int transistorPin = 2;

//Variabili
int numeroLetture = 9;
int sommaValoriLetture = 0;
int valoriLetture[9];
int umiditaRilevata;
int h;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(pinPulsante, INPUT);
  pinMode(pinPulsante, OUTPUT);


  dht.begin();

  Serial.begin(115200);

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("Benvenuto!"));
  delay(2500);
}

void loop() {

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


  lcd.setCursor(0, 0);
  lcd.print(F("Temp: "));
  lcd.print(t);
  lcd.print(F(" C"));

  lcd.setCursor(0, 1);
  lcd.print(F("Umidita: "));
  lcd.print(h);
  lcd.print(F("%"));

  int statoPulsante = digitalRead(pinPulsante);

  if (statoPulsante == HIGH) {
    lcd.clear();
    digitalWrite(transistorPin, HIGH);
    delay(1000);

    for (int i = 0; i <= numeroLetture; i++) {
      valoriLetture[i] = analogRead(A0);
      lcd.setCursor(i, 0);
      lcd.print(F("."));
                  Serial.println(valoriLetture[i]);

      sommaValoriLetture = sommaValoriLetture + valoriLetture[i];

      delay(500);
    }
                  Serial.println(sommaValoriLetture);

    digitalWrite(transistorPin, LOW);

    umiditaRilevata = sommaValoriLetture / (numeroLetture + 1);
            Serial.println(umiditaRilevata);

    h = map(umiditaRilevata, 400, 1015, 100, 0);
    h = constrain(h, 0, 100);
    sommaValoriLetture = 0;


    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rilevato: " + String(h) + "%");
    lcd.setCursor(0, 1);
    lcd.print(F("Limite : 10%"));
    delay(3000);
          lcd.clear();

    if (h < 10) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Innaffia la"));
      lcd.setCursor(0, 1);
      lcd.print(F("pianta!"));
      delay(2500);
      lcd.clear();
    }
  }

  delay(50);

}
