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
}

void loop() {
  double xis;
  double ips;
  int i = 0;
  /*
  while(!Serial.available()){
    
  }
  */
  /*
  digitalWrite(mA1, HIGH);
  digitalWrite(mA2, LOW);
  digitalWrite(mB1, HIGH);
  digitalWrite(mB2, LOW);
  delay(5000);

  digitalWrite(mA1, LOW);
  digitalWrite(mA2, LOW);
  digitalWrite(mB1, LOW);
  digitalWrite(mB2, LOW);
  delay(2000);
  */
  Coord coord;
  while(Serial.available()!=0){
    /*
    xis = Serial.parseInt();
    a[i] = xis;
    //Serial.println(a[i]);
    i++;
    for(int j = 0; j<sizeof(a); j++){
      Serial.print(a[j]);
    }
    //ips = Serial.parseInt();
    //Serial.println(ips);
    */
    coord.x = Serial.parseInt(SKIP_WHITESPACE);
    coord.y = Serial.parseInt(SKIP_WHITESPACE);
    Serial.print("X: ");
    Serial.println(coord.x);
    Serial.print("Y: ");
    Serial.println(coord.y);
    Serial.println(i);
    i++;
    
  }
  //delay(2000);
  /*
  if(Serial.available()!=0){
    ips = Serial.parseInt();
    Serial.println(ips);
  }
  */
}
