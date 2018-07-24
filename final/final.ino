#include <Servo.h>


#define uOneTrigPin 12
#define uOneEchoPin 11
#define uTwoTrigPin 10
#define uTwoEchoPin 9

#define uOneLEDPin 2
#define servoPin 3

//const bool servoOn = true;
const bool servoOn = false;

Servo myServo;

const int rotationIncrementLimit = 5; //number of rotational increments before a full 360 rotation
int currentDirection = 0;         //variable to hold the current direction sensor is facing

String blipsMessage = "";         //variable to hold the blips
const int charLimit = rotationIncrementLimit * 2;         //blipsMessage character limit

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
  clearOutgoingBuffer();

  scanSurroundings();

  sendDataToSerial();

  resetPosition();
}

/*
 * Makes one full 360 scan
 */
void scanSurroundings()
{
  long distance, distance2;

  for(int iteration = 0; iteration < rotationIncrementLimit; iteration++) {
    turnSensor();

    getDistance();
  }
}

void resetPosition()
{
  if (servoOn) {
    for(int pos = 180; pos >= 0; pos = pos - 9) {
        myServo.write(pos);
        delay(50);
    }
  }
}

/*
 * Prepares the current distance value for output to excel
 */
String encodeData(long distance)
{
  return String(distance);
}

/*
 * Turns the sensor one increment
 */
void turnSensor()
{
  // Code for incrementing the servo
  // We want %rotationIncrementLimit% increments to bring it back to

  int scaledVal = currentDirection * (180 / rotationIncrementLimit); //scale to servo coordinates 

  if (servoOn) {
    myServo.write(scaledVal);
    delayMicroseconds(30);
  }

  currentDirection = (currentDirection + 1) % rotationIncrementLimit; //rotate it like a clock
}

/*
 * Captures distance to the nearest object in front of sensor
 */
void getDistance()
{
  long distance, distanceTwo, duration, durationTwo;

  digitalWrite(uOneTrigPin, LOW);  // Added this line
  digitalWrite(uTwoTrigPin, LOW);
  
  delayMicroseconds(2); // Added this line
  digitalWrite(uOneTrigPin, HIGH);
  digitalWrite(uTwoTrigPin, HIGH);

  delayMicroseconds(10); // Added this line
  digitalWrite(uOneTrigPin, LOW);
  digitalWrite(uTwoTrigPin, LOW);
  duration = pulseIn(uOneEchoPin, HIGH);
  durationTwo = pulseIn(uTwoEchoPin, HIGH);
  distance = (duration/2) / 29.1;
  distanceTwo = (durationTwo/2) / 29.1;

  // Front sensor
  blipsMessage += encodeData(distance);
  blipsMessage += ",";

  // Back sensor
  blipsMessage += encodeData(distanceTwo);
  blipsMessage += ",";

  if (isInRange(distance) || isInRange(distanceTwo)) {
      digitalWrite(uOneLEDPin, LOW);
  }
  else {
    digitalWrite(uOneLEDPin, HIGH);
  }

  return distance;
}

/*
 * Debug print for distance
 */
void printDistance(long distance)
{
  if (isInRange(distance)){
    Serial.println("Out of range");
    digitalWrite(uOneLEDPin, LOW);
    
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
    digitalWrite(uOneLEDPin, HIGH);
  }
}

/*
 * Helper function 
 */
bool isInRange(long distance)
{
  return (distance >= 200 || distance <= 0);
}

void sendDataToSerial()
{
  Serial.print(blipsMessage);
  Serial.println();                   //send line ending to complete serial message
}

void clearOutgoingBuffer()
{
  //blipsMessage = " , , , , , , , , , , , , , , , , , , ,";         //send all empty characters to refresh cordoba data
  //sendDataToSerial();                           //send directly to serial
  blipsMessage = "";                            //reset to empty string
}

