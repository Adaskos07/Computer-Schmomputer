const byte LED_RED = 5;
const byte LED_YELLOW = 3;
const byte LED_GREEN = 2;
const byte BUTTON_POWER = 8;
const byte BUTTON = 10;

unsigned long startTime;
unsigned long elapsedTime;
unsigned long stateTime;

unsigned long buttonStartT;
unsigned long buttonElapsedT;
unsigned long buttonControlSum;

enum State {
    RED,
    RED_YELLOW,
    GREEN,
    YELLOW,
};
byte currentState;

bool buttonActive;

void setup() {
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(BUTTON_POWER, OUTPUT);
    pinMode(BUTTON, INPUT);

    digitalWrite(BUTTON_POWER, HIGH);

    buttonControlSum = 0;
    currentState = RED;
    switchLights(currentState);
    stateTime = 4000;
    buttonActive = false;

    startTime = buttonStartT = millis(); 
}

void loop() {
    /* check if button pressed for 800ms */
    buttonElapsedT = millis() - buttonStartT;
    if (isPressed() && (buttonElapsedT >= 25)) {
       buttonControlSum += 25;
       buttonStartT = millis();
       if (buttonControlSum >= 800)
            buttonActive = true;
    }
    else if (isPressed()) {
      ;
    }
    else {
      buttonControlSum = 0;
    }

    /* if pressed during RED, cut the state */
    if (buttonActive && currentState==RED)
        stateTime = 2000;

    /* change the state when once stateTime passes */
    elapsedTime = millis() - startTime;
    if (elapsedTime >= stateTime) {
        switch (currentState) {
            case RED:
                currentState = RED_YELLOW;
                stateTime = 1500;
                break;
            case RED_YELLOW:
                currentState = GREEN;
                stateTime = buttonActive ? 8000 : 4000; 

                buttonActive = false;
                break;
            case GREEN:
                if (buttonActive) {
                    currentState = GREEN;
                    stateTime = 8000;
                    buttonActive = false; 
                }
                else {
                    currentState = YELLOW;
                    stateTime = 2500;
                }
                break;
            case YELLOW:
                currentState = RED;
                stateTime = buttonActive ? 2000 : 4000;           
                break;
        }
        switchLights(currentState);
        startTime = millis();
    }
}

void switchLights(byte state) {
    switch (state) {
        case RED:
            digitalWrite(LED_RED, HIGH);
            digitalWrite(LED_YELLOW, LOW);
            digitalWrite(LED_GREEN, LOW);
            break;
        case RED_YELLOW:
            digitalWrite(LED_RED, HIGH);
            digitalWrite(LED_YELLOW, HIGH);
            digitalWrite(LED_GREEN, LOW);
            break;
        case GREEN:
            digitalWrite(LED_RED, LOW);
            digitalWrite(LED_YELLOW, LOW);
            digitalWrite(LED_GREEN, HIGH);
            break;
        case YELLOW:
            digitalWrite(LED_RED, LOW);
            digitalWrite(LED_YELLOW, HIGH);
            digitalWrite(LED_GREEN, LOW);
            break;
    }
}


inline bool isPressed() {
    return !digitalRead(BUTTON);
}
