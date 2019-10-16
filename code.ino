int LED_RED = 4;
int LED_YELLOW = 3;
int LED_GREEN = 2;
int BUTTON = 10;
int delayTimeRed;
int delayTimeGreen;
unsigned long StartTime;
unsigned long ElapsedTime;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUTTON, INPUT);
  delayTimeRed = 2000;
  delayTimeGreen = 5000;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  digitalWrite(LED_RED, HIGH);
  redTime();
  
  digitalWrite(LED_YELLOW,HIGH);
  redTime();
  
  delayTimeRed = 2000;
  
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, HIGH);
  StartTime = millis();
  while(1) {
    ElapsedTime = millis() - StartTime;
    if(!digitalRead(BUTTON)) {
      delayTimeGreen = 10000;
    }
    if(ElapsedTime >= delayTimeGreen) {
      break;
    }
  }
  delayTimeGreen = 5000; 

  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_YELLOW,HIGH);
  redTime();
  

  digitalWrite(LED_YELLOW, LOW);
}

void redTime() {
  StartTime = millis();
  while(1) {
    ElapsedTime = millis() - StartTime;
    if(!digitalRead(BUTTON)) {
      delayTimeRed = 1000;
      delayTimeGreen = 10000;
    }
    if(ElapsedTime >= delayTimeRed) {
      break;
    }
  }
}
