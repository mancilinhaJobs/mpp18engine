char config;   // byte para verificacao do tipo de configuracao
int tipo;      // 0 = regular e open 1 - micro;
const int entradaStrain_1 = A0;  // entrada analogica do extensometro superior
const int entradaStrain_2 = A0;  // entrada analogica do extensometro inferior
int valorStrain_1 = 0;        // valor lido do extensometro superior (0-1023==0-5V)
int valorStrain_2 = 0;        // valor lido do extensometro superior (0-1023==0-5V) 
volatile unsigned long tempoAntes = 0;
volatile unsigned long tempo_1_Volta=0;
volatile unsigned long tempo_Voltas=0;
volatile unsigned int voltas = 0;
volatile int estado=0;
volatile unsigned long rpm = 0;
unsigned long rpmAntes = 0;
unsigned long tempoAtual; 
int motorMicro=9;
int leitura=0;
int velocidade=0;

void setup() {

        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
        pinMode(13, OUTPUT);
}

void loop() {
        
        attachInterrupt(0, sensorRPM, RISING);//inicializacao da rpm,  a mesma pra todas configuracoes
        tempoAntes = micros();
        //loop a definir
        while (Serial.available()<=0){
            leitura=analogRead(A0);
            velocidade=map(leitura, 0,1023,0,255);
            analogWrite(motorMicro,velocidade);
        };
}

void inicializacao(){
    Serial.println("Manda a nova configuracao logo seu mala");
    while (Serial.available() <= 0){};
    config = (char)Serial.read(); 
    if (config=='C'){
    Serial.println("Certo, mas mande o tipo");
    while (Serial.available()<=0){};
      tipo = Serial.read();
      switch (tipo){
      case '0': Serial.println("Configuracao: Regular");
                break;
      case '1': Serial.println("Configuracao: Micro");
                    break;
      default: Serial.println("Configuracao zuada, por favor repita");
                    break;
    }         
    }
    else {Serial.println("Configuracao zuada, por favor repita");}
}

void sensorRPM()
{
  if (estado){
    digitalWrite(13, HIGH);
    estado = 0;
  }
  else{digitalWrite(13, LOW);
    estado=1;}
  tempoAtual = micros();
  tempo_1_Volta = tempoAtual - tempoAntes;
  tempo_Voltas = tempo_1_Volta + tempo_Voltas; 
  tempoAntes = tempoAtual;
  voltas++;
  if (tempo_Voltas > 100000){
    rpm = ((voltas*60000000)/tempo_Voltas);
    tempo_Voltas-=;
    voltas = 0;
    Serial.print("RPM: ");
    Serial.println(rpm);
  }
}
