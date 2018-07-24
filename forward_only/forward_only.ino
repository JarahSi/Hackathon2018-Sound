
#define uOneTrigPin 12
#define uOneEchoPin 11

#define vibratorPin 3

const int promptDistance = 200;
String blipsMessage = "";         //variable to hold the blips

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode(uOneTrigPin, OUTPUT);
  pinMode(uOneEchoPin, INPUT);
  pinMode(vibratorPin, OUTPUT);

  blipsMessage.reserve(200);        //reserve 200 bytes to avoid memory fragmentation issues
}

void loop() {
  // put your main code here, to run repeatedly:
  clearOutgoingBuffer();

  scanSurroundings();

  sendDataToSerial();
}

/*
 * Makes one full 360 scan
 */
void scanSurroundings()
{
  long distance;

  // Get readings from both sensors
  distance = getDistance(uOneTrigPin, uOneEchoPin);

  // Front sensor
  blipsMessage = encodeData(distance);

  // Ping if an object is close by
  if (isInRange(distance)) {
      vibrate(distance);
  }
  else {
    vibrate(promptDistance + 1);
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
 * Captures distance to the nearest object in front of sensor
 */
int getDistance(int trigPin, int echoPin)
{
  long distance, distanceTwo, duration, durationTwo;

  digitalWrite(trigPin, LOW);  // Added this line
  
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  return distance;
}

/*
 * Debug print for distance
 */
void printDistance(long distance)
{
  if (isInRange(distance)){
    Serial.println("Out of range");
    
    
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
  }
}

/*
 * Helper function 
 */
bool isInRange(long distance)
{
  return (distance < promptDistance && distance > 0);
}

void vibrate(long distance)
{
  int vibrateSpeed;

  if (distance > promptDistance || distance < 0) {
    vibrateSpeed = 0;
  }
  else {
    vibrateSpeed = 255 - distance;
  }

  analogWrite(vibratorPin, vibrateSpeed);
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

