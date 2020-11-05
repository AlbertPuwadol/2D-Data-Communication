#include <Adafruit_MCP4725.h>
#include<Wire.h>
Adafruit_MCP4725 dac;

#define r_slope 100
int p[4] = {400, 800, 1200, 1600};
int cycle[4] = {0,0,0,0};
unsigned long timer = 0;
unsigned long a = 0;
unsigned long b = 0;
int bits = 8;
int count = 0;
int temp;
int prev;
int s = 0;
unsigned long delays = 400;
boolean startTimer = true;

void setup() {
  /* set buadrate serial is 115200 */
  Serial.begin(115200);
  // delay for sampling period of sine
  // (Tsine – delayfrom DAC processing speed)
  Serial.flush(); // for clear buffer serial
  for(int i = 0;i < 4;i++){
    cycle[i] = p[i]/80;
    Serial.println(cycle[i]);
  }  
}

void loop() {
  int tmp = analogRead(A0); // read signal from analog pin
  if (prev - 300 > r_slope && 300 - tmp > r_slope) {
    count++;
    //Serial.println(a);
    delays = (delays+a)/2;
    //Serial.print("a ");
    //Serial.println(a);
    //Serial.println(b);

    if (startTimer == true) {
      timer = micros();
      startTimer = false;
    }

  }

  prev = tmp;

  if (micros() - timer >  12500 && startTimer == false) {
    Serial.println(count);
    if (count == cycle[0]) {
      Serial.print("0 0 ");
      bits = bits - 2;
    }
    else if (count == cycle[1]) {
      Serial.print("0 1 ");
      
      temp = 1;
      for (int rounds = 2; rounds < bits; rounds++)  temp *= 2;
      s += temp;
      
      bits = bits - 2;
    }
    else if (count == cycle[2]) {
      Serial.print("1 0 ");
      
      temp = 1;
      for (int rounds = 1; rounds < bits; rounds++)  temp *= 2;
      s += temp;
      
      bits = bits - 2;
    }
    else if (count == cycle[3]) {
      Serial.print("1 1 ");
      
      temp = 1;
      for (int rounds = 1; rounds < bits; rounds++)  temp *= 2;
      s += temp;
      temp = 1;
      for (int rounds = 2; rounds < bits; rounds++)  temp *= 2;
      s += temp;
      
      bits = bits - 2;
    }
    count = 0;
    timer = micros();
    startTimer = true;
  }

  if (bits == 0) {
    //Serial.print("s");
    Serial.println(char(s));
    s = 0;
    bits = 8;
  }
}
