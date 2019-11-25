#define trigPin 12
#define echoPin 11
/* 343m/s => 0.0343cm/uS */

void setup()
{
	Serial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop()
{
	long time, distance;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    time = pulseIn(echoPin, HIGH);
    distance = time / 58;

    Serial.print(distance);
    Serial.println(" cm");

    delay(500);   
}
