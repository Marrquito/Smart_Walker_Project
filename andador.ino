#include <LiquidCrystal.h> // biblioteca tela LCD
#include <IRremote.h> // biblioteca receptor infravermelho

#include <SoftwareSerial.h> // modulo GSM
#include <Sim800L.h>
Sim800L Sim800l;

LiquidCrystal LCD(2,3,4,5,6,7); // iniciando a lib do LCD

char* msg = "Possivel situacao de queda";
char* num = "83981379312";

#define ledRed 12
#define infra 9
#define temperatura A0


IRrecv recebeSinal(infra);
decode_results resultInfra;

float calculaCelsius(){ // func para calcular °C

  // conversões necessárias para chegar na temperatura em °C
  float valor_analog_lm35 = float(analogRead(temperatura));
  float tensao = (valor_analog_lm35 * 5) / 1023; 
  float temperatura = tensao / 0.010; 

  delay(300); 
  
  return temperatura;
}


void VerificaCelsiusCM(){ //faz a verificacao e impressao da temperatura
  LCD.print("Celsius: ");
  LCD.print(calculaCelsius());
  delay(500);
  LCD.clear();
  delay(500);
}

void SendSMS(){ // envia a mensagem para o ctt de emergencia 
  /*
  Serial.println("Mensagem enviada para XX-XXXXXXXXX");
  LCD.print("Mensagem enviada para ctt de SOS");
  delay(1000);
  LCD.clear();
  LCD.print("p/ ctt de SOS");
  delay(1000);
  LCD.clear();
  */
  Serial.println("Sensor acionado! Enviando SMS...");
  Sim800l.sendSms(num, msg);
  Serial.println("SMS Enviado...");
}

void setup(){
  Serial.begin(9600);

  // GSM
  Sim800l.begin();
  Serial.println("Aguardando acionamento do sensor...");

  // led
  pinMode(ledRed, OUTPUT);

  //tela lcd;
  LCD.begin(16, 2); // inicia os pixels da telinha 
  LCD.setCursor(0, 0); // onde a telinha inicia

  // sensor de temperatura;
  pinMode(temperatura, INPUT); // sensor de temperatura

  //infravermelho
  recebeSinal.enableIRIn(); // inicia o recebimento de sinal infra
}


void loop(){
  int i, press = 1, press2 = 1;
  int pertei = 1;
  float celsius;
  
  celsius = calculaCelsius();
  LCD.setCursor(1, 3);
  LCD.print("Smart Walker");
  delay(500);
  LCD.clear();

  Serial.println(calculaCelsius());
    
  if(celsius >= 29){
      Serial.println(calculaCelsius());
      while(1){
        Serial.println(calculaCelsius());
        press = 1;
        if(celsius >= 29 && press == 1){ //botou a mao = ligou
          Serial.println(calculaCelsius());
          celsius = calculaCelsius();
          VerificaCelsiusCM();
          press2 = 1;
          pertei = 1;
        }else if(celsius < 29){ // tirou a mao = desligado ( precisa enviar msg )
          Serial.println(calculaCelsius());
          LCD.print("stand-by...");
          LCD.setCursor(1,3);
          LCD.print("Smart Walker");
          delay(1000);
          LCD.clear();
        
          while(press2){ // enquanto andador não for desativado
            if(pertei){ // condição para desativar por meio do infravermelho
              for(i = 10; i >= 0; i--){
                if(celsius >= 29) break;
                Serial.println(calculaCelsius());
                if(recebeSinal.decode(&resultInfra)){
                  recebeSinal.resume();
                }
                delay(10);
                if(resultInfra.value == 16726215){ // cod decimal do controle
                  Serial.println(calculaCelsius());
                  delay(1000);
                  LCD.print("Desligando...");
                  delay(1000);
                  LCD.clear();
                  press2 = 0;
                  break;
                }else{
                  Serial.println(calculaCelsius());  
                  LCD.print("DESATIVE...");
                  delay(500);
                  LCD.clear();
                  delay(500);
                  //Serial.println(calculaCelsius()); 
                  digitalWrite(ledRed, HIGH);
                  delay(250);
                  digitalWrite(ledRed, LOW);
                  delay(250);
                }
             
                if(i == 0) pertei = 0;
              }
            }else
              SendSMS();    
        }
         //Serial.println(calculaCelsius());
         celsius = calculaCelsius();
      }
        celsius = calculaCelsius();
        //Serial.println(calculaCelsius());
    }
  }
}
