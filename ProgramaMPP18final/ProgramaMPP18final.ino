const int extensometro = A0;  // Analog input pin that the potentiometer is attached to

int tensaoExtensometro = 0;        // value read from the pot
volatile long previousMillis = 0;
long intervalo = 1000;
volatile unsigned int contagem = 0;
volatile int estado=0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  attachInterrupt(0, paradas, RISING);
  pinMode(13,OUTPUT);
}

void loop() {
  
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis > intervalo) {
    detachInterrupt(0); 
    previousMillis = currentMillis;// salva o ultimo periodo em que foi calculada a rotaçao  
    tensaoExtensometro = analogRead(extensometro);
    double rpm= (60*contagem);
    Serial.print("extensometro = " );                       
    Serial.print(tensaoExtensometro);      
    Serial.print(" rpm = ");      
    Serial.println(rpm);  
    contagem=0;
    attachInterrupt(0, paradas, RISING);
  }
}

void paradas()
{
  if (estado){
    digitalWrite(13, HIGH);
    estado = 0;
  }
  else{digitalWrite(13, LOW);
    estado=1;}
  contagem++;
}

