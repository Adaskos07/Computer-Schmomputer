const byte LED_RED = 12;
const byte LED_YELLOW = 11;
const byte LED_GREEN = 10;

const byte PD_LED_RED = 8;
const byte PD_LED_GREEN = 7;

const byte BUS_BUTTON_POWER = 4;
const byte BUS_BUTTON = 5;

const byte PD_BUTTON_POWER = 2;
const byte PD_BUTTON = 3;

unsigned long startTime;
unsigned long elapsedTime;
unsigned long stateTime;

unsigned long busButtonStartT;
unsigned long busButtonElapsedT;
unsigned int busButtonCtrlSum;

unsigned long pdButtonStartT;
unsigned long pdButtonElapsedT;
unsigned int pdButtonCtrlSum;

enum State {
    RED        = (1 << 0),
    RED_YELLOW = (1 << 1),
    GREEN      = (1 << 2),
    YELLOW     = (1 << 3),
    PD_RED     = (1 << 4),
    PD_GREEN   = (1 << 5),
};
byte currentState;
byte previousState;

bool busButtonActive;
bool pdButtonActive;

void setup() {
    Serial.begin(9600);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(PD_LED_RED, OUTPUT);
    pinMode(PD_LED_GREEN,OUTPUT);

    pinMode(BUS_BUTTON_POWER, OUTPUT);
    pinMode(BUS_BUTTON, INPUT);
    pinMode(PD_BUTTON_POWER, OUTPUT);
    pinMode(PD_BUTTON, INPUT);

    digitalWrite(BUS_BUTTON_POWER, HIGH);
    digitalWrite(PD_BUTTON_POWER, HIGH);

    busButtonCtrlSum = 0;
    busButtonActive = false;

    pdButtonCtrlSum = 0;
    pdButtonActive = false;

    
    currentState = (RED | PD_RED);
    switchLights(currentState);

    Serial.println("BEGIN");

    stateTime = 4000;
    startTime = busButtonStartT = pdButtonStartT = millis(); 
}

void loop() {
    /* check if BUS button pressed for 900ms */
    busButtonElapsedT = millis() - busButtonStartT;
    if (isBusBtPressed() && (busButtonElapsedT >= 25)) {
       busButtonCtrlSum += 25;
       busButtonStartT = millis();
       if (busButtonCtrlSum >= 900) {
            Serial.println("BUS_ACTIVE");
            busButtonCtrlSum = 0;
            busButtonActive = true;
       }
    }
    else if (isBusBtPressed()) {
      ;
    }
    else {
      busButtonCtrlSum = 0;
    }


    /* check if PEDESTRIAN button pressed for 100 ms */
    pdButtonElapsedT = millis() - pdButtonStartT;
    if (isPedBtPressed() && (pdButtonElapsedT >= 25)) {
       pdButtonCtrlSum += 25;
       pdButtonStartT = millis();
       if (pdButtonCtrlSum >= 100) {
            Serial.println("PEDESTRIAN_ACTIVE");
            pdButtonActive = true;
            pdButtonCtrlSum = 0;
        }
    }
    else if (isPedBtPressed()) {
      ;
    }
    else {
      pdButtonCtrlSum = 0;
    }


    /* if pressed during RED | PD_RED, cut the state */
    if (busButtonActive && currentState==RED | PD_RED)
        stateTime = 2000;

    /* change the state when once stateTime passes */
    elapsedTime = millis() - startTime;
    if (elapsedTime >= stateTime) {
        switch (currentState) {
            case (RED | PD_RED):
                if (previousState == (YELLOW | PD_RED)) {
                    currentState = RED | PD_GREEN;
                    stateTime = 8000;
                }
                else {
                    currentState = RED_YELLOW | PD_RED;
                    stateTime = 1500;
                }
                break;

            case (RED_YELLOW | PD_RED):
                currentState = GREEN | PD_RED;
                stateTime = busButtonActive ? 8000 : 4000;
                busButtonActive = false;
                break;

            case (GREEN | PD_RED):
                if (busButtonActive) {
                    currentState = GREEN | PD_RED;
                    stateTime = 8000;
                    busButtonActive = false;
                }
                else {
                    currentState = YELLOW | PD_RED;
                    stateTime = 2000;
                }
                break;

            case (YELLOW | PD_RED):
                previousState = currentState;
                currentState = RED | PD_RED;
                stateTime = 3000;
                break;

            case (RED | PD_GREEN):
                previousState = currentState;
                currentState = RED | PD_RED;
                stateTime = 4000;
                break;
        }
        switchLights(currentState);
        startTime = millis();
    }
}

void switchLights(byte state) {
    switch (state) {
        case (RED | PD_RED):
            digitalWrite(LED_RED, HIGH);
            digitalWrite(LED_YELLOW, LOW);
            digitalWrite(LED_GREEN, LOW);

            digitalWrite(PD_LED_RED, HIGH);
            digitalWrite(PD_LED_GREEN, LOW);
            break;
        case (RED_YELLOW | PD_RED):
            digitalWrite(LED_RED, HIGH);
            digitalWrite(LED_YELLOW, HIGH);
            digitalWrite(LED_GREEN, LOW);

            digitalWrite(PD_LED_RED, HIGH);
            digitalWrite(PD_LED_GREEN, LOW);
            break;
        case (GREEN | PD_RED):
            digitalWrite(LED_RED, LOW);
            digitalWrite(LED_YELLOW, LOW);
            digitalWrite(LED_GREEN, HIGH);

            digitalWrite(PD_LED_RED, HIGH);
            digitalWrite(PD_LED_GREEN, LOW);
            break;
        case (YELLOW | PD_RED):
            digitalWrite(LED_RED, LOW);
            digitalWrite(LED_YELLOW, HIGH);
            digitalWrite(LED_GREEN, LOW);

            digitalWrite(PD_LED_RED, HIGH);
            digitalWrite(PD_LED_GREEN, LOW);
            break;
        case (RED | PD_GREEN):
            digitalWrite(LED_RED, HIGH);
            digitalWrite(LED_YELLOW, LOW);
            digitalWrite(LED_GREEN, LOW);

            digitalWrite(PD_LED_RED, LOW);
            digitalWrite(PD_LED_GREEN, HIGH);
            break;
    }
}


inline bool isBusBtPressed() {
    return !digitalRead(BUS_BUTTON);
}

inline bool isPedBtPressed() {
    return !digitalRead(PD_BUTTON);
}
