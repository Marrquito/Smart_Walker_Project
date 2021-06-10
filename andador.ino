#include <LiquidCrystal.h>

LiquidCrystal LCD(12,11,5,4,3,2); // iniciando a lib do LCD

int button = 13;
int buttonStart = 0;
int buttonEnd = 0;
int ledRed = 10;

float calculaCelsius(){ // func para calcular °C
  	int tmp;
    float voltage, milliVolt, celsius;
	
	// serie de conversoes retiradas da documentacao do sensor
  	tmp = analogRead(A0); // tmp recebe o valor lido da porta analogica A0
    voltage = (tmp * 5.0) / 1024;
    milliVolt = voltage * 1000;
    celsius = (milliVolt - 500 ) / 10; 
  	
  	return celsius;
}

void VerificaCelsiusCM(){
	LCD.print("Celsius: ");
	LCD.print(calculaCelsius());
	delay(700);
	LCD.clear();
	delay(700);
}

/* void VerificaCelsiusSM(float *celsius){
	LCD.print(calculaCelsius());
	delay(500);
	LCD.clear();
	delay(500);
	*celsius = calculaCelsius();
}
 */

void setup(){
  	
	pinMode(button, INPUT);
	pinMode(ledRed, OUTPUT);

  	LCD.begin(18, 4); // inicia os pixels da telinha
  	LCD.setCursor(0, 1); // onde a telinha inicia
  
    pinMode(A0, INPUT); // sensor de temperatura
    pinMode(27, OUTPUT); // porta do sensor de temperatura
    Serial.begin(9600);
}

void loop(){
  	int i, press = 1, press2 = 1;
  	float celsius;
  	
	buttonStart = digitalRead(button);
	celsius = calculaCelsius();
    
	if(celsius >= 35.5){
		while(1){
			press = 1;
			if(celsius >= 35.5 && press == 1){ // veio botou a mao = ligou
				celsius = calculaCelsius();
				VerificaCelsiusCM();
				press2 = 1;
			}else if(celsius < 35.5){ // tirou a mao = desligado ( precisa enviar msg )
				LCD.print("stand-by...");
				delay(1000);
				LCD.clear();
				while(press2){
					for(i = 60; i >= 0; i--){
						if(celsius >= 35.5) break;
						buttonStart = digitalRead(button);
						if(buttonStart == HIGH){
							delay(1000);
							LCD.print("Desligando...");
							delay(1000);
							press2 = 0;
							break;
						}else{
							celsius = calculaCelsius();
							//VerificaCelsiusSM(&celsius);
							LCD.print("DESATIVE...");
							delay(1000);
						}
						digitalWrite(ledRed, HIGH);
						delay(1000);
						digitalWrite(ledRed, LOW);
						
					}
				}
			// envia mensagem pro ctt de emergencia aqui
				celsius = calculaCelsius();
			}
			celsius = calculaCelsius();
		}
	}
}