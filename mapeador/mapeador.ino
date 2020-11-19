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
  for(azim = 0; azim <= 180; azim+=10){
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
}

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  sonar.attach(5);
  Serial.begin(9600);
  

}

void loop() {
  //Serial.println(medirDist());
  //medirDist();
  
  //int azim = 0;
  
  for(int azim = 0; azim <= 180; azim+=10){
    sonar.write(azim);
    delay(30);
    d = medirDist();
    delay(500);
    Serial.print("Angulo: ");
    Serial.println(azim);
    Serial.print("Distancia: ");
    Serial.println(d);
  }
}
