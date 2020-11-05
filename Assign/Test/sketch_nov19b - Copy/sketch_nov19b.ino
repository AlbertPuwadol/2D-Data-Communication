#include <Servo.h>
Servo myServo;
char order;
void rotateR45() {
  int detect = myServo.read();
  for (int i = detect; i > detect - 45; i--) {
    myServo.write(i);
    delay(50);
  }
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
  pinMode(9, OUTPUT);
  pinMode(10, INPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  myServo.attach(13);
  goStart();
  Serial.print("Start");
}

void loop() {
  if (Serial.available() > 0) {
    order = Serial.read();
  }
  if (order == 's') {//start
    // already at L
    if (myServo.read() <= 50) {
      goStart();
      Serial.print("f");
    }
    else {
      rotateR45();
      Serial.print("c");
    }
  }
}
