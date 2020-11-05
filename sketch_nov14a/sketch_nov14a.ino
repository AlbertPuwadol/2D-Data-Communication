#include <Adafruit_MCP4725.h>
#include<Wire.h>
Adafruit_MCP4725 dac;

#define p0min 2400
#define p0max 2600
#define p1min 1150
#define p1max 1350
#define p2min 740
#define p2max 940
#define p3min 550
#define p3max 750

#define r_slope 20
int count = 0;
int frequency[4] = {400, 800, 1200, 1600};
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
    cycle[i] = frequency[i] / 80;          // 250 is baud rate
    //Serial.println(cycle[i]);
  }
}

void loop() {
  int tmp = analogRead(A0); // read signal from analog pin
  a = micros() - b;
  if (tmp - 1000 > r_slope && a > delays[data]) {
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
      if (period > p0min && period  < p0max) {
        data = 0;
      }
      else if (period > p1min && period  < p1max) {
        data = 1;
      }
      else if (period > p2min && period  < p2max) {
        data = 2;
      }
      else if (period > p3min && period  < p3max) {
        data = 3;
      }
    }
  }

  if (count == cycle[data]) {
    if (data == 0) {
      Serial.print("0 0 ");
      count = 0;
      bits = bits - 2;
      data = 0;
    }
    else if (data == 1) {
      Serial.print("0 1 ");
      temp = 1;
      for (int rounds = 2; rounds < bits; rounds++)  temp *= 2;
      s += temp;
      count = 0;
      bits = bits - 2;
      data = 0;
    }
    else if (data == 2) {
      Serial.print("1 0 ");
      temp = 1;
      for (int rounds = 1; rounds < bits; rounds++)  temp *= 2;
      s += temp;
      count = 0;
      bits = bits - 2;
      data = 0;
    }
    else if (data == 3) {
      Serial.print("1 1 ");
      temp = 1;
      for (int rounds = 1; rounds < bits; rounds++)  temp *= 2;
      s += temp;
      temp = 1;
      for (int rounds = 2; rounds < bits; rounds++)  temp *= 2;
      s += temp;
      count = 0;
      bits = bits - 2;
      data = 0;
    }
  }

  if (bits == 0) {
    Serial.println(char(s));
    data = 0;
    s = 0;
    bits = 8;
  }
}
