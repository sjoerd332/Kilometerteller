int clockPin = 3;
int dataPin = 2;

int loopCounter = 0;

inline void setBit(int state){
  if(state > 0) { digitalWrite(dataPin,HIGH); }
  else{ digitalWrite(dataPin,LOW); }
  digitalWrite(clockPin,HIGH);
  digitalWrite(clockPin,LOW);
}

inline void setByte(uint8_t state){
  for(int i = 0;i<8;i++){
    int writeVar = (state >> i) & 1;
    setBit(writeVar);
  }
}

void setNumber(int number){
  // order 7 segment pin numbers: 10,9,7,6,5,4,2,1
  if(number == 0)     {setByte(0b01111110);}
  else if(number == 1){setByte(0b01001000);}
  else if(number == 2){setByte(0b00111101);}
  else if(number == 3){setByte(0b01101101);}
  else if(number == 4){setByte(0b01001011);}
  else if(number == 5){setByte(0b01100111);}
  else if(number == 6){setByte(0b01110111);}
  else if(number == 7){setByte(0b01001100);}
  else if(number == 8){setByte(0b01111111);}
  else if(number == 9){setByte(0b01101111);}
  else if(number == 10){setByte(0b01011111);}// A
  else if(number == 11){setByte(0b01110011);}// B
  else if(number == 12){setByte(0b00110110);}// C
  else if(number == 13){setByte(0b01111001);}// D
  else if(number == 14){setByte(0b00110111);}// E
  else if(number == 15){setByte(0b00010111);}// F
  else if(number == 16){setByte(0b10000000);}// dot
  else{setByte(0b10000000);}// dot
}

void setup() {
  // put your setup code here, to run once:
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin, OUTPUT);
  digitalWrite(clockPin,LOW);
  digitalWrite(dataPin,LOW);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  setNumber(loopCounter % 10); // lower digit
  setNumber((loopCounter / 10) % 10); // upper digit
  Serial.println(loopCounter);
  loopCounter++;
  if(loopCounter > 99){loopCounter = 0;}
  delay(200);
}
