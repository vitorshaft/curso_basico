const int trig = 7;
const int echo = 8;

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

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(13,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  medirDist();
  
  if(dist < 100){
    Serial.println(dist);
    if(dist > 10){
      digitalWrite(13,HIGH);
    }
    else{
      digitalWrite(13,LOW);
    }
  }
  delay(500);
}
