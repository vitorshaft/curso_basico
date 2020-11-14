#include <EEPROM.h>

//pinos dos motores A e B
int mA1 = 9;
int mA2 = 10;
int mB1 = 11;
int mB2 = 12;
int a[] = {};
struct Coord{
  double x;
  double y;
};
//pinos dos sensores dos encoders
#define esq 3
#define dir 2

float vEsq = 0.00;
float vDir = 0.00;

void encEsq(){
  vEsq+=0.01;
}
void encDir(){
  vDir+=0.01;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(mA1, OUTPUT);
  pinMode(mA2, OUTPUT);
  pinMode(mB1, OUTPUT);
  pinMode(mB2, OUTPUT);

  digitalWrite(mA1, LOW);
  digitalWrite(mA2, LOW);
  digitalWrite(mB1, LOW);
  digitalWrite(mB2, LOW);
  attachInterrupt(digitalPinToInterrupt(2),encDir,CHANGE);
  attachInterrupt(digitalPinToInterrupt(3),encEsq,CHANGE);
  
}

void loop() {
  while(vEsq < 2.00){
    digitalWrite(mA1, HIGH);
    digitalWrite(mA2, LOW);
    digitalWrite(mB1, HIGH);
    digitalWrite(mB2, LOW);
    Serial.println(vEsq);
  }
  digitalWrite(mA1, LOW);
  digitalWrite(mA2, LOW);
  digitalWrite(mB1, LOW);
  digitalWrite(mB2, LOW);
  delay(3000);
  
}
