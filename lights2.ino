#include "Queue.h"
//library from github

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

bool cutState;
bool buttonActive;
byte currentState;

enum Light {
    RED = 0,
    RED_YELLOW = 1,
    GREEN = 2,
    YELLOW = 3,
    RED_SHORT = 4,
    GREEN_LONG = 5,
}; 

int assignedTimes[6];
Queue<byte> stateQueue = Queue<byte>(30);

void switchLights(byte state);
bool isPressed();

void setup() {
    Serial.begin(19200);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(BUTTON_POWER, OUTPUT);
    pinMode(BUTTON, INPUT);
    /* activate the button */
    digitalWrite(BUTTON_POWER, HIGH);

    /* default state times in miliseconds */ 
    assignedTimes[RED]        = 4000;
    assignedTimes[RED_YELLOW] = 1500;
    assignedTimes[GREEN]      = 3000;
    assignedTimes[YELLOW]     = 3000;
    assignedTimes[RED_SHORT]  = 2000;
    assignedTimes[GREEN_LONG] = 6000;

    cutState = false;

    stateQueue.push(RED);
    switchLights(RED);
    buttonControlSum = 0;

    startTime = buttonStartT = millis(); 
    delay(10);   
}

void loop() {
    currentState = stateQueue.peek();

    /* check if button pressed for longer than 800ms */
    buttonElapsedT = millis() - buttonStartT;
    if (isPressed() && (buttonElapsedT >= 25)) {
       buttonControlSum += 25;
       Serial.println(buttonControlSum);
       buttonStartT = millis();
       if (buttonControlSum >= 2100) {
         buttonActive = true;
       }
    }
    else if (isPressed()) {
      ;
    }
    else {
      buttonControlSum = 0;
    }

    if (buttonActive && (stateQueue.count() < 10)) {
        Serial.println("PRESSED");
        switch (currentState) {
            case RED:
                stateQueue.push(RED_SHORT);
                stateQueue.push(RED_YELLOW);
                stateQueue.push(GREEN_LONG);
                cutState = true;
                break;
            case RED_YELLOW:
            case GREEN:
            case GREEN_LONG:
                stateQueue.push(GREEN_LONG);
                break;
            case YELLOW:
                stateQueue.push(RED_SHORT);
                stateQueue.push(RED_YELLOW);
                stateQueue.push(GREEN_LONG);
                break;
        }
        buttonControlSum = 0;
        buttonActive = false;
    }

    elapsedTime = millis() - startTime;
    if ((elapsedTime >= assignedTimes[currentState] || cutState)) {
        //Serial.print(currentState);
        stateQueue.pop();
        cutState = false;
        if (stateQueue.count()==0) {
            if(currentState==GREEN_LONG)
              stateQueue.push(YELLOW);
            else
              stateQueue.push((currentState+1) % 4);
        }
        switchLights(stateQueue.peek());
        startTime = millis();
    }

}

void switchLights(byte state) {
    switch (state) {
        case RED:
        case RED_SHORT:
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
        case GREEN_LONG:
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
