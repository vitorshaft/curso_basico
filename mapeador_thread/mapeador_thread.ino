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
#include <SoftwareSerial.h> 
#include <Servo.h>
#include <Coordinates.h>

SoftwareSerial bt(5,6); //RX e TX
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
  Serial.println(dist);
  //retorna a distancia em cm
  //return dist;
}
int pos = 0;
void girar(){
  if(pos <= 180){
    sonar.write(pos);
    pos+=5;
  }
  else{
    pos = 0;
    sonar.write(pos);
  }
}


void encEsq(){
  contE++;
  //Serial.println(contE);
}
void encDir(){
  contD++;
}
Thread andar;
void mover(){
  //Seguir em frente (1m) enquanto a distancia no azim 90° não detectar nada em menos de 20 cm
  if(scan[3] > 20){
    while(contE < 80 || contD < 80){
      motores(150,0,150,0);
    }
    motores(0,0,0,0);
  }
  //Quando obstaculo detectado a menos de 20 cm:
  else{
    //Checar se direita (0°) bloqueada e girar a esquerda
    if(scan[0] < 20){
      while(contE < 80 || contD < 80){
      motores(150,0,0,150); //esquerda, duas voltas de roda
    }
    motores(0,0,0,0);
    }
    //Se direita livre, girar 90° direita
    else{
      while(contE < 80 || contD < 80){
      motores(0,150,150,0);
    }
    motores(0,0,0,0);
    }
  }
}
void motores(int a,int b,int c,int d){  
//funcao para facilitar controle dos motores de forma indefinida
  //zera os encoders
  contE = 0;
  contD = 0;
  /*
  digitalWrite(mA1, a);
  digitalWrite(mA2, b);
  digitalWrite(mB1, c);
  digitalWrite(mB2, d);
  */
  analogWrite(mA1, a);
  analogWrite(mA2, b);
  analogWrite(mB1, c);
  analogWrite(mB2, d);
}

Thread plotar;
int xis,ips = 0;
Coordinates ponto = Coordinates();
void cart(){
  ponto.fromPolar(dist,pos);
  xis = ponto.getX();
  ips = ponto.getY();
}
void tx(){
  bt.print(xis);
  bt.print(ips);
  Serial.print(xis);
  Serial.println(ips);
}

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  sonar.attach(4);
  Serial.begin(9600);
  //bt.begin(9600);

  attachInterrupt(digitalPinToInterrupt(2),encEsq,CHANGE);
  attachInterrupt(digitalPinToInterrupt(3),encDir,CHANGE);

  pinMode(mA1,OUTPUT);
  pinMode(mA2,OUTPUT);
  pinMode(mB1,OUTPUT);
  pinMode(mB2,OUTPUT);

  mapear.setInterval(500);
  mapear.onRun(girar);
  mapear.onRun(medirDist);
  
  plotar.setInterval(500);
  plotar.onRun(cart);
  //plotar.onRun(tx);
  
  //andar.setInterval(1000);
  //andar.onRun(mover);
}

void loop() {
  /* 1. Protoboard/jumpers com defeito
   * a) Usar varredura com motores das rodas
   * b) só retornar leituras válidas
   */
  
  if(mapear.shouldRun()){
    mapear.run();
    //mapear.runned();
  }
  if(plotar.shouldRun()){
    plotar.run();
    //plotar.runned();
  }
  /*if(andar.shouldRun()){
    andar.run();
  }
  */
  motores(0,0,0,0);
  //Serial.println(dist);
}
