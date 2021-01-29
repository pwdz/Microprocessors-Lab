#include <LiquidCrystal.h>
#include <Wire.h>

#define DEVICE_ADDRESS 0b1010000
#define MODE_MEMORY_ADDR 100

#define BUZ 49

//LCD
const int rs = 27, en = 28, d4 = 29, d5 = 30, d6 = 31, d7 =32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int LCD_WIDTH = 16;
const int LCD_HEIGHT = 2;


//Q length Between 0 and 16
String Q = "Salam or Hi?";
//AS1&ANS2 length Between 0 and 6
String ANS1 = "Salam";
String ANS2 = "Hi";

String v[4] = {"", "", "", ""};
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);

  Wire.begin();
  
  lcd.begin(16, 2);
  lcd.clear();

  pinMode(BUZ, OUTPUT);
  loadVotes();
  broadcastQandA();
}
void loadVotes(){
  uint8_t savedData[4];
  eepromRead(MODE_MEMORY_ADDR, savedData, 4);

  int vote1 = 0, vote2 =0;
  for(int i=0; i<4; i++){
      if(savedData[i] == '1'){
        vote1++;
        v[i] += savedData[i];
      }
      else if(savedData[i] <= '2'){
          v[i] += savedData[i];
          vote2++;
      }
  }
  updateLCD("Total Votes:", 0);
  lcd.print(String(vote1+vote2));
  updateLCD(ANS1 + ":", 1);
  lcd.print(String(vote1));
  lcd.print(" "+ANS2+":");
  lcd.print(String(vote2));
}
void setQandAs(char QandAs[]){
  int index =0;
  for(int i=0; i<Q.length(); i++){
    QandAs[index] = Q[i];
    index++;
  }
  QandAs[index]='\n';
  index++;

  for(int i=0; i<ANS1.length(); i++){
    QandAs[index] = ANS1[i];
    index++;
  }
  QandAs[index]='\n';
  index++;
  
  for(int i=0; i<ANS2.length(); i++){
    QandAs[index] = ANS2[i];
    index++;
  }
  QandAs[index]='\n';
  index++;
}
void broadcastQandA(){
  char QandAs[Q.length() + ANS1.length() + ANS2.length() + 3];
  setQandAs(QandAs);
  char votedMsg[] = {'U','\'','v','e',' ','v','o','t','e','d','\n',' ','\n',' ','\n'};

  delay(50);

  if(v[1] == "")
    Serial1.write(QandAs);  
  else
    Serial1.write(votedMsg);  
  Serial1.flush();
  delay(10);
  
  
  if(v[2] == "")
    Serial2.write(QandAs);  
  else
    Serial2.write(votedMsg);  
  Serial2.flush();
  delay(10);

  if(v[3] == "")
    Serial3.write(QandAs);  
  else
    Serial3.write(votedMsg);  
  Serial3.flush();
  delay(10);

  if(v[0] == "")
    Serial.write(QandAs);  
  else
    Serial.write(votedMsg);  
  Serial.flush();
  delay(10);
}
void updateLCD(String s, int row){
  lcd.setCursor(0, row);
  lcd.print("                ");
  lcd.setCursor(0, row);
  lcd.print(s);
}
void eepromWrite(uint8_t memory_address, uint8_t* data, int _size){
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(memory_address);
  
  for(int i=0; i<_size; i++){
    Wire.write(data[i]);
  }
  
  Wire.endTransmission();
}

void eepromRead(uint8_t memory_address, uint8_t *data, uint8_t _size){
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(memory_address);
  Wire.endTransmission();
  
  Wire.requestFrom(DEVICE_ADDRESS, _size);
  for(int i=0; i<_size; i++){
    data[i] = Wire.read();
  }
}
void loop() {
  if(Serial.available() && v[0] == ""){
    char ch = (char) Serial.read();
    v[0] += ch;
    updateLCD("Device4:", 0);
    updateLCD(v[0], 1);

    uint8_t tempData[1] = {ch};
    eepromWrite(MODE_MEMORY_ADDR, tempData ,1);
    tone(BUZ, 400);
  } 
  if(Serial1.available() && v[1] == ""){
    char ch1 = (char) Serial1.read();
    v[1] += ch1;
    updateLCD("Device1:", 0);
    updateLCD(v[1], 1);

    uint8_t tempData[1] = {ch1};
    eepromWrite(MODE_MEMORY_ADDR + 1, tempData ,1);
    tone(BUZ, 400);
  }
  if(Serial2.available() && v[2] == ""){
    char ch2 = (char) Serial2.read();
    v[2] += ch2;
    updateLCD("Device2:", 0);
    updateLCD(v[2], 1);

    uint8_t tempData[1] = {ch2};
    eepromWrite(MODE_MEMORY_ADDR + 2, tempData ,1);
    tone(BUZ, 400);
  }
  if(Serial3.available() && v[3] == ""){
    char ch3 = (char) Serial3.read();
    v[3] += ch3;
    updateLCD("Device3:", 0);
    updateLCD(v[3], 1);

    uint8_t tempData[1] = {ch3};
    eepromWrite(MODE_MEMORY_ADDR + 3, tempData ,1);
    tone(BUZ, 400);
  }
}
