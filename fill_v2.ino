/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"

LiquidCrystal_I2C lcd(0x27, 16, 2); // Inisialisasi LCD 16x2
HX711 scale;
#define CALIBRATION_FACTOR 99.603

const int RELAY_PIN = 13;
const int LOADCELL_DOUT_PIN = 11;
const int LOADCELL_SCK_PIN = 12;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

int targetVolume = 0;
// String eksekusi ;
bool isFilling = false;
int weight ;

void setup(){
  lcd.begin();
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(CALIBRATION_FACTOR); 
  scale.tare();
}

void loop(){
  char customKey = customKeypad.getKey();
  weight = round(scale.get_units());

if(customKey == 'D'){ 
  targetVolume = 0;
  isFilling = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("     START    ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Target:");
  lcd.print(targetVolume);
  lcd.setCursor(0, 1);
  lcd.print("Status:   none ");
  // lcd.print(eksekusi);
  }else if(customKey == '*'){
    isFilling = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Target:");
    lcd.print(targetVolume);
    lcd.setCursor(0, 1);
    lcd.print("Status: mengisi");

  } else if(customKey == '#'){
    // eksekusi ='mengisi';
    lcd.clear();
    targetVolume = 0;
    lcd.print("Target:");
    lcd.print(targetVolume);
    lcd.setCursor(0, 1);
    lcd.print("Status:  none ");
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println(customKey);
    delay(1000);
    digitalWrite(RELAY_PIN, LOW);
  } else if (customKey == 'A') {
    targetVolume = 600;
    lcd.setCursor(0, 0);
    lcd.print("Target: 600 ml");

  }else if (customKey == 'B') {
    targetVolume = 1000;
    lcd.setCursor(0, 0);
    lcd.print("Target: 1000 ml");

  } else if (customKey == 'C') {
    targetVolume = 5000;
    lcd.setCursor(0, 0);
    lcd.print("Target: 5000 ml");

  }else if (isdigit(customKey)) {
      int digit = customKey - '0';
      targetVolume = targetVolume * 10 + digit;
      lcd.setCursor(0, 0);
      lcd.print("Target: ");
      lcd.print(targetVolume);
  }
  if (isFilling) {
     // Baca berat dari load cell
    if (weight >= targetVolume) {
      digitalWrite(RELAY_PIN, LOW); // Matikan relay
      isFilling = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Target:");
      lcd.print(targetVolume);
      lcd.setCursor(0, 1);
      lcd.print("Status: SELESAI");
    }
  }
Serial.println(targetVolume);
Serial.println(weight);
}
