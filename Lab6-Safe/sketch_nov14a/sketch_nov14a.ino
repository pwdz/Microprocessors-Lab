#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

#define SAFE_OPEN 90
#define SAFE_CLOSED 0
#define LED_PIN 17
#define BUZZER_PIN 21

//Setting LCD configs
const int rs = 13, en = 12, d4 = 8, d5 = 9, d6 = 10, d7 =11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int LCD_WIDTH = 16;
const int LCD_HEIGHT = 2;



//Setting Keypad configs
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'7','8','9','/'},
  {'4','5','6','*'},
  {'1','2','3','-'},
  {'C','0','=','+'}
};
byte rowPins[ROWS] = {23, 25, 27, 29}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {31,33, 35,37}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//Servo
Servo servo;  // create servo object to control a servo

//Defining Timer class
class Timer{
  public: 
  int startTime;
  int timeLimit = -1;
  int lastTime;
  Timer(){
  }
  public:
  int getTimeLimit(){
    return timeLimit;  
  }
  void setTimeLimit(int seconds){
    timeLimit = seconds;
  }
  void start(){
    startTime = millis()/1000;  
  }  

  bool isTimeFinished(){
    int passedTime = millis()/1000 - startTime;
//    Serial.println("Passed Time:"+ String(passedTime));
    return (millis()/1000 - startTime) > (timeLimit-1);  
  }  
  int getRemainingTime(){
    return (timeLimit - (millis()/1000 - startTime));   
  }
  bool isSecondPassed(){
      int sec = millis()/1000;
      if(sec == ((millis()%1000)-sec)){
         return true;
      }
      return false;
  }
};

String password = "1234";
Timer timer;
int safeOpenTime = -1;
int safeCloseTime = -1;
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  timer.setTimeLimit(5);

  lcd.begin(16, 2);
  lcd.clear();
  
  setupServo();
//  Serial.begin(9600);
//  Serial.println("[#] Default Timer time: 5s");
  printLCD("CLOSED", 1);
}
void setupServo(){
  servo.attach(3,1000,2000);  // attaches the servo on pin 9 to the servo object
  servo.write(SAFE_CLOSED);  
  digitalWrite(LED_PIN, LOW);
}
//------------------------------------------------//
void printLCD(String in, int row){
    lcd.setCursor(0, row);
    lcd.print("                ");
    lcd.setCursor(0, row);
    lcd.print(in);  
}
bool isKeyNumber(char key){
  int keyAsciiCode = (int)key;
  return keyAsciiCode >= (int)'0' && keyAsciiCode <= (int)'9';
}
void rotateServo(int degree){
    servo.write(degree);
}
void checkPassword(String in, bool *isSafeOpen){
  if(in == password){
    printLCD("CORRECT PASS", 0);
    openSafe(isSafeOpen);
    tone(BUZZER_PIN, 400);
    safeOpenTime = millis();
  }else{
    printLCD("INCORRECT PASS", 0);
    if(!(*isSafeOpen))
      printLCD("CLOSED", 1);
  }
}
void checkTimer(bool *isSafeOpen){
  if(timer.isTimeFinished()){
    closeSafe(isSafeOpen);
  }else{
      if(timer.isSecondPassed()){
          printLCD("OPEN "+String(timer.getRemainingTime())+"s", 1);
      }
  }  
}
void closeSafe(bool *isSafeOpen){
  rotateServo(SAFE_CLOSED);  
  (*isSafeOpen) = false;
  digitalWrite(LED_PIN, LOW);
  printLCD("CLOSED", 1);
  tone(BUZZER_PIN, 400);
  safeCloseTime = millis();
}
void openSafe(bool *isSafeOpen){
  printLCD("OPEN "+String(timer.getTimeLimit())+"s", 1);
  rotateServo(SAFE_OPEN);//Opening servo
  digitalWrite(LED_PIN, HIGH);
  timer.start();
  (*isSafeOpen) = true;
}
void changePassword(String in){
  password = in;
  printLCD("PASSWORD CHANGED", 0);
}
void changeTimer(String in){
  timer.setTimeLimit(in.toInt());
  printLCD("TIME CHANGED: "+String(timer.getTimeLimit()), 0);
}
void readKeypad(){
  char key = keypad.getKey();  
  static String number = "";
  static bool isSafeOpen = false;
  
  if(key){
    if(isKeyNumber(key)){
        number += key;
    }
    else if(key == '='){//Check password
        checkPassword(number, &isSafeOpen);
        number = "";
    }else if(key == 'C'){//Close safe
        closeSafe(&isSafeOpen);
        number = "";
    }else if(key == '/'){//Set new password
        changePassword(number);
        number = "";
    }else if(key == '*'){//Set new Time
        changeTimer(number);
        number = "";
    }
  }

  if(isSafeOpen){
    if(millis()-safeOpenTime > 100){
      noTone(BUZZER_PIN);  
    }
    checkTimer(&isSafeOpen);  
  }else if(safeCloseTime != -1){
      if( millis() - safeCloseTime > 100){
        safeCloseTime = -1;
        noTone(BUZZER_PIN);  
      }
  }
}  
void loop() {
  readKeypad();
}
