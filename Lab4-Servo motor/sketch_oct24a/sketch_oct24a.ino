#include <Servo.h>
#include <Keypad.h>

//Setting Keypad configs
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'7','8','9','/'},
  {'4','5','6','*'},
  {'1','2','3','-'},
  {'O','0','=','+'}
};
byte rowPins[ROWS] = {23, 25, 27, 29}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {31,33, 35,37}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//Servo
Servo servo;  // create servo object to control a servo

int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

void setup() {
  servo.attach(9,1,2);  // attaches the servo on pin 9 to the servo object
 Serial.begin(9600);
}
void rotate0to180(){
  static int degree = 0;
  static int sign = 1;
  
  servo.write(degree);
  degree = degree + sign;
  if(degree == 180 || degree == 0)
    sign *= -1;
    
  Serial.println(degree);
  delay(10);
}
bool isKeyNumber(char key){
  int keyAsciiCode = (int)key;
  return keyAsciiCode >= (int)'0' && keyAsciiCode <= (int)'9';
}
void handleRotateByKeypad(){
  char key = keypad.getKey();  
  static String number = "";
  if (key){
    if(isKeyNumber(key)){
        number += key;
    }
    else if(key == '='){
      
      int value = map(number.toInt(), 0, 180,-90,90);
      Serial.print(number + " => ");
      Serial.println(value);
      servo.write(number.toInt());
      number = "";
    }
  }
}
void handleRotateByTerminal(){
    int data = Serial.read();
    static int number = 0;
    if(data == (int)'=')
    {
        Serial.println("\n Degree:");
        Serial.println(number);
        servo.write(number);
        number = 0;
    }else if(isKeyNumber((char)data)){
        number = number*10 + (data - (int)'0');
        Serial.print((char)data);
    }
}
void handleRotateByPotentiometer(){
    Serial.print("Potentiometer: ");
    val = analogRead(A0);            // reads the value of the potentiometer (value between 0 and 1023)
    val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
    Serial.println(val);
    servo.write(val);
    delay(1000);
}
void loop() {
//  rotate0to180();
//  handleRotateByKeypad();
//  handleRotateByTerminal();
  handleRotateByPotentiometer();
}
