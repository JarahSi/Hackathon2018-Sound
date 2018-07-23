
#define uOneTrigPin 12
#define uOneEchoPin 11

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode(uOneTrigPin, OUTPUT);
  pinMode(uOneEchoPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  long distanceSensorOne;

  distanceSensorOne = getDistance(uOneTrigPin, uOneEchoPin);

  printDistance(distanceSensorOne);

  delay(500);
}

long getDistance(int trigPin, int echoPin)
{
  long distance, duration;

  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  return distance;
}

void printDistance(long distance)
{
  if (distance >= 200 || distance <= 0){
    Serial.println("Out of range");
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
  }
}
