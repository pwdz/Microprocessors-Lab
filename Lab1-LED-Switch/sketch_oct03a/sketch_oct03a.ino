
const long interval = 300;           // interval at which to blink (milliseconds)

void setup() {
  // set the digital pin as output:
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  pinMode(8, INPUT); //key1
  pinMode(9, INPUT);//key2
  pinMode(10, INPUT);//key3
}

void loop() {

    if(digitalRead(8) == LOW  ){
      digitalWrite(0, HIGH);
      delay(interval);

      digitalWrite(1, HIGH);
      delay(interval);

      digitalWrite(2, HIGH);
      delay(interval);

      digitalWrite(3, HIGH);
    }
    else if(digitalRead(9) == LOW){   
      digitalWrite(3, HIGH);
      delay(interval);

      digitalWrite(2, HIGH);
      delay(interval);

      digitalWrite(1, HIGH);
      delay(interval);

      digitalWrite(0, HIGH);
    }
    else if(digitalRead(10) == LOW){
      digitalWrite(2, LOW);
      digitalWrite(1, LOW);
      digitalWrite(0, LOW);        
      digitalWrite(3, LOW);        
    }    
  }
