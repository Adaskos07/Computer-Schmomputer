const byte LED_RED = 5;
const byte LED_YELLOW = 3;
const byte LED_GREEN = 2;
const byte BUTTON_POWER = 8;
const byte BUTTON = 10;

unsigned long startTime;
unsigned long elapsedTime;

unsigned long buttonStartT;
unsigned long buttonElapsedT;
unsigned long buttonControlSum;

enum State {
    RED,
    RED_YELLOW,
    GREEN,
    YELLOW,
}

byte currentState;
bool buttonActive;


void setup() {
    // put your setup code here, to run once:
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(BUTTON_POWER, OUTPUT);
    pinMode(BUTTON, INPUT);
    /* activate the button */
    digitalWrite(BUTTON_POWER, HIGH);

    buttonControlSum = 0;

    startTime = buttonStartT = millis(); 
    delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:

    buttonElapsedT = millis() - buttonStartT;
    if (isPressed() && (buttonElapsedT >= 25)) {
       buttonControlSum += 25;
       Serial.println(buttonControlSum);
       buttonStartT = millis();
       if (buttonControlSum >= 1000) {
         buttonActive = true;
       }
    }
    else if (isPressed()) {
      ;
    }
    else {
      buttonControlSum = 0;
    }

    
    switch (currentState) {
        case RED:
        case RED_YELLOW:
        case GREEN:
        case YELLOW:
    }
}

inline bool isPressed() {
    return !digitalRead(BUTTON);
}
