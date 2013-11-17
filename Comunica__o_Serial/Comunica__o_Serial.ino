char config;   // byte para verificacao do tipo de configuracao
int tipo;      // 0 = regular e open 1 - micro;
const int entradaStrain_1 = A0;  // entrada analogica do extensometro superior
const int entradaStrain_2 = A1;  // entrada analogica do extensometro inferior
int valorStrain_1 = 0;        // valor lido do extensometro superior (0-1023==0-5V)
int valorStrain_2 = 0;        // valor lido do extensometro superior (0-1023==0-5V) 
volatile unsigned long tempoAntes = 0;
volatile unsigned long tempo_1_Volta=0;
volatile unsigned long delta_T=0;
volatile unsigned long tempoLimite=10000000;
volatile unsigned int voltas = 0;
volatile int estado=0;
volatile unsigned rpm = 0;
volatile float constante_rotacao;
unsigned long rpmAntes = 0;
unsigned long tempoAtual; 
int motorMicro = 9;
int leitura = 0;
int velocidade = 0;


void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
        pinMode(13, OUTPUT);
}

void loop() {
        inicializacao();
        tempoAntes = micros();
        while (Serial.available()<=0){
            delta_T = micros()-tempoAntes;
            if (delta_T>tempoLimite){
                rotacoes();
                delta_T=0;
            }
            leitura=analogRead(A0);
            velocidade=map(leitura, 0,1023,0,255);
            analogWrite(motorMicro,velocidade);       
        }
}


void rotacoes()
{
        rpm = (constante_rotacao*voltas);
        Serial.print("RPM: ");
        Serial.println(rpm);
        Serial.print("Tempo=");
        Serial.print(delta_T);
        Serial.print(" voltas=");
        Serial.println(voltas);
        tempoAntes = micros();
        voltas = 0;
}

void sensorRPM()
{
  detachInterrupt(0); 
  voltas = voltas + 1;
  attachInterrupt(0, sensorRPM, RISING);
}

void inicializacao()
{
    boolean ready = false;
    constante_rotacao = ((float)1000000/(float)tempoLimite)*60.f;
    while (!ready){
       while (Serial.available()<=0){
         Serial.println("Aguardando Conexao");
         delay(100);
       }
       config = (char)Serial.read(); 
       if (config=='C'){
         Serial.println("Conectado");
         while (Serial.available()<=0){}
         tipo = Serial.read();
         switch (tipo){
           case '0': 
                     Serial.println("Configuracao: Regular");
                     attachInterrupt(0, sensorRPM, RISING);
                     Serial.println("Ready: 000");
                     ready = true; 
                     break;
           case '1': Serial.println("Configuracao: Micro");
                     attachInterrupt(0, sensorRPM, RISING);
                     Serial.println("Ready: 001");
                     ready = true;
                     break;
           default: Serial.println("Configuracao invalida");
                     Serial.println("Erro: 001");
                     break;
          }         
       }
       else {Serial.println("Comando invalido");}
    }  
}

