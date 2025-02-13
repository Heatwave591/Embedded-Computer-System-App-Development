// #define IR_SENSOR_PIN 23
// #define LED 22

// void setup() {
// Serial.begin(115200);         
// pinMode(IR_SENSOR_PIN, INPUT);
// pinMode(LED, OUTPUT);
// }

// void loop() {
// int sensorState = digitalRead(IR_SENSOR_PIN); 
// if(IR_SENSOR_PIN == 0){
//   Serial.println("Object detected");
//   digitalWrite(LED, HIGH);
// }

// else{
//   Serial.println("nothing");
//   digitalWrite(LED,LOW);
// }
// delay(1000);

// }

const int sensorPin = 9;
const int led1 = 3;
// const int led2 = 3;
// const int led3 = 4;
// const int led4 = 5;
// const int led5 = 6;
// const int led6 = 7;

void setup() {
  Serial.begin(115200);

  pinMode(sensorPin, INPUT);
  pinMode(led1, OUTPUT);
  // pinMode(led2, OUTPUT);
  // pinMode(led3, OUTPUT);
  // pinMode(led4, OUTPUT);


}

void loop() {
  int sensorValue = digitalRead(sensorPin);

  if (sensorValue == LOW) {
    Serial.println("Object detected");
    digitalWrite(led1, HIGH);
    // digitalWrite(led2, HIGH);
    // digitalWrite(led3, HIGH);
    // digitalWrite(led4, HIGH);
    // digitalWrite(led5, HIGH);
    // digitalWrite(led6, HIGH);

  } 
  
  else {
  Serial.println("No object detected");
    digitalWrite(led1, LOW);
    // digitalWrite(led2, LOW);
    // digitalWrite(led3, LOW);
    // digitalWrite(led4, LOW);
    // digitalWrite(led5, LOW);
    // digitalWrite(led6, LOW);
}

delay(1000);
}