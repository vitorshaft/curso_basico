//https://github.com/vitorshaft/roboMovel/raspMovelDC.ino
#include <SoftwareSerial.h> //biblioteca para configurar a comunicacao Serial
#include <EEPROM.h> //biblioteca para armazenar dados na memória ROM
#include <Coordinates.h>  //biblioteca para converter coordenadas

SoftwareSerial BT(2,3);  //2 = RX e 3 = TX

int mA1 = 9,mA2 = 10,mB1 = 11,mB2 = 12;

void posicao(int x, int y, int a){
  EEPROM.write(0,x);
  EEPROM.write(1,y);
  EEPROM.write(2,a);
}

/*
int lerPosicao(){
  int xis = EEPROM.read(0);
  int ips = EEPROM.read(1);
  int alfa = EEPROM.read(2);
  
  int pos[3]{xis,ips,alfa};
  
  return pos; //funcoes nao podem retornar arrays
}
*/

void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()){
    int comando = Serial.read();
  }
}
