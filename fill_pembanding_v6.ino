#include "HX711.h"
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int LOADCELL_DOUT_PIN = 11;
const int LOADCELL_SCK_PIN = 12;
const int RELAY_PIN = 13; 

HX711 scale;
#define CALIBRATION_FACTOR 99.603
float volume600ml = 600; // Example value, adjust based on your actual volume measurement
float volume1000ml = 1000;
float volume5000ml = 5000;
String eksekusi = ""; // Membuat variabel string kosong

//BAGIAN  KEYPAD 4x4
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

char penampung = 0 ; //harusnya diganti float ? char variabel seperti apaa? 
int weight ;

void setup(){
  Serial.begin(57600);
  lcd.begin();
  lcd.setCursor(0, 1);
  lcd.print("FILLING  TESTING");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(CALIBRATION_FACTOR); // Atur faktor kalibrasi
  scale.tare(); // Reset scale ke 0
  digitalWrite(RELAY_PIN, LOW);
  pinMode(RELAY_PIN, OUTPUT);
  Serial.println("FILLING TESTING");
  delay(3000);
  // lcd.clear();
  
}
  
void loop(){
  
  // lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Target:");
  lcd.print(penampung);
  lcd.setCursor(14, 0);
  lcd.print("ml");
  lcd.setCursor(0, 1);
  lcd.print("Status:");
  lcd.print(eksekusi);
  weight = round(scale.get_units()); // Dapatkan berat dengan mengambil rata-rata dari 10 pembacaan
  // float eksekusi= 0;  //harusnya diganti float
  int targetVolume = 0;// harusnya diganti float
  char customKey = customKeypad.getKey();
    // just print the pressed key
  if (customKey == 'A'){
    // Serial.println(volume600ml);
    // Serial.println("600A");
    // lcd.setCursor(9, 0);
    // lcd.print("600");
    // lcd.print("ml");
    penampung = volume600ml;
  }else if (customKey == 'B'){
    Serial.println(volume1000ml);
    // lcd.setCursor(9, 0);
    // lcd.print(volume1000ml);
    // lcd.print("ml");
    penampung = volume1000ml;
  }else if (customKey == 'C'){
    Serial.println(volume5000ml);
    // lcd.setCursor(9, 0);
    // lcd.print(volume5000ml);
    // lcd.print("ml");
    penampung = volume5000ml;
  }else if (customKey != NO_KEY) {
    // Add the key to the input
    targetVolume += customKey - 0; // Assuming '0' to '9' are the only keys for manual input
    Serial.println(targetVolume);
    // lcd.setCursor(9, 0);
    // lcd.print(targetVolume);
    // lcd.print(" ml");
    // penampung = targetVolume;
    
    // lcd.clear();
    // lcd.print("Volume: ");
    // lcd.print(targetVolume);
  }else if (customKey == '*') {
    // pengisi();
      if (penampung == 600){
        eksekusi =  "mengisi";
        digitalWrite(RELAY_PIN, HIGH);
        delay(5000);
        penampung = 0;
        if (weight == volume600ml){
          digitalWrite(RELAY_PIN, LOW);
          eksekusi =  "selesai";}
      }else if (penampung == 1000){
        eksekusi =  "mengisi";
        digitalWrite(RELAY_PIN, HIGH);
        if (weight == volume1000ml){
          digitalWrite(RELAY_PIN, LOW);
          eksekusi =  "selesai";}
          penampung = 0;
      }else if (penampung == 5000){
        eksekusi =  "mengisi";
        digitalWrite(RELAY_PIN, HIGH); 
        if (weight == volume5000ml){
          digitalWrite(RELAY_PIN, LOW);
          eksekusi =  "selesai";}
          penampung = 0;
      }else if (penampung < 600.00 && penampung > 1.00){
        eksekusi =  "mengisi";
        digitalWrite(RELAY_PIN, HIGH);
        delay(2000);
        if (weight == penampung){
          digitalWrite(RELAY_PIN, LOW);
          eksekusi =  "selesai";}
          penampung = 0;
      }else{
      penampung = 0;
      }
  }else if (customKey == '#'){
    Serial.println("KOSONG");
    penampung = 0;
    weight=0;
    scale.tare();
    lcd.clear();
  }
  // delay(500);
  Serial.println(weight);
  Serial.println(penampung);
}

// void pengisi() {
//   if (penampung == 600.00){
//     eksekusi =  "mengisi";
//     digitalWrite(RELAY_PIN, HIGH);
//     delay(5000);
//     if (weight == volume600ml){
//       digitalWrite(RELAY_PIN, LOW);
//       eksekusi =  "selesai";
//     }
//   } else if (penampung == 1000.00){
//     eksekusi =  "mengisi";
//     digitalWrite(RELAY_PIN, HIGH);
//     if (weight == volume1000ml){
//       digitalWrite(RELAY_PIN, LOW);
//       eksekusi =  "selesai";
//     }
//   } else if (penampung == 5000.00){
//     eksekusi =  "mengisi";
//     digitalWrite(RELAY_PIN, HIGH); 
//     if (weight == volume5000ml){
//       digitalWrite(RELAY_PIN, LOW);
//       eksekusi =  "selesai";
//     }
//   }else if (penampung < 600.00 && penampung > 1.00){
//     eksekusi =  "mengisi";
//     digitalWrite(RELAY_PIN, HIGH);
//     delay(2000);
//     if (weight == penampung){
//       digitalWrite(RELAY_PIN, LOW);
//       lcd.setCursor(9, 1);
//       eksekusi =  "selesai";
//     }else{
//       penampung = 0;
//     }
//   }
//if (key != NO_KEY) {
// }

