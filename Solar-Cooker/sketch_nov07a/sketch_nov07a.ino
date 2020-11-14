#include <Servo.h>
#include <Keypad.h>

#define COOKER_OPEN 90
#define COOKER_CLOSED 0
#define BUZZERPIN 49

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

//Defining Timer class
class Timer{
  public: 
  int startTime;
  int timeLimit = -1;
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
    Serial.println("Passed Time:"+ String(passedTime));
    return (millis()/1000 - startTime) > timeLimit;  
  }  
};

int requiredTemperature = -1;
Timer timer;
void setup() {
  pinMode(BUZZERPIN, OUTPUT);
  setupServo();
  Serial.begin(9600);
  Serial.println("Please Enter Time(in Seconds) & requiredTemperature(in Degrees) in Keypad");
}
void setupServo(){
  servo.attach(9,1000,2000);  // attaches the servo on pin 9 to the servo object
  servo.write(COOKER_OPEN);  
}
bool isKeyNumber(char key){
  int keyAsciiCode = (int)key;
  return keyAsciiCode >= (int)'0' && keyAsciiCode <= (int)'9';
}
void rotateServo(int degree){
    Serial.print("Servo new Degree: ");
    Serial.println(degree);
    servo.write(degree);
}
float readAnalogTemperature(){
  float temp = analogRead(A0);
  temp *= 0.48828125;
  Serial.println("Current temperature: [" + String(temp)+"*C] Required Tempreture: ["+String(requiredTemperature)+"*C]");
  return temp;
}
bool isTemperatureEnough(){
  delay(300);
  return readAnalogTemperature() >= requiredTemperature;
}
bool shouldReadKeypad(){
    return requiredTemperature == -1 || timer.getTimeLimit() == -1;
}
void readKeypad(){
  char key = keypad.getKey();  
  static String number = "";
  
  if(key){
    if(isKeyNumber(key)){
        number += key;
    }
    else if(key == '='){
      if(timer.getTimeLimit() == -1){//User entered time
        Serial.println("Time: " + number);
        timer.setTimeLimit(number.toInt());  
      }else{//User entered requiredTemperature
        Serial.println("Required Temperature: " + number);
        requiredTemperature = number.toInt();
        Serial.println("Closing Cooker. Start cooking...");
        rotateServo(COOKER_CLOSED);
      }
      number = "";
    }
  }  
}
void loop() {
  static bool isTimerStarted = false;
  static bool isCookingFinished = false;
  if(shouldReadKeypad())
    readKeypad();
  else{
    if(isTimerStarted){
      if(!isCookingFinished && timer.isTimeFinished()){
        rotateServo(COOKER_OPEN);  
        isCookingFinished = true;
        Serial.println(">>>>>>>>>Cooking finished<<<<<<<<<");
        tone(BUZZERPIN, 400);
        delay(4000);
        noTone(BUZZERPIN);
        //Buzzer
      }
    }else if(isTemperatureEnough()){
      Serial.println("Start Timer. Required Time:["+String(timer.getTimeLimit())+"s]");
      timer.start();
      isTimerStarted = true;  
    }
  }
}
