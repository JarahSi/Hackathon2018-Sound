#include <Servo.h>


#define uOneTrigPin 12
#define uOneEchoPin 11
#define uTwoTrigPin 10
#define uTwoEchoPin 9

#define uOneLEDPin 2
#define servoPin 3

Servo myServo;

String blipsMessage = "";         //variable to hold the blips
const int charLimit = 12;         //blipsMessage character limit

const int rotationIncrementLimit = 12; //number of rotational increments before a full 360 rotation
int currentDirection = 0;         //variable to hold the current direction sensor is facing

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode(uOneTrigPin, OUTPUT);
  pinMode(uOneEchoPin, INPUT);
  pinMode(uTwoTrigPin, OUTPUT);
  pinMode(uTwoEchoPin, INPUT);
  pinMode(uOneLEDPin, OUTPUT);

  myServo.attach(servoPin);
  blipsMessage.reserve(200);        //reserve 200 bytes to avoid memory fragmentation issues
}

void loop() {
  // put your main code here, to run repeatedly:
  long distanceSensorOne;

  clearOutgoingBuffer();

  scanSurroundings();

  sendDataToSerial();
}

/*
 * Makes one full 360 scan
 */
void scanSurroundings()
{
  long distance, distance2;

  for(int iteration = 0; iteration < rotationIncrementLimit; iteration++) {
    turnSensor();

    distance = getReading(uOneTrigPin, uOneEchoPin);
    distance2 = getReading(uTwoTrigPin, uTwoEchoPin);
  
    blipsMessage += encodeData(distance);
    blipsMessage += "|";

    blipsMessage += encodeData(distance2);
    blipsMessage += ",";
  }
}

String encodeData(long distance)
{
    return String(distance);
  //return String(currentDirection) + "/" + String(distance);
}

/*
 * Turns the sensor one increment
 */
void turnSensor()
{
  // Code for incrementing the servo
  // We want %rotationIncrementLimit% increments to bring it back to

  int scaledVal = currentDirection * 15; //scale to servo coordinates 
  //myServo.write(scaledVal);
  delay(100);
  
  currentDirection = (currentDirection + 1) % rotationIncrementLimit; //rotate it like a clock
}

/*
 * Captures the distance reading in the current direction
 */
long getReading(int trigPin, int echoPin)
{
  long distance = getDistance(trigPin, echoPin);

  //printDistance(distance);

  return distance;
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
    digitalWrite(uOneLEDPin, LOW);
    
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
    digitalWrite(uOneLEDPin, HIGH);
  }
}

void sendDataToSerial()
{
  Serial.print(blipsMessage);
  Serial.println();                   //send line ending to complete serial message
}

void clearOutgoingBuffer()
{
  blipsMessage = " , , , , , , , , , , , , , , , , , , ,";         //send all empty characters to refresh cordoba data
  sendDataToSerial();                           //send directly to serial
  blipsMessage = "";                            //reset to empty string
}

