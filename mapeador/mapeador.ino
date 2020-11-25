/*  https://github.com/vitorshaft
 *   https://instagram.com/shaftrobotica
 *   Sketch para Arduino em plataforma robótica com:
 *   - Motores DC com encoders (20 furos)
 *   - Sensor de distância ultrassom HC-SR04
 *   - Módulo Bluetooth HC-05
 *   - Ponte-H L9110
 */

 /* Funções:
  *  - Medir distância
  *  - girar sonar
  *  - Frente, trás, esquerda, direita
  *  - Odometria
  */

#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>
#include <Servo.h>
Servo sonar;
const int trig = 7;
const int echo = 8;
int cont = 0;
int d;

int contE = 0;
int contD = 0;

#define mA1 9
#define mA2 10
#define mB1 11
#define mB2 12

struct Radar{
  int az;
  int d;
};

int medirDist(){
  long duracao;
  int dist;
  // Clears the trigPin
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duracao = pulseIn(echo, HIGH);
  // Calculating the distance
  dist= duracao*0.034/2;
  // Prints the distance on the Serial Monitor
  if(dist > 100){
    dist = 100;
  }
  return dist;
}

void girarSonar(){
  //Radar radar;
  int azim = 0;
  int d;
  for(azim = 0; azim <= 180; azim+=30){
    sonar.write(azim);
    delay(30);
    d = medirDist();
    delay(500);
    Serial.print("Angulo: ");
    Serial.println(azim);
    Serial.print("Distancia: ");
    Serial.println(d);
  }
  /*
  for(azim = 180; azim >= 0; azim-=1){
    sonar.write(azim);
    cp[0] = medirDist();
    cp[1] = azim;
    delay(20);
  }
  */
  int plot[2] = {azim,d};
  return plot;
}

void encEsq(){
  contE++;
  //Serial.println(contE);
}
void encDir(){
  contD++;
}

void frente(int dist){ //distancia em cm
  contE = 0;
  contD = 0;
  int pulsos = dist/5;
  while(contE < pulsos || contD < pulsos){
    Serial.println(contE);
    digitalWrite(mA1, HIGH);
    digitalWrite(mA2, LOW);
    digitalWrite(mB1, HIGH);
    digitalWrite(mB2, LOW);
  }
  digitalWrite(mA1, LOW);
  digitalWrite(mA2, LOW);
  digitalWrite(mB1, LOW);
  digitalWrite(mB2, LOW);
}
void esqAx(int ang){
  contE = 0;
  contD = 0;
  int pulsos = ang/8; //cada pulso representa 8° de rotação axial
  while(contE < pulsos || contD < pulsos){
    digitalWrite(mA1, HIGH);
    digitalWrite(mA2, LOW);
    digitalWrite(mB1, LOW);
    digitalWrite(mB2, HIGH);
  }
  digitalWrite(mA1, LOW);
  digitalWrite(mA2, LOW);
  digitalWrite(mB1, LOW);
  digitalWrite(mB2, LOW); 
}
void dirAx(int ang){
  contE = 0;
  contD = 0;
  int pulsos = ang/8; //cada pulso representa 8° de rotação axial
  while(contE < pulsos || contD < pulsos){
    digitalWrite(mA1, LOW);
    digitalWrite(mA2, HIGH);
    digitalWrite(mB1, HIGH);
    digitalWrite(mB2, LOW);
  }
  digitalWrite(mA1, LOW);
  digitalWrite(mA2, LOW);
  digitalWrite(mB1, LOW);
  digitalWrite(mB2, LOW); 
}
void tras(int dist){
  contE = 0;
  contD = 0;
  int pulsos = dist/5;
  while(contE < pulsos || contD < pulsos){
    digitalWrite(mA1, LOW);
    digitalWrite(mA2, HIGH);
    digitalWrite(mB1, LOW);
    digitalWrite(mB2, HIGH);
  }
  digitalWrite(mA1, LOW);
  digitalWrite(mA2, LOW);
  digitalWrite(mB1, LOW);
  digitalWrite(mB2, LOW);
}
void motores(int a,int b,int c,int d){  
//funcao para facilitar controle dos motores de forma indefinida
  digitalWrite(mA1, a);
  digitalWrite(mA2, b);
  digitalWrite(mB1, c);
  digitalWrite(mB2, d);
}

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  sonar.attach(5);
  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(2),encEsq,CHANGE);
  attachInterrupt(digitalPinToInterrupt(3),encDir,CHANGE);

  pinMode(mA1,OUTPUT);
  pinMode(mA2,OUTPUT);
  pinMode(mB1,OUTPUT);
  pinMode(mB2,OUTPUT);
}

void loop() {
  //Serial.println(medirDist());
  //medirDist();
  //int azim = 0;
  //delay(500);
  girarSonar();
  sonar.write(80);
  delay(1000);
  if(medirDist() > 20){
    Serial.println(medirDist());
    frente(100);
  }
}
