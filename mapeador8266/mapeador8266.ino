const int encEsq = 12;
const int encDir = 14;

float pEsq = 0.0;
float pDir = 0.0;

void esq(){
  pEsq+=0.1;
}
void dir(){
  pDir+=0.1;
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(encEsq),esq,CHANGE);
  attachInterrupt(digitalPinToInterrupt(encDir),dir,CHANGE);

}

void loop() {
  //INSERIR BROADCAST DO ENCODER VIA WIFI

}
