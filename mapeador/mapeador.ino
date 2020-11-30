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

int dist;
int medirDist(){  //Le a distancia pelo sensor ultrassonico e retorna em cm
  long duracao;
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
  //return dist;
}

int alfa;
void girar(int ang){
  sonar.write(ang);
  delay(30);
}

void encEsq(){
  contE++;
  //Serial.println(contE);
}
void encDir(){
  contD++;
}


void motores(int a,int b,int c,int d){  
//funcao para facilitar controle dos motores de forma indefinida
  analogWrite(mA1, a);
  analogWrite(mA2, b);
  analogWrite(mB1, c);
  analogWrite(mB2, d);
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
  /*
  if(alfa < 160){
    girar(alfa);
    alfa+=30;
  }
  else{
    alfa = 0;
    girar(alfa);
  }
  */
  medirDist();
  /*
  if(dist > 20){
    motores(150,0,150,0);
  }
  else{
    motores(0,0,0,0);
  }
  */
  Serial.print("Dist/ang: ");
  Serial.print(dist);
  Serial.print(", ");
  Serial.println(alfa);
  delay(500);
}
