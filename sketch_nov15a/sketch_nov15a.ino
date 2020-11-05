#include <Adafruit_MCP4725.h>
#include<Wire.h>
Adafruit_MCP4725 dac;

#define f0min 2400
#define f0max 2600
#define f1min 1150
#define f1max 1350
#define f2min 750
#define f2max 950
#define f3min 525
#define f3max 725

#define r_slope 20
int count = 0;
int frequency[4] = {500, 750, 1000, 1250};
int delays[4] = {0, 0, 0, 0};
int cycle[4] = {0, 0, 0, 0};
unsigned long start = 0;
unsigned long period = 0;
unsigned long a = 0;
unsigned long b = 0;
int data = 0;
int bits = 8;
int temp;
int s;


void setup() {
  /* set buadrate serial is 115200 */
  Serial.begin(115200);
  dac.begin(0x62); // set to default
  // delay for sampling period of sine
  // (Tsine – delayfrom DAC processing speed)
  Serial.flush(); // for clear buffer serial
  for (int i = 0; i < 4; i++) {
    delays[i] = (1000000 / frequency[i]) / 4;
    //Serial.println(delays[i]);
    cycle[i] = frequency[i] / 250;          // 250 is baud rate
    //Serial.println(cycle[i]);
  }
}

void loop() {
  int tmp = analogRead(A0); // read signal from analog pin
  a = micros() - b;
  if (tmp - 300 > r_slope && a > delays[data]) {
    count++;
    //Serial.println(count);
    b = micros();
    //Serial.println(a);
    if (count == 1) {
      start  = micros();
    }
    else if (count == 2) {
      period = micros() - start;
      //Serial.println(period);
      if (period > f0min && period  < f0max) {
        data = 0;
      }
      else if (period > f1min && period  < f1max) {
        data = 1;
      }
      else if (period > f2min && period  < f2max) {
        data = 2;
      }
      else if (period > f3min && period  < f3max) {
        data = 3;
      }
    }
  }

  if (count == cycle[data]) {
    if (data == 0) {
      Serial.print("0 0 ");
    }
    else if (data == 1) {
      Serial.print("0 1 ");
      temp = 1;
      for (int rounds = 2; rounds < bits; rounds++)  temp *= 2;
      s += temp;
    }
    else if (data == 2) {
      Serial.print("1 0 ");
      temp = 1;
      for (int rounds = 1; rounds < bits; rounds++)  temp *= 2;
      s += temp;
    }
    else if (data == 3) {
      Serial.print("1 1 ");
      temp = 1;
      for (int rounds = 1; rounds < bits; rounds++)  temp *= 2;
      s += temp;
      temp = 1;
      for (int rounds = 2; rounds < bits; rounds++)  temp *= 2;
      s += temp;
    }
    count = 0;
    bits = bits - 2;
  }

  if (bits == 0) {
    Serial.println(char(s));
    data = 0;
    s = 0;
    bits = 8;
  }
}
