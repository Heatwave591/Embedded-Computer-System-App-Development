#define led1 2
#define led2 3
#define led3 4
#define led4 5
#define PR A0

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
}

void loop() {
  int analogValue = analogRead(PR);


  Serial.print("Analog Value = ");
  Serial.print(analogValue);   

  if (analogValue < 50) {
    Serial.println(" => Dark");
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);

  } else if (analogValue >= 50 && analogValue < 100) {
    Serial.println(" => Dim");
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);

  } else if (analogValue >= 100 && analogValue < 300) {
    Serial.println(" => Light");
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);

  } else if (analogValue >= 300 && analogValue < 450) {
    Serial.println(" => Bright");
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, LOW);
  } else if(analogValue > 450)  {
    Serial.println(" => Very bright");
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
  }

  delay(500);
}
