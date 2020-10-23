#include <LiquidCrystal.h>


const int rs = 13, en = 12, d4 = 8, d5 = 9, d6 = 10, d7 =11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int LCD_WIDTH = 16;
const int LCD_HEIGHT = 2;

const String name = "Ebiiiiii";
void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  //lcd.cursor();
  lcd.print(name);
}

int cursorRow=0;
int cursorColumn=name.length();

int lastTime=0;
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
void loop() {
  if( millis()/1000 > lastTime){
    lastTime = millis()/1000;
    shift();  
  }
//  shift();
  //lcd.print(millis() / 1000);
}
