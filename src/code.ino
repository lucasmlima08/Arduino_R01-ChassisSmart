//- Motores: rodas.
#define motorRight 0
#define motorLeft 1
//- Leds: faróis.
#define lightFrontRight 2
#define lightFrontLeft 3
//- Leds: setas.
#define lightArrowRight 4
#define lightArrowLeft 5
//- Leds: freios.
#define lightBreakRight 6
#define lightBreakLeft 7
//- Sensor ultrassônico.
#define trigUltrasonic 8
#define echoUltrasonic 9
//- Botão: liga-desliga.
#define buttonPower 10
//- Limite de colisão.
#define limitCollision 20

//- Status de funcionamento.
boolean ligado;
//- Intervalo de tempo atual.
int intervalTime;
//- Direção atual.
String currentDirection;
//- Tempo para contagem de segudos.
int lastTime;

void setup(){
  	//- Pinos de saída.
  	for (int i=0; i <= 8; i++){
  		  pinMode(i, OUTPUT);
  	}
  	//- Pinos de entrada.
  	for (int i=9; i <= 10; i++){
  		  pinMode(i, INPUT);
  	}
    ligado = true;
    intervalTime = 0;
    currentDirection = "break";
    lastTime = millis();
}

void loop(){

    //- liga-desliga.
  	//power();

    //- Chama processo se estiver ligado.
    if (ligado == true){
        process();
	  }
}

void process(){
    //- Verifica se vai colidir. 
    int collision = detectCollision();
    if (collision == false){ //- Não vai colidir.
        if (millis() - lastTime >= intervalTime){
            lastTime = millis();
            motionChange(); //- Muda direção se o tempo esgotou.
        } else {
            movement(); //- Mantém direção.
        }
    } else { //- Vai colidir.
        //- Muda a direção para um dos lados. 
        int generateDirection = random(2);
        if (generateDirection == 0){
            currentDirection = "right";
        } else {
            currentDirection = "left";
        }
        intervalTime = 1000 * 2;
        movement(); //- Movimenta o veículo.
	  }
}

void power(){
  	if (digitalRead(buttonPower) == HIGH){
        if (ligado == false){
			      ligado = true;
    	  } else {
    		    ligado = false;
    	  }
        intervalTime = 0;
        delay(1000);
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
    delayMicroseconds(3);
    int generateDirection = random(4);
    if (generateDirection == 0){
        intervalTime = 1000 * 3;
        currentDirection = "front";
    } else if (generateDirection == 1){
        intervalTime = 1000 * 2;
        currentDirection = "break";
    } else {
        intervalTime = 1000;
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

