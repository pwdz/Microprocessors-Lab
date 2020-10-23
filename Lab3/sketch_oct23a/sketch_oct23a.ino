#include <LiquidCrystal.h>
#include <Keypad.h>

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
  {'O','0','=','+'}
};
byte rowPins[ROWS] = {23, 25, 27, 29}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {31,33, 35,37}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const uint8_t analog_pins[] = {A0,A1,A2,A3,A4,A5,A6,A7,A8};
int arrLength = sizeof(analog_pins) / sizeof(analog_pins[0]);



//Shifting Name variables
const String name = "Ebiiiiii";
int cursorRow=0;
int cursorColumn=name.length();
int lastTime=0;

//password checking variables
const String PASSWORD = "1234";
String enteredPassword="";

//calculator variables
int numbers[2] = {0,0};
int index=0;
char operand;

//Flags
bool shiftProgram = false;
bool passwordProgram = true;
bool calculatorProgram = false;

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  if(shiftProgram)
    setupShiftingNameConfigs();

}
void setupShiftingNameConfigs(){
  lcd.print(name);
}
void shift(){
  
  if(cursorColumn < LCD_WIDTH){
    lcd.scrollDisplayRight();
    cursorColumn++;
  }else{
      cursorColumn =name.length();
      cursorRow = (cursorRow + 1) % LCD_HEIGHT;
      lcd.clear();
      lcd.setCursor(0, cursorRow);
      lcd.print(name);
  }

}

bool isKeyNumber(char key){
  int keyAsciiCode = (int)key;
  return keyAsciiCode >= (int)'1' && keyAsciiCode <= (int)'9';
}

void handlePassword(){
  char key = keypad.getKey();  
  if (key){
    if(isKeyNumber(key)){
      enteredPassword += key;
      lcd.write(key);
    }else if(key == '='){
      lcd.setCursor(0,1);
      if(PASSWORD == enteredPassword ){
          lcd.print("Correct pass");
      }else
        lcd.print("wrong pass");  
    }
  }
}
void calculate(){
  char key = keypad.getKey();  
  if (key){
    if(isKeyNumber(key)){
      numbers[index] = numbers[index]*10 + ((int) key -(int)'0');
      lcd.write(key);
    }
    else if(key == '='){
      lcd.setCursor(0,1);
      if(operand == '+'){
        lcd.print(numbers[0]+numbers[1]);
      }else if(operand == '-'){
        lcd.print(numbers[0]-numbers[1]);
      }else if(operand == '/'){
        lcd.print(numbers[0]/numbers[1]);
      }else if(operand == '*'){
         lcd.print(numbers[0]*numbers[1]);
      }
    }else{
      operand = key;
      index++;
      lcd.write(key);
    }
  }
}
void loop() {
  if(shiftProgram){
    if( millis()/1000 > lastTime){
      lastTime = millis()/1000;
      shift();  
    }
  }else if(passwordProgram){
    handlePassword();
  }else if(calculatorProgram){
    calculate();
  }
  
}
