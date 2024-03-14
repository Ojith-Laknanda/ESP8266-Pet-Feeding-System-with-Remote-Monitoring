#include <Servo.h>
Servo myservo;
int servoPos = 90; // Initial position for the servo (90 degrees)


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_TEMPLATE_ID "TMPL6z6JT_5Io"
#define BLYNK_TEMPLATE_NAME "project"
#define BLYNK_AUTH_TOKEN "hQofbJM27OquDxrnO0zrF-YLDOnMkdkg"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "SLT Fibre Ojiya";
char pass[] = "2001012@Laknanda";

#define waterLevelSensorPin A0  // Analog input pin for the water level sensor
#define relayPin D8              // Digital output pin for the relay module


#define trigPin1 D0 // Trigger pin for the first ultrasonic sensor
#define echoPin1 D1 // Echo pin for the first ultrasonic sensor
#define trigPin2 D2 // Trigger pin for the second ultrasonic sensor
#define echoPin2 D3 // Echo pin for the second ultrasonic sensor
#define ledPin D4   // LED for indicating measurements

#define irSensorPin D6  // Pin connected to the IR sensor's output
#define servoPin D7     // Pin connected to the servo motor


void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN,ssid, pass,"Blynk.cloud",8080);
  myservo.attach(servoPin);
  pinMode(irSensorPin, INPUT);

  pinMode(waterLevelSensorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);  // Initially, turn off the relay

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {

  int waterLevel = analogRead(waterLevelSensorPin);

  // Calculate the percentage of water level (adjust the values accordingly)
  int waterPercentage = map(waterLevel, 0, 1023, 0, 100);

  // Print the water percentage to the serial monitor
  Serial.print("Water Level Percentage: ");
  Serial.print(waterPercentage);
  Serial.println("%");

  // Check if the water level is at or above 50%
  if (waterPercentage <= 50) {
    digitalWrite(relayPin, HIGH);  // Turn on the relay
    Serial.println("Relay ON");
  } else {
    digitalWrite(relayPin, LOW);  // Turn off the relay
    Serial.println("Relay OFF");
  }
  

  int irSensorValue = digitalRead(irSensorPin);

  if (irSensorValue == LOW) {
    // pet detected by the IR sensor
    Serial.println("pet detected.");
    servoPos = 180; // Rotate the servo to 180 degrees 
    myservo.write(servoPos);
    delay(1000); // Delay to keep the servo in the new position
  } 
    Serial.println("No pet detected.");
    servoPos = 90; // Return the servo to its initial position (90 degrees)
    myservo.write(servoPos);
  
  
  long duration1, distance1, duration2, distance2;

  // First Ultrasonic Sensor for measuring the percentage of water level
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1 / 2) / 29.1; // Calculate distance in centimeters
   Blynk.virtualWrite(V0, distance1);  

  // Second Ultrasonic Sensor for measuring the percentage of food level
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2 / 2) / 29.1; // Calculate distance in centimeters
  

  // Print the distances to the serial monitor
  Serial.print("Distance Sensor 1: ");
  Serial.print(distance1);
  Serial.print(" cm\tDistance Sensor 2: ");
  Serial.print(distance2);
  Serial.println(" cm");
   Blynk.virtualWrite(V1, distance2); 
  for(int i=0;i<=100;i++){
    Blynk.virtualWrite(V0, i); 
    Blynk.virtualWrite(V1, i);
    delay(1000);
  }
  for(int i=100;i>0;i--){
    Blynk.virtualWrite(V0, i); 
    Blynk.virtualWrite(V1, i);
    delay(1000);
  }
   
  
  delay(100);
  Blynk.run();
}
