#include <Servo.h>
Servo myServo;
char order;
void rotateR45() {
  int detect = myServo.read();
  for (int i = detect; i > detect - 45; i--) {
    myServo.write(i);
    delay(50);
  }
  Serial.print("c");
}
void rotateAll45(int dest) {
  int detect = myServo.read();
  if (detect <= dest) {
    for (int i = detect; i <= dest; i++) {
      myServo.write(i);
      delay(50);
    }
  }
  else {
    for (int i = detect; i > dest; i--) {
      myServo.write(i);
      delay(50);
    }
  }
  Serial.print("c");
}
void goStart() {
  if (myServo.read() <= 135) {
    for (int i = myServo.read(); i <= 135; i++) {
      myServo.write(i);
      delay(50);

    }
  }
  else {
    for (int i = myServo.read(); i > 135; i--) {
      myServo.write(i);
      delay(50);
    }
  }

  order = '\0';


}
void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, INPUT);
  myServo.attach(13);
  goStart();
  //Serial.print("Start");
  digitalWrite(2, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    digitalWrite(2, HIGH);
    order = Serial.read();
  }
  else
    digitalWrite(2, LOW);

  if (order == 's') {//start
    //
    if (myServo.read() <= 50) {
      goStart();
      //Serial.print("f");
    }
    else {
      rotateR45();
    }
  }
  else if (order == 'l'){
    rotateAll45(135);
  }
  else if (order == 'm'){
    rotateAll45(90);
  }
  else if (order == 'r'){
    rotateAll45(45);
  }
  else if (order == 'e'){
    goStart();
  }

}
