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

const int sensorPin = 23;
const int led1 = 22;
const int led2 = 21;
const int led3 = 19;
const int led4 = 18;
void setup() {
  Serial.begin(115200);

  pinMode(sensorPin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);


}

void loop() {
  int sensorValue = digitalRead(sensorPin);

  if (sensorValue == LOW) {
    Serial.println("Object detected");
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);

  } 
  
  else {
  Serial.println("No object detected");
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
}

delay(1000);
}