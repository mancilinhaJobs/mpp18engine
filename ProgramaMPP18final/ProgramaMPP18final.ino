#include <Servo.h>

char config;   // byte para verificacao do tipo de configuracao
int tipo;      // 0 = regular e open 1 - micro;
char comando;   //para alterar velocidade do motor ou mudar a configuracao do programa

const int entradaStrain_1 = A0;  // entrada analogica do extensometro superior
const int entradaStrain_2 = A1;  // entrada analogica do extensometro inferior
int valorStrain_1 = 0;        // valor lido do extensometro superior (0-1023==0-5V)
int valorStrain_2 = 0;        // valor lido do extensometro superior (0-1023==0-5V)

Servo acelerador;          //o acelerador da open e da regular e um servo   
int motorMicro = 10;       //pino onde o motor da micro sera ligado

int aceleracaoMin; // a ser usado somente para regular e open
int aceleracaoMax; // a ser usado somente para regular e open
int deltaAceleracao;

int pos=90;
int leitura = 0;
int porcentMotor = 0;//porcentagem da velocidade do motor;
int  velocidade = 0;

unsigned long tempoAtual;

void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
        inicializacao();
}

void loop() {
  
         
        if(tipo=='0'){
            velocidade = map(porcentMotor, 0, 100, aceleracaoMin, aceleracaoMax);
            Serial.print("velocidade");
            Serial.println(velocidade);
            acelerador.write(velocidade);
        }
        if(tipo=='1'){
            velocidade = map(porcentMotor, 0, 100, 0, 255);
            analogWrite(motorMicro,velocidade);
        }
        valorStrain_1 = analogRead(entradaStrain_1);
        Serial.print("Leitura SG1:");
        Serial.println(valorStrain_1);
        valorStrain_2 = analogRead(entradaStrain_2);
        Serial.print("Leitura SG2:");
        Serial.println(valorStrain_2);
        if (Serial.available()>0){
          comando = (char)Serial.read();//tenho que pensar no loop
          switch (comando){
            case 'v': Serial.println("Qual a nova porcentagem da velocidade do motor:");
                      while(Serial.available()<=0){}
                      leitura = Serial.read()-48;//nao esta detectando erros!!!!
                      Serial.println(leitura);
                      porcentMotor = (10*leitura);//tah muito zuado....
                      while(Serial.available()<=0){}
                      leitura = Serial.read()-48;
                      Serial.println(leitura);
                      porcentMotor += leitura;
                      Serial.print("Ok, motor em:");
                      Serial.println(porcentMotor);
                      Serial.println(leitura);
                      break;
                      delay(1000);
            case 'c': inicializacao();
                      break;
            default: Serial.println("Comando invalido(v:alterar velocidade c:configuracao");
                     break;
          }
        }
        delay(1000);      
}

void sensorRPM()
{
  detachInterrupt(0);
  Serial.print("i");
  tempoAtual+=micros();
  Serial.println(tempoAtual);
  attachInterrupt(0, sensorRPM, RISING);
}

void inicializacao()
{
    pos=90;
    leitura = 0;
    velocidade = 0;
    porcentMotor = 0;
    boolean ready = false;
    while (!ready){
       while (Serial.available()<=0){
         Serial.println("Aguardando Conexao");
         delay(100);
       }
       config = (char)Serial.read(); 
       if (config == 'C'){
         Serial.println("Conectado");
         while (Serial.available()<=0){}
         tipo = Serial.read();
         switch (tipo){
           case '0': 
                     Serial.println("Configuracao: Regular");
                     configRegular();
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

void configRegular(){
    char alteracao;
    boolean readyMin=false;
    boolean readyMax=false;
    int extremo;
    acelerador.attach(9);
    acelerador.write(pos);
    Serial.println("Ajuste do acelerador");
    Serial.println("Altere a posicao e diga se eh o minimo ou maximo");
    while (!readyMin||!readyMax){
        while (Serial.available()<=0){}
        alteracao=Serial.read();
        switch (alteracao){
          case '+': pos+=1;
                    acelerador.write(pos);
                    break;
          case '-': pos-=1;
                    acelerador.write(pos);
                    break;
          
          case 'm': Serial.println("Ok, mas eh maximo ou minimo(min=0 e max=1)?");
                    while (Serial.available()<=0){}
                    extremo=Serial.read();
                    switch (extremo){
                      case '0': aceleracaoMin = pos;
                                readyMin=true;
                                Serial.print("Ok, aceleracao minima em:");
                                Serial.println(aceleracaoMin);
                                break;
                      case '1': aceleracaoMax = pos;
                                readyMax=true;
                                Serial.print("Ok, aceleracao maxima em:");
                                Serial.println(aceleracaoMax);
                                break;
                      default: Serial.println("Nao entendi se eh min ou max- digite m denovo");
                               break;
                    }
          }
     }
     Serial.println("Acelerador ajustado");
     deltaAceleracao = aceleracaoMax - aceleracaoMin;
     delay(100);
}
