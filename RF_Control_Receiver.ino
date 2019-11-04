#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Servo.h>

struct estruturaDadosRF
{
   boolean ligando = false;   //Esta variavel será usada para solicitar os dados do outro aparelho. Será útil quando o aparelho solicitante esta sendo ligado, para manter os valores do aparelho que já esta ligado.
   int potenciometro1 = 0;
   int potenciometro2 = 0;
   int potenciometro3 = 0;
   int potenciometro4 = 0;
   boolean pd = false;
};
typedef struct estruturaDadosRF tipoDadosRF;
tipoDadosRF dadosRF;
tipoDadosRF dadosRecebidos;

boolean transmitido = true;
boolean alterado = false;



int pot1Ant = 0;
int pot1    = 0;
int pot2Ant = 0;
int pot2    = 0;
int pot3Ant = 0;
int pot3    = 0;
int pot4Ant = 0;
int pot4    = 0;
int angulo2 = 0;

Servo servo2,ESC;


RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(void)
{
 pinMode(6, OUTPUT);
 pinMode(7, OUTPUT);
  servo2.attach(5);
  ESC.attach(3);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
}

void loop(void)
{
   //*************** Controle do RF ***********************
  // se houve alteração dos dados, envia para o outro radio 
  if (alterado || !transmitido) {
     radio.stopListening();                                   
     transmitido = radio.write( &dadosRF, sizeof(tipoDadosRF) );
     radio.startListening();  
     alterado = false;
  }

  //verifica se esta recebendo mensagem       
  if (radio.available()) {                                   
     radio.read( &dadosRecebidos, sizeof(tipoDadosRF) ); 

     //verifica se houve solicitação de envio dos dados
     if (dadosRecebidos.ligando) {
        alterado = true;
     } else {
        dadosRF = dadosRecebidos;
     }
  }
//***************************sessão de controle*****************************
  int vel = map(dadosRF.potenciometro4, 0, 1023, 0, 179); 
  ESC.write(vel);

  angulo2 = map(dadosRF.potenciometro3, 0, 1023, 38, 126);  
  servo2.write(angulo2);

  if(dadosRF.pd == true)
  {
    digitalWrite(6,HIGH);
    digitalWrite(7,HIGH);
  }
  if(dadosRF.pd == false)
  {
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);
  }
  
  delay(10);

}
