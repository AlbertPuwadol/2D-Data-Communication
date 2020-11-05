#include <Adafruit_MCP4725.h>
#include<Wire.h>
Adafruit_MCP4725 dac;


#define f0min 1750
#define f1min 1080
#define f2min 910
#define f3min 700
#define f0max 2250
#define f1max 1580
#define f2max 1100
#define f3max 900

#define r_slope 5
int count = 0;
int delays[4] = {500, 333, 250, 200};
unsigned long start = 0;
unsigned long finish = 0;
int data = -1;
int bits = 0;
int temp;
int s;

void setup() {
  /* set buadrate serial is 115200 */
  Serial.begin(115200);
  dac.begin(0x62); // set to default
  // delay for sampling period of sine
  // (Tsine – delayfrom DAC processing speed)
  Serial.flush(); // for clear buffer serial
}

void loop() {
  int tmp = analogRead(A0); // read signal from analog pin
  if (tmp - 1000 > r_slope micros - b > 500) {
    count++;
    b = micros
    if (count == 1) {
      start  = micros();
      delayMicroseconds(delays[0]);
    }
    else if (count == 2) {
      finish = micros();
      if (finish - start > f0min && finish - start  < f0max) {
        data = 0;
      }
      else if (finish - start > f1min && finish - start  < f1max) {
        data = 1;
      }
      else if (finish - start > f2min && finish - start  < f2max) {
        data = 2;
      }
      else if (finish - start > f3min && finish - start  < f3max) {
        data = 3;
      }
      delayMicroseconds(delays[data]);
    }
    else
      delayMicroseconds(delays[data]);
  }

  if (data == 0) {
    if (count == 2) {
      Serial.print("0 0 ");
      count = 0;
      bits = bits + 2;
      data = -1;
    }
  }
  else if (data == 1) {
    if (count == 3) {
      Serial.print("0 1 ");
      temp = 1;
      for (int rounds = 1; rounds <= bits; rounds++)  temp *= 2;
      s += temp;
      count = 0;
      bits = bits + 2;
      data = -1;
    }
  }
  else if (data == 2) {
    if (count == 4) {
      Serial.print("1 0 ");
      temp = 1;
      for (int rounds = 0; rounds <= bits; rounds++)  temp *= 2;
      s += temp;
      count = 0;
      bits = bits + 2;
      data = -1;
    }
  }
  else if (data == 3) {
    if (count == 5) {
      Serial.print("1 1 ");
      temp = 1;
      for (int rounds = 0; rounds <= bits; rounds++)  temp *= 2;
      s += temp;
      temp = 1;
      for (int rounds = 1; rounds <= bits; rounds++)  temp *= 2;
      s += temp;
      count = 0;
      bits = bits + 2;
      data = -1;
    }
  }

  if (bits == 8) {
    Serial.println(char(s));
    s = 0;
    bits = 0;
  }

}
