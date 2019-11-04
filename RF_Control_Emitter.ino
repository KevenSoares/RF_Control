#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <LiquidCrystal.h>
struct estruturaDadosRF
{
   boolean ligando = false;   //Esta variavel será usada para solicitar os dados do outro aparelho. Será útil quando o aparelho solicitante esta sendo ligado, para manter os valores do aparelho que já esta ligado.
   int potenciometro1 = 0;
   int potenciometro2 = 0;
   int potenciometro3 = 0;
   int potenciometro4 = 0;
   float angulompu[2];
};
typedef struct estruturaDadosRF tipoDadosRF;
tipoDadosRF dadosRF;
tipoDadosRF dadosRecebidos;

boolean transmitido = true;
boolean alterado = false;

RF24 radio(9,10);

const uint64_t pipe = 0xE8E8F0F0E1LL;


int pot1Ant = 0;
int pot1    = 0;
int pot2Ant = 0;
int pot2    = 0;
int pot3Ant = 0;
int pot3    = 0;
int pot4Ant = 0;
int pot4    = 0;
int angulo1 = 0;
int angulo2 = 0;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() 
{
  lcd.begin(20, 4);
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);

}

void loop() 
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
//************************sessão de controle de leitura*******************************
    pot1 = analogRead(A0);
    if (pot1 != pot1Ant) {
       dadosRF.potenciometro1 = pot1;
       alterado = true;  //esta variavel controla o envio dos dados sempre que houver uma alteração
    }
    pot1Ant = pot1;
  
    pot2 = analogRead(A1);
    if (pot2 != pot2Ant) {
       dadosRF.potenciometro2 = pot2;
       alterado = true;  //esta variavel controla o envio dos dados sempre que houver uma alteração
    }
    pot3Ant = pot3;
        pot3 = analogRead(A2);
    if (pot3 != pot3Ant) {
       dadosRF.potenciometro3 = pot3;
       alterado = true;  //esta variavel controla o envio dos dados sempre que houver uma alteração
    }
    pot3Ant = pot3;
  
    pot4 = analogRead(A3);
    if (pot4 != pot4Ant) {
       dadosRF.potenciometro4 = pot4;
       alterado = true;  //esta variavel controla o envio dos dados sempre que houver uma alteração
    }
    pot4Ant = pot4;
    
    angulo1 = map(dadosRF.potenciometro3, 0, 1023, 0, 179);
    int velocidade = map(dadosRF.potenciometro4, 0, 1023, 0, 100);
    lcd.setCursor(0, 0);
  lcd.print("L:");
  lcd.setCursor(6, 0);
  lcd.print("V:");
  lcd.setCursor(11, 0);
  lcd.print("%");
  lcd.setCursor(13, 0);
  lcd.print("CM:");
  lcd.setCursor(18, 0);
  lcd.print("A");
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.setCursor(5, 1);
  lcd.print("C");
  lcd.setCursor(8, 1);
  lcd.print("X:");
  lcd.setCursor(8, 2);
  lcd.print("Y:");
  lcd.setCursor(8, 3);
  lcd.print("Z:");
  lcd.setCursor(0, 3);
  lcd.print("jaraqui");
  lcd.setCursor(1,0);
  lcd.print(ArrumaZero(angulo1));
  lcd.setCursor(7,0);
  lcd.print(ArrumaZero(velocidade));
  lcd.setCursor(10,1);
  lcd.print(dadosRF.angulompu[0]);
  lcd.setCursor(10,2);
  lcd.print(dadosRF.angulompu[1]);
   
}
String ArrumaZero(int i)
{
  String ret;
  if (i < 100) ret += "0";
  ret += i;
  return ret;
}

