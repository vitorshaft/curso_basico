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
//libs do Ivan Seidel
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
Thread mapear;
int scan[7] = {100,100,100,100,100,100,100};
int dist;
int medirDist(){  //Le a distancia pelo sensor ultrassonico e retorna em cm
  long duracao;
  //int dist;
  // Inicia o trig desligado
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  // Aciona o trig por 10 microssegundos
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Le o echo e retorna o tempo de viagem da onda de ultrassom
  duracao = pulseIn(echo, HIGH);
  // Calculo da dsitancia
  dist= duracao*0.034/2;
  // Limitamos a leitura a 1m (100 cm)
  if(dist > 100){
    dist = 100;
  }
  //retorna a distancia em cm
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
    scan[azim/30] = d;
    delay(150);
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
  
  int plot[2] = {azim,d};
  return plot;
  */
}

void encEsq(){
  contE++;
  //Serial.println(contE);
}
void encDir(){
  contD++;
}
Thread andar;
void varrer(){
  //Seguir em frente (1m) enquanto a distancia no azim 90° não detectar nada em menos de 20 cm
  if(scan[3] > 20){
    frente(100);
  }
  //Quando obstaculo detectado a menos de 20 cm:
  else{
    //Checar se direita (0°) bloqueada e girar a esquerda
    if(scan[0] < 20){
      esqAx(90);
    }
    //Se direita livre, girar 90° direita
    else{
      dirAx(90);
    }
  }
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
  //zera os encoders
  contE = 0;
  contD = 0;
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

  mapear.setInterval(1500);
  mapear.onRun(girarSonar);
  andar.setInterval(1000);
  andar.onRun(varrer);
}

void loop() {
  if(mapear.shouldRun()){
    mapear.run();
  }
  if(andar.shouldRun()){
    andar.run();
  }
  //Serial.println(medirDist());
  //medirDist();
  //int azim = 0;
  //delay(500);
  /*
  girarSonar();
  sonar.write(80);
  delay(1000);
  if(medirDist() > 20){
    Serial.println(medirDist());
    frente(100);
  }
  */
}
