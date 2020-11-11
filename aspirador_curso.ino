//https://github.com/vitorshaft/roboMovel/raspMovelDC.ino
#include <SoftwareSerial.h> //biblioteca para configurar a comunicacao Serial
#include <EEPROM.h> //biblioteca para armazenar dados na memória ROM
#include <Coordinates.h>  //biblioteca para converter coordenadas

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

void rota(int coords[], int qt){        
  /*Armazena na EEPROM as coord da rota em enderecos sequenciais
   * Ex: [X,Y,Theta,Esq,Dir,x1,y1,x2,y2,xn,yn,...]
   * Recebe vetor de coordenadas (coords[]) e tamanho do vetor (qt)
   */
   Posicao vertice;
   /*
  for(int cont = 0; cont <= qt;cont++){
    int endereco = cont+5;
    if(coords[cont] > 0){
      int valor = map(coords[cont],0,400,0,255);  //redistribui valores de 0 a 400 em 0 a 255 para inserir na EEPROM
      EEPROM.write(endereco,valor);
    }
    else if(coords[cont] < 0){
      int ab = (coords[cont])*-1;
      int valor = map(ab,0,400,0,255);  //redistribui valores de 0 a 400 em 0 a 255 para inserir na EEPROM
      EEPROM.write(endereco,valor);
    }
  }
  */
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
  float dAlfa = alfa-ang;
  R = (int) R;
  if(dAlfa > 0){
    float graus = (dAlfa*4068.0)/71;
    graus = (int) graus;
    esqAx(graus);
    frente(R);
    pos.vX = x;
    pos.vY = y;
    pos.vA = alfa/10.0;
    EEPROM.put(0,pos);
  }
  else if(dAlfa < 0){
    float graus = (dAlfa*(-4068.0))/71;
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
    digitalWrite(mB1, LOW);
    digitalWrite(mB2, HIGH);
  }
  else if(comando == 2){
    //Serial.println("trás");
    digitalWrite(mA1, LOW);
    digitalWrite(mA2, HIGH);
    digitalWrite(mB1, HIGH);
    digitalWrite(mB2, LOW);
  }
  else if(comando == 3){
    //Serial.println("esquerda");
    digitalWrite(mA1, HIGH);
    digitalWrite(mA2, LOW);
    digitalWrite(mB1, HIGH);
    digitalWrite(mB2, LOW);
  }
  else if(comando == 4){
    //Serial.println("direita");
    digitalWrite(mA1, LOW);
    digitalWrite(mA2, HIGH);
    digitalWrite(mB1, LOW);
    digitalWrite(mB2, HIGH);
  }
}

void encEsq(){
  Encoder encoder;
  EEPROM.get(3,encoder);
  encoder.encEsq+=1.00;
  //encoder.encDir+= 0.01;
  EEPROM.put(3,encoder);
}
void encDir(){
  Encoder encoder;
  EEPROM.get(3,encoder);
  //encoder.encEsq+=1.00;
  encoder.encDir+= 0.01;
  EEPROM.put(3,encoder);
}

void esqAx(int graus){
  /*Rotação por pulso = 35,26º (9,79%)
   * AJUSTAR CONVERSÃO E ARMAZENAMENTO DE PULSOS
   * A EEPROM SÓ SALVA VALORES ENTRE 0 E 255
   */
  Encoder encoder;
  encoder.encEsq = 0;
  encoder.encDir = 0;
  EEPROM.put(3,encoder);
  //EEPROM.write(3,0);
  //EEPROM.write(4,0);
  int setPoint = graus/3;
  EEPROM.get(3,encoder);
  while(map(encoder.encEsq,0,0.92,0,360) <= setPoint || map(encoder.encDir,0,0.92,0,360)){
    int s = int(Serial.parseInt(SKIP_WHITESPACE));
    if(s == 9) break;
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
    int s = int(Serial.parseInt(SKIP_WHITESPACE));
    if(s == 9) break;
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
  while(encoder.encEsq <= setPoint || encoder.encDir <= setPoint){
    int s = int(Serial.parseInt(SKIP_WHITESPACE));
    if(s == 9) break;
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
  Encoder ncd;
  EEPROM.get(5,ncd);
  Serial.println(ncd.encDir);
  mover(1);
  
  int i = 5;
  int caminho[] = {};
  //delay(1000);
  
  while(Serial.available()){
    long com = Serial.parseInt(SKIP_WHITESPACE);
    int comando = int(com);
    caminho[i] = comando;
    frente(comando);
    //mover(0);
  }
  rota(caminho,sizeof(caminho));
  */
  //int i = 5;
  //int caminho[] = {};
  //delay(1000);
  //while(Serial.available()){
  long com = Serial.parseInt(SKIP_WHITESPACE);
  if(com > 0){
    caminho[i] = float(com);
    i++;
  }
  else{
    i = 5;
  }
  Serial.println(com);
  //}
  rota(caminho,sizeof(caminho));
  Posicao vert;
  for(int ponto = 5; ponto <= sizeof(caminho); ponto++){
    EEPROM.get(ponto,vert);
    Serial.println(vert.vX);
    Serial.println(vert.vY);
    pipf(vert.vX,vert.vY);
  }
  
  mover(0);
  
}
