#include <EEPROM.h>

//pinos dos motores A e B
int mA1 = 9,mA2 = 10,mB1 = 11,mB2 = 12;
//pinos dos sensores dos encoders
#define irEsq 2
#define irDir 3

struct Encoder{
  float encEsq;
  float encDir;
};

struct Posicao{
  float vX;
  float vY;
  float vA;
};

void parar(){
  digitalWrite(mA1, LOW);
  digitalWrite(mA2, LOW);
  digitalWrite(mB1, LOW);
  digitalWrite(mB2, LOW);
}

void frente(){
  digitalWrite(mA1, HIGH);
  digitalWrite(mA2, LOW);
  digitalWrite(mB1, LOW);
  digitalWrite(mB2, HIGH);
}

void esquerda(){
  digitalWrite(mA1, HIGH);
  digitalWrite(mA2, LOW);
  digitalWrite(mB1, HIGH);
  digitalWrite(mB2, LOW);  
}

void encEsq(){
  Encoder encoder;
  EEPROM.get(3,encoder);
  //encoder.encEsq+=1.00;
  encoder.encEsq+= 0.01;
  Serial.println(encoder.encEsq);
  EEPROM.put(3,encoder);
}
void encDir(){
  Encoder encoder;
  EEPROM.get(3,encoder);
  //encoder.encEsq+=1.00;
  encoder.encDir+= 0.01;
  EEPROM.put(3,encoder);
}

void setup() {
  Serial.begin(9600);

  pinMode(irDir, INPUT);
  pinMode(irEsq, INPUT);
  
  pinMode(mA1, OUTPUT);
  pinMode(mA2, OUTPUT);
  pinMode(mB1, OUTPUT);
  pinMode(mB2, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(2),encEsq,CHANGE);
  attachInterrupt(digitalPinToInterrupt(3),encDir,CHANGE);

}

void loop() {
  frente();
  delay(5000);
  parar();
  delay(2000);

}
