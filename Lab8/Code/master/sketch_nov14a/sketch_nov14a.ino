#include <SPI.h>

#define SS 48
#define SS1 49

byte HELLO_WORLD[12] = {'h','e','l','l','o',' ','w','o','r','l','d'};
byte HI[2] = {'H','i'};

bool sendHello = false;
bool sendHelloHi = false;
bool sendTempLightBool = true;

void setup() {
  pinMode(SS, OUTPUT);
  pinMode(SS1, OUTPUT);
  digitalWrite(SS, HIGH);
  digitalWrite(SS1, HIGH);
  
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV4);

  if(sendHello)
    digitalWrite(SS, LOW);//Enable Slave0

  Serial.begin(9600);
  Serial.println("This is master");

//  if(sendHelloHi || sendHello){
//    String("Hello world!").getBytes(HELLO_WORLD, sizeof(HELLO_WORLD))?;
//    String("Hi").getBytes(HI, sizeof(HI));
//  }
}
void printHelloWorld(){
//  SPI.transfer(HELLO_WORLD, 12);
//  Serial.println(String((char*)HELLO_WORLD));
  for(int i=0; i<12; i++){
    SPI.transfer(HELLO_WORLD[i]);
    Serial.print((char)HELLO_WORLD[i]);
    delay(10); 
  }
  Serial.println("");
  
  delay(1000);
}
void printHiHello(){
  Serial.println("Sending>>HELLOWWORLD");
  digitalWrite(SS, LOW);//Enable Slave0
  delay(50);
//  SPI.transfer(HELLO_WORLD, sizeof(HELLO_WORLD));
  for(int i=0; i<12; i++){
    SPI.transfer(HELLO_WORLD[i]);
    Serial.print((char)HELLO_WORLD[i]);
    delay(10); 
  }
  delay(50);
  digitalWrite(SS, HIGH);//Enable Slave0
  delay(500);

  Serial.println("Sending>>HI");
  digitalWrite(SS1, LOW);//Enable Slave0
  delay(50);
//  SPI.transfer(HI, sizeof(HI));
  for(int i=0; i<2; i++){
    SPI.transfer(HI[i]);
    Serial.print((char)HI[i]);
    delay(10); 
  }
  delay(50);
  digitalWrite(SS1, HIGH);//Enable Slave0
  delay(500);

}

void sendTempLight(){
  digitalWrite(SS, LOW);//Enable Slave0
  delay(50);
//  SPI.transfer(HELLO_WORLD, sizeof(HELLO_WORLD));
  float temp = analogRead(A0);
  temp *= 0.48828125;
  Serial.println("Current temperature: [" + String(temp)+"*C");
  byte tempTemperature[2];
  tempTemperature[0] = (int)temp;
  tempTemperature[1] = (int)temp >> 8; 


  
  for(int i=0; i<2; i++){
    SPI.transfer(tempTemperature[i]);
//    Serial.print((char)HELLO_WORLD[i]);
    delay(10); 
  }
  delay(50);
  digitalWrite(SS, HIGH);//Enable Slave0
  delay(500);

//  Serial.println("Sending>>HI");
  digitalWrite(SS1, LOW);//Enable Slave0
  delay(50);
//  SPI.transfer(HI, sizeof(HI));

  int analogLight = analogRead(A1) ;
//  analogLight = map(analogLight, 0, 1023, 0, 990);
  byte tempLight[2];
  tempLight[0] = analogLight;
  tempLight[1] = analogLight >> 8;
  Serial.println("Current Light: " + String(analogLight));
  for(int i=0; i<2; i++){
    SPI.transfer(tempLight[i]);
//    Serial.print((char)HI[i]);
    delay(10); 
  }
  delay(50);
  digitalWrite(SS1, HIGH);//Enable Slave0
  delay(500);

}
void loop() {
  if(sendHello) 
    printHelloWorld();
  else if(sendHelloHi)
    printHiHello();
  else
    sendTempLight();  
//  printHiHello();


  
}
