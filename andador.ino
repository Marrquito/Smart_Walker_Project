#include <LiquidCrystal.h>
#include <IRremote.h>

LiquidCrystal LCD(12,11,5,4,3,2); // iniciando a lib do LCD

int button = 13;
int buttonStart = 0;
int ledRed = 10;
int infra = 9;


IRrecv recebeSinal(infra);
decode_results resultInfra;

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

void VerificaCelsiusCM(){ //faz a verificacao e impressao da temperatura
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

void SendSMS(){ // envia a mensagem para o ctt de emergencia 
	Serial.println("Mensagem enviada para XX-XXXXXXXXX");
	LCD.print("Mensagem enviada para ctt de SOS");
	delay(1000);
	LCD.clear();
	LCD.print("p/ ctt de SOS");
	delay(1000);
	LCD.clear();
}


void setup(){
    Serial.begin(9600);
  	
	//botao e tela lcd;
	pinMode(button, INPUT);
	pinMode(ledRed, OUTPUT);

	//tela lcd;
  	LCD.begin(18, 4); // inicia os pixels da telinha
  	LCD.setCursor(0, 1); // onde a telinha inicia
	
	// sensor de temperatura;
    pinMode(A0, INPUT); // sensor de temperatura
    pinMode(27, OUTPUT); // porta do sensor de temperatura

	//infravermelho
	recebeSinal.enableIRIn(); // inicia o recebimento de sinal infra

}

void loop(){
  	int i, press = 1, press2 = 1;
	int pertei = 1;
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
				while(press2){ // enquanto o botao nao for precionado para desativar
					if(pertei){ // if para condicao de desativacao por meio do botao
						for(i = 10; i >= 0; i--){
							if(celsius >= 35.5) break;
							buttonStart = digitalRead(button);
							 if(recebeSinal.decode(&resultInfra)){
								recebeSinal.resume();
							}

							if(buttonStart == HIGH || resultInfra.value == 0xFD08F7){
							//if(buttonStart == HIGH){
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
								LCD.clear();
							}
							digitalWrite(ledRed, HIGH);
							delay(1000);
							digitalWrite(ledRed, LOW);
							if(i == 0) pertei = 0;
						}
					}else{
						SendSMS();		
					}
				}
				celsius = calculaCelsius();
			}
			celsius = calculaCelsius();
		}
	}
}