/* @file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
*/
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'7','8','9','/'},
  {'4','5','6','*'},
  {'1','2','3','-'},
  {'O','0','=','+'}
};
byte rowPins[ROWS] = {23, 25, 27, 29}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {31,33, 35,37}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

static const uint8_t analog_pins[] = {A0,A1,A2,A3,A4,A5,A6,A7,A8};
int arrLength = sizeof(analog_pins) / sizeof(analog_pins[0]);
void setup(){
  setupLEDPins();
  
  Serial.begin(9600);
  Serial.println("Begin:");
}

void setupLEDPins(){
  for (int i = 0; i < arrLength; i++) {
    pinMode(analog_pins[i], OUTPUT);
  }  
}

void turnLEDOn(int keyNum){
  keyNum --;
  for (int i = 0; i < arrLength; i++) {
    if(i != keyNum)
      digitalWrite(analog_pins[i], LOW);
    else
      digitalWrite(analog_pins[i], HIGH);
  }    
}
void turnAllLEDsOff(){
  for(int i=0; i< arrLength; i++)
    digitalWrite(analog_pins[i], LOW);
}
bool isKeyNumber(char key){
  int keyAsciiCode = (int)key;
  return keyAsciiCode >= (int)'1' && keyAsciiCode <= (int)'9';
}
void loop(){
  char key = keypad.getKey();
  
  
  
  if (key){
    Serial.println(key);
    if(isKeyNumber(key)){
      int keyNum = (int) key;
      keyNum -= (int)'0';
      turnLEDOn(keyNum);
    }else if(key == 'O')
      turnAllLEDsOff();
    
  }
}
