//- Motores: rodas.
#define motorRight = 0;
#define motorLeft = 1;
//- Leds: faróis.
#define lightFrontRight = 2;
#define lightFrontLeft = 3;
//- Leds: setas.
#define lightArrowRight = 4;
#define lightArrowLeft = 5;
//- Leds: freios.
#define lightBreakRight = 6;
#define lightBreakLeft = 7;
//- Sensor ultrassônico.
#define trigUltrasonic = 8;
#define echoUltrasonic = 9;
//- Botão: liga-desliga.
#define buttonPower = 10;
//- Limite de colisão.
#define limitCollision = 20;

//- Status de funcionamento.
boolean ligado;
//- Intervalo de tempo atual.
int currentTime;
//- Direção atual.
String currentDirection;

setup(){
  	//- Pinos de saída.
  	for (int i=0; i <= 8; i++){
  		  pinMode(i, OUTPUT);
  	}
  	//- Pinos de entrada.
  	for (int i=9; i <= 10; i++){
  		  pinMode(i, INPUT);
  	}
    ligado = false;
    currentTime = 0;
    currentDirection = "break";
}

loop(){

    //- liga-desliga.
  	power();

    //- Chama processo se estiver ligado.
    if (ligado == true){}
        process();
	}
}

void process(){
    //- Verifica se vai colidir. 
    int collision = detectCollision();
    if (collision == false){
        int timeMillis = millis();
        if (timeMillis + currentTime <= timeMillis){
            motionChange(); //- Muda direção se o tempo esgotou.
        } else {
            movement(); //- Mantém direção.
        }
    } else {
		motionChange(); //- Muda direção se vai colidir.
	}
    currentTime--;
}

void power(){
  	int leituraBotao = digitalRead(botao);
  	if (leituraBotao == HIGH){
    	if (ligado == false){
			ligado = true;
            currentTime = 1000;
    	} else {
    		ligado = false;
    	}
  	}
}

boolean detectCollision(){
    digitalWrite(trigUltrasonic, LOW);
    delayMicroseconds(2);
    digitalWrite(trigUltrasonic, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigUltrasonic, LOW);
    int distance = pulseIn(echoUltrasonic, HIGH) / 29 / 2;
    if (distance <= limitCollision){
        return true;
    } else {
        return false;
    }
}

void motionChange(){
    //- 0 = frente/front | 1 = freio/break | 2 = direita/Right | 3 = esquerda/left.
    //- 3 segundos/seconds = frente/front | 2 segundos/seconds = freio/break | 1 segundo/second = direita-esquerda/right-left.
    int generateDirection = randomSeed(4);
    if (generateDirection == 0){
        currentTime = 1000 * 3;
        currentDirection = "front";
    } else if (generateDirection == 1){
        currentTime = 1000 * 2;
        currentDirection = "break";
    } else {
        currentTime = 1000;
        if (generateDirection == 2){
            currentDirection = "right";
        } else {
            currentDirection = "left";
        }
    }
    resetLeds();
}

void movement(){
    if (currentDirection.equals("front")){
        digitalWrite(motorRight, HIGH);
        digitalWrite(motorLeft, HIGH);
        digitalWrite(lightFrontRight, HIGH);
        digitalWrite(lightFrontLeft, HIGH);
    } else if (currentDirection.equals("break")){
        digitalWrite(motorRight, LOW);
        digitalWrite(motorLeft, LOW);
        digitalWrite(lightBreakRight, HIGH);
        digitalWrite(lightBreakLeft, HIGH);
    } else if (currentDirection.equals("right")){
        digitalWrite(motorRight, LOW);
        digitalWrite(motorLeft, HIGH);
        digitalWrite(lightArrowRight, HIGH);
    } else if (currentDirection.equals("left")){
        digitalWrite(motorRight, HIGH);
        digitalWrite(motorLeft, LOW);
        digitalWrite(lightArrowLeft, HIGH);
    }
}

void resetLeds(){
    for (int i=2; i <= 7; i++){
        digitalWrite(i, LOW);
    }
}

