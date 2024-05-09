#include <Servo.h>
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

// Include the ultrasonic sensor library
#include <Ultrasonic.h>
// Insert your network credentials
#define WIFI_SSID "LAVA"
#define WIFI_PASSWORD "123456789"

// Insert Firebase project API Key
#define API_KEY "AIzaSyA4ez6GPL6kuECR1hrhL_9pEPLZwigD8as"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "imtiyazuddin959@gmail.com"
#define USER_PASSWORD "Dsc@1234"

// Insert RTDB URL
#define DATABASE_URL "https://distance-measurement-4a0c6-default-rtdb.asia-southeast1.firebasedatabase.app/"
// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Variables to save database paths
String databasePath;
String distancePath;

// Ultrasonic sensor pins
#define trigPin 16   //D0
#define echoPin 13   //D7
Ultrasonic ultrasonic(trigPin, echoPin);

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 1000;

#define sensor A0
int  ir =14;
int inductive = 12;
Servo servo;
int motorPin1 = 5;  // Blue   - 28BYJ48 pin 1
int motorPin2 = 4;  // Pink   - 28BYJ48 pin 2
int motorPin3 = 0; // Yellow - 28BYJ48 pin 3
int motorPin4 = 2;
int motorSpeed = 1;

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Write float values to the database
void sendFloat(String path, float value){
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)){
    Serial.print("Writing value: ");
    Serial.print (value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

float calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH);
  while(digitalRead(echoPin) == HIGH);
  // Speed of sound is approximately 343 meters per second (or 0.0343 cm/microsecond)
  float distance = (duration * 0.0343) / 2.0; // Divide by 2 because the sound travels to the object and back
  
  return distance;
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(ir,INPUT);
  pinMode(inductive,INPUT);
  servo.attach(15);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  initWiFi();

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid;

  // Update database path for sensor readings
  distancePath = databasePath + "/distance"; // --> UsersData/<user_uid>/distance

}

void clockwise (){
 // 1
 digitalWrite(motorPin1, HIGH);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin4, LOW);
 delay(motorSpeed);
 // 2
 digitalWrite(motorPin1, HIGH);
 digitalWrite(motorPin2, HIGH);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin4, LOW);
 delay (motorSpeed);
 // 3
 digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, HIGH);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin4, LOW);
 delay(motorSpeed);
 // 4
 digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, HIGH);
 digitalWrite(motorPin3, HIGH);
 digitalWrite(motorPin4, LOW);
 delay(motorSpeed);
 // 5
 digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin3, HIGH);
 digitalWrite(motorPin4, LOW);
 delay(motorSpeed);
 // 6
 digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin3, HIGH);
 digitalWrite(motorPin4, HIGH);
 delay (motorSpeed);
 // 7
 digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin4, HIGH);
 delay(motorSpeed);
 // 8
 digitalWrite(motorPin1, HIGH);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin4, HIGH);
 delay(motorSpeed);
}

//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 4 to 1
//delay "motorSpeed" between each pin setting (to determine speed)

void counterclockwise(){
 // 1
 digitalWrite(motorPin4, HIGH);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin1, LOW);
 delay(motorSpeed);
 // 2
 digitalWrite(motorPin4, HIGH);
 digitalWrite(motorPin3, HIGH);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin1, LOW);
 delay (motorSpeed);
 // 3
 digitalWrite(motorPin4, LOW);
 digitalWrite(motorPin3, HIGH);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin1, LOW);
 delay(motorSpeed);
 // 4
 digitalWrite(motorPin4, LOW);
 digitalWrite(motorPin3, HIGH);
 digitalWrite(motorPin2, HIGH);
 digitalWrite(motorPin1, LOW);
 delay(motorSpeed);
 // 5
 digitalWrite(motorPin4, LOW);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin2, HIGH);
 digitalWrite(motorPin1, LOW);
 delay(motorSpeed);
 // 6
 digitalWrite(motorPin4, LOW);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin2, HIGH);
 digitalWrite(motorPin1, HIGH);
 delay (motorSpeed);
 // 7
 digitalWrite(motorPin4, LOW);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin1, HIGH);
 delay(motorSpeed);
 // 8
 digitalWrite(motorPin4, HIGH);
 digitalWrite(motorPin3, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(motorPin1, HIGH);
 delay(motorSpeed);
}


int irfun(){
  int ir_value=digitalRead(ir);
  return ir_value;
}

int inductivefun(){
  int inductive_value =digitalRead(inductive);
  return inductive_value;
}

int moisture_sensor(){
  int val = analogRead(sensor);
  Serial.println("Analog Output: ");
  Serial.println(val);
  delay(500);
  return val;
}

void finalvalue(){
  // Send new readings to database
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    // Get latest sensor readings
    float distance = calculateDistance();
    Serial.println(distance);

    // Send reading to database:
    sendFloat(distancePath, distance);
}
}
void loop() {
  // put your main code here, to run repeatedly:
  servo.write(180);
  
  int ir = irfun();
  if(ir==0){
      Serial.println("waste aaya");
      int inductive = inductivefun();
      if(inductive==0){
        Serial.println("waste is metal");
        for(int i = 0; i < 171; i++)
          counterclockwise();     
        delay(2000); // rotate slowly from 0 degrees to 180 degrees, one by one degree
        servo.write(-180); 
        delay(5000); // control servo to go to position in variable 'pos'
        servo.write(180);
        delay(1000);
        for(int n = 0; n < 171; n++) // good
          clockwise();
        delay(2000);

        finalvalue();
        }



      else
      { 
       int val=moisture_sensor();
       Serial.println(val);
        if(val<900){
          
          Serial.println("waste is wet");
          
          for(int n = 0; n < 171; n++) // good
            clockwise();
          delay(5000);
          servo.write(-180);
          delay(5000);
          servo.write(180);
          delay(5000);
          for(int i = 0; i < 171; i++)
            counterclockwise();     
          delay(2000);

          finalvalue();
      }
        else{
          Serial.println("waste is dry");
          delay(5000);
          servo.write(-180);
          delay(5000);
          servo.write(180);
          delay(1000);

          finalvalue();
        } 
      } 

    }  
  else{
      Serial.println("khuda daal");
      delay(1000);
    }

}
