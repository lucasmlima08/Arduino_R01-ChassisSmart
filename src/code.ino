int const motorRight = 2;
int const motorLeft = 6;

int const lightFrontRight = 4;
int const lightFrontLeft = 5;
int const lightBreakRight = 6;
int const lightBreakLeft = 7;
int const lightArrowRight = 8;
int const lightArrowLeft = 9;

int const trigUltrasonic = 10;
int const echoUltrasonic = 11;

int const buttonPower = 12;

int const potenciometer = 0;

int limitCollision = 20;

boolean statusActions; //- Status de ações. On / Off
int intervalTime; //- Tempo definido para uma ação.
String currentDirection; //- Direção atual.
int lastTime; //- Auxilia na contagem do tempo.
int vPot; //- Recebe o valor do potenciometro.

void setup(){
    for (int i = 2; i <= 10; i++)
        pinMode(i, OUTPUT);
    for (int i = 11; i <= 12; i++)
        pinMode(i, INPUT);
    statusActions = true;
    intervalTime = 0;
    currentDirection = "break";
    lastTime = millis();
    vPot = analogRead(potenciometer) / 4;
}

void loop(){
    vPot = analogRead(potenciometer) / 4;
    //power(); //- Verifica se está ligado.
    if (statusActions == true) //- Chama processo se estiver ligado.
        process();
    intervalTime--;
    delay(1);
}

void process(){
    //- Verifica se vai colidir.
    int collision = detectCollision();
    if (collision == false){ //- Não vai colidir.
        int timeMillis = millis();
        if (timeMillis - lastTime >= intervalTime){
            lastTime = timeMillis;
            motionChange(); //- Muda direção se o tempo esgotou.
            movement();
        }
    } else { //- Vai colidir.
        //- Muda a direção para um dos lados.
        int generateDirection = random(2);
        if (generateDirection == 0){
            currentDirection = "right";
        } else {
            currentDirection = "left";
        }
        intervalTime = 1000;
        resetLeds();
        resetMotors();
        movement(); //- Movimenta o ve�culo.
        delay(500);
    }
}

void power(){
    if (digitalRead(buttonPower) == HIGH){
        if (statusActions == false)
            statusActions = true;
        else
            statusActions = false;
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
    if (distance <= limitCollision)
        return true;
    else
        return false;
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
        if (generateDirection == 2)
            currentDirection = "right";
        else
            currentDirection = "left";
    }
    resetLeds();
    resetMotors();
}

void movement(){
    if (currentDirection.equals("front")){
        analogWrite(motorRight, vPot);
        analogWrite(motorLeft, vPot);
        digitalWrite(lightFrontRight, HIGH);
        digitalWrite(lightFrontLeft, HIGH);
    } else if (currentDirection.equals("break")){
        digitalWrite(lightBreakRight, HIGH);
        digitalWrite(lightBreakLeft, HIGH);
    } else if (currentDirection.equals("right")){
        analogWrite(motorLeft, vPot);
        digitalWrite(lightArrowRight, HIGH);
    } else if (currentDirection.equals("left")){
        analogWrite(motorRight, vPot);
        digitalWrite(lightArrowLeft, HIGH);
    }
}

void resetLeds(){
    for (int i=4; i <= 9; i++)
        digitalWrite(i, LOW);
}

void resetMotors(){
    analogWrite(motorRight, 0);
    analogWrite(motorLeft, 0);
}

