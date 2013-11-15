int motorMicro=9;
int leitura=0;
int velocidade=0;

void setup () {
}

void loop(){
  leitura=analogRead(A0);
  velocidade=map(leitura, 0,1023,0,255);
  analogWrite(motorMicro,velocidade);
  delay(100);
}
