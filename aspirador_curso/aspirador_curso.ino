//https://github.com/vitorshaft/curso_basico
#include <SoftwareSerial.h> //biblioteca para configurar a comunicacao Serial
#include <EEPROM.h> //biblioteca para armazenar dados na memória ROM
#include <Coordinates.h>  //biblioteca para converter coordenadas
#include <math.h>

// ***** REFATORAR TUDO USANDO EEPROM.GET() E EEPROM.PUT() *****

/*Funcoes:
 * - Receber coordenadas via BT;
 * - Iterar endereços EEPROM e armazenar X e Y (entre 5 e 1023)
 * - Ler posição atual
 * - Virar robô na direção dos pontos 
 * - Mover robô aos objetivos, um a um
 * - Monitorar deslocamento (Interrupt)
 */

 /*DIMENSÕES:
  * - Distância entre eixos = 150 mm;
  * - Diâmetro da roda = 65 mm;
  * - Perímetro axial = 471,2 mm;
  * - Perímetro radial = 942,48 mm;
  * - Perímetro da roda = 204,2 mm;
  * - Pulsos por rotação = 20;
  * - Ângulo por pulso = 18º;
  */

//instancia o objeto "point" da classe Coordinates
Coordinates point = Coordinates();

//EEPROM.write(3,0);  //encoder ESQUERDO
//EEPROM.write(4,0);  //encoder DIREITO

int i = 5;
int caminho[] = {};
struct Encoder{
  float encEsq;
  float encDir;
};

struct Posicao{
  float vX;
  float vY;
  float vA;
};

SoftwareSerial BT(4,5);  //4 = RX e 5 = TX

#define irDir 3
#define irEsq 2

int mA1 = 9,mA2 = 10,mB1 = 11,mB2 = 12; //variáveis de controle dos motores
float vEsq = 0.00;
float vDir = 0.00;

void rota(int coords[], int qt){        
  
   Posicao vertice;
  
  for(int cont = 0; cont <= qt; cont++){
    int endereco = cont+5;
    if(cont%2 == 0){
      vertice.vX = (float(coords[cont]))/10.0;
    }
    else{
      vertice.vY = (float(coords[cont]))/10.0;
    }
    EEPROM.put(endereco,vertice);
  }
}
void pipf(float x, float y){
  /*Recebe (x,y) do obj
   * - Le posicao e angulo atuais
   * - Calcula direcao e distancia ao obj
   * - Gira o robô para a direção do obj
   * - Move o robô a distância calculada
   */
  Posicao pos;
  EEPROM.get(0,pos);
  float xi = pos.vX;
  float yi = pos.vY;
  float ang = pos.vA*10.0;
  float dx = x-xi;
  float dy = y-yi;
  point.fromCartesian(dx,dy);
  float R = 10*point.getR();
  float alfa = point.getAngle();
  alfa *= 57.2958;
  //alfa = map(alfa,0.0,6.2832,0.0,360.0);
  float dAlfa = alfa-ang;
  R = (int) R;
  if(dAlfa > 0){
    float graus = dAlfa;//(dAlfa*4068.0)/71;
    graus = (int) graus;
    esqAx(graus);
    frente(R);
    pos.vX = x;
    pos.vY = y;
    pos.vA = alfa/10.0;
    EEPROM.put(0,pos);
  }
  else if(dAlfa < 0){
    float graus = dAlfa;//(dAlfa*(-4068.0))/71;
    graus = (int) graus;
    dirAx(graus);
    frente(R);
    pos.vX = x;
    pos.vY = y;
    pos.vA = alfa/10.0;
    EEPROM.put(0,pos);
  }
  else{
    frente(R);
    pos.vX = x;
    pos.vY = y;
    pos.vA = alfa/10.0;
    EEPROM.put(0,pos);
  }
  delay(1000);
  Serial.println(xi);
  Serial.println(yi);
  Serial.println(ang);
  Serial.println(R);
  Serial.println(dAlfa);
  
  mover(0);
  delay(1000);
  
}
void mover(int comando){
  if(comando == 0){
    digitalWrite(mA1, LOW);
    digitalWrite(mA2, LOW);
    digitalWrite(mB1, LOW);
    digitalWrite(mB2, LOW);
  }
  else if(comando == 1){
    //Serial.println("frente");
    digitalWrite(mA1, HIGH);
    digitalWrite(mA2, LOW);
    digitalWrite(mB1, HIGH);
    digitalWrite(mB2, LOW);
  }
  else if(comando == 2){
    //Serial.println("trás");
    digitalWrite(mA1, LOW);
    digitalWrite(mA2, HIGH);
    digitalWrite(mB1, LOW);
    digitalWrite(mB2, HIGH);
  }
  else if(comando == 3){
    //Serial.println("esquerda");
    digitalWrite(mA1, HIGH);
    digitalWrite(mA2, LOW);
    digitalWrite(mB1, LOW);
    digitalWrite(mB2, HIGH);
  }
  else if(comando == 4){
    //Serial.println("direita");
    digitalWrite(mA1, LOW);
    digitalWrite(mA2, HIGH);
    digitalWrite(mB1, HIGH);
    digitalWrite(mB2, LOW);
  }
}

void encEsq(){
  vEsq+= 0.01;
  Encoder encoder;
  EEPROM.get(3,encoder);
  //encoder.encEsq+=1.00;
  encoder.encEsq+= 0.01;
  EEPROM.put(3,encoder);
  //Serial.println(encoder.encEsq);
}
void encDir(){
  vDir+= 0.01;
  Encoder encoder;
  EEPROM.get(3,encoder);
  //encoder.encDir+=1.00;
  encoder.encDir+= 0.01;
  EEPROM.put(3,encoder);
}

void esqAx(int graus){
  /*Rotação por pulso = 35,26º (9,79%)
   * AJUSTAR CONVERSÃO E ARMAZENAMENTO DE PULSOS
   * A EEPROM SÓ SALVA VALORES ENTRE 0 E 255
   */
  //Serial.println(graus);
  delay(1000);
  Encoder encoder;
  
  encoder.encEsq = 0;
  encoder.encDir = 0;
  EEPROM.put(3,encoder);
  //EEPROM.write(3,0);
  //EEPROM.write(4,0);
  
  int setPoint = graus/3;
  EEPROM.get(3,encoder);
  while(391.304*vEsq < setPoint || 391.304*vDir < setPoint){
    //int s = int(Serial.parseInt(SKIP_WHITESPACE));
    //if(s == 9) break;
    //Serial.println(encoder.encEsq);
    mover(3);
    
  }
  mover(0);
}
void dirAx(int graus){
  /*Rotação por pulso = 35,26º (9,79%)
   * 
   */
  Encoder encoder;
  encoder.encEsq = 0;
  encoder.encDir = 0;
  EEPROM.put(3,encoder);
  //EEPROM.write(3,0);
  //EEPROM.write(4,0);
  int setPoint = graus/3;
  EEPROM.get(3,encoder);
  while(map(encoder.encDir,0,0.92,0,360) <= setPoint || map(encoder.encEsq,0,0.92,0,360)){
    //int s = int(Serial.parseInt(SKIP_WHITESPACE));
    //if(s == 9) break;
    mover(4);
  }
  mover(0);
}
void frente(int mm){
  /*cada pulso = 5 mm
   * dividir a dist por 5 para a contagem de pulsos
   */
  Encoder encoder;
  encoder.encEsq = 0;
  encoder.encDir = 0;
  EEPROM.put(3,encoder);
  int setPoint = mm/5;
  EEPROM.get(3,encoder);
  while(100*vEsq <= setPoint || 100*vDir <= setPoint){
    //int s = int(Serial.parseInt(SKIP_WHITESPACE));
    //if(s == 9) break;
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
  //esqAx(360);
}

void loop() {
  /*
  esqAx(90);
  delay(5000);
  dirAx(90);
  //int i = 5;
  //int caminho[] = {};
  //delay(1000);
  */
  while(Serial.available()> 0){
  double ab = Serial.parseInt(SKIP_WHITESPACE);
  double ord = Serial.parseInt(SKIP_WHITESPACE);
  ab = float(ab);
  ord = float(ord);
  //Serial.println(ab);
  //Serial.println(ord);
  delay(1000);
  pipf(ab,ord); 
  
  }
    
  //mover(0);
  
}
