#include "HX711.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,4,5,6,7);
const int buttonPin = 12;    //start sequence button
const int ledPin = 13;       //LED indicator and/or buzzer
const int igniterPin = 11;   //igniter transistor circuit
int buttonState = 0;
#define DOUT 3
#define CLK 2
HX711 scale(2,3);
float calibration_factor = -760;   //CHANGE FOR ADJUSTMENT


void setup() {
  // put your setup code here, to run once:
  
  pinMode(buttonPin, INPUT);
  pinMode(igniterPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("HX711 Rocket Motor Dynamometer, V.5");
  Serial.println("Place rocket motor oriented downwards in holder. Move away from test stand doods.");
  Serial.println("Press the button to start.");
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" MOTOR");
  lcd.setCursor(0, 1);
  lcd.print(" DYNAMOMETER");
  delay(1000);
  scale.set_scale();
  scale.tare();   //Reset the scale to 0 here
  long zero_factor = scale.read_average();   //Get a baseline reading
  Serial.print("Zero factor: ");
  Serial.println(zero_factor);
  Serial.println("");
  
  }

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Rocket dyno");
  lcd.setCursor(0, 1);
  lcd.print("Standby");
  scale.set_scale(calibration_factor);
  lcd.print(scale.get_units(), 1);
  lcd.print(" g");
  delay(500);
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Rocket dyno ");
    lcd.setCursor(0, 1);
    lcd.print(" STAND CLEAR! ");
    Serial.println(" Ignition sequence activated ");

    for (int i=0; i<=50; i++){
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      delay(100);
    }
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rocket dyno");
    lcd.setCursor(1, 1);
    lcd.print("Acquiring data");
    digitalWrite(igniterPin, HIGH);
    Serial.print("Start time, ms: ");
    Serial.print(millis());
    Serial.println("");
    Serial.println();

    for (int i=0; i<= 800; i++){
      scale.set_scale(calibration_factor);
      Serial.print(scale.get_units(), 1);
      Serial.println();
    }
    Serial.println();
    Serial.print("Stop Time, ms: ");
    Serial.print(millis());
    digitalWrite(ledPin, LOW);
    digitalWrite(igniterPin, LOW);
    Serial.println();
  }
  
}
