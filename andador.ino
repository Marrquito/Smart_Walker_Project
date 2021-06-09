#include <LiquidCrystal.h>

LiquidCrystal LCD(12,11,5,4,3,2); // iniciando a lib do LCD

int sensorPino = 1; // pino analogico A1
int pinoButton = 13;

float calculaCelsius(){ // func para calcular °C
  	int tmp;
    float voltage, milliVolt, celsius;
	
  	tmp = analogRead(A0); // conversões retirdas da documentação do sensor
    voltage = (tmp * 5.0) / 1024;
    milliVolt = voltage * 1000;
    celsius = (milliVolt - 500 ) / 10; 
  	
  	return celsius;
}

void setup(){
  	pinMode(pinoButton, INPUT);
  
  	LCD.begin(18, 4); // inicia os pixels da telinha
  	LCD.setCursor(0, 1); // onde a telinha inicia
  
    pinMode(A0, INPUT); // sensor de temperatura
    pinMode(27, OUTPUT); // porta do sensor de temperatura
    Serial.begin(9600);

}

void loop(){

  	int i;
  	float celsius;
  	celsius = calculaCelsius();
    
    
  	//int button, lastButton; // variaveis p/ controle do botao
  	
  	//lastButton = button = 0; // os dois iniciam com 0 p/ OFF
  
 	if(celsius >= 35.5){ // veio botou a mao = ligou
      	  LCD.print("Celsius: ");
          LCD.print(calculaCelsius());
          delay(700);
          LCD.clear();
  	      delay(700);
		  calculaCelsius();	
    }else{ // tirou a mao = desligado ( precisa enviar msg )
      	for(i = 5; i >= 0; i--){
          	if(celsius >= 35.5) break;
      		LCD.print(calculaCelsius());
          	delay(500);
          	LCD.clear();
          	delay(500);
            celsius = calculaCelsius();
        }
      // envia mensagem pro ctt de emergencia
    }
	
  
}