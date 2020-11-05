#include <Adafruit_MCP4725.h>
#include<Wire.h>
Adafruit_MCP4725 dac;
#define defaultFreq 1700 //DAC speed (Hz)
/*freq0 : frequency of carrier sine wave (Hz)*/
#define freq0 200
int delay0;
/*A[0]-A[3] : ASK Amplitude (0,5] (V)*/
/*S_DAC : Amplitude (12bit) of sine wave at 0,90,180,270*/
#define a0min 164
#define a0max 245
#define a1min 368
#define a1max 450
#define a2min 573
#define a2max 655
#define a3min 777
#define a3max 859

#define r_slope 80

int s = 0;
int maxs = 0;
int prev = 0;
int check = false;
int output = -1;
int count = 0;
int temp = 1;
int wave = 0;

void setup() {
  /* set buadrate serial is 115200 */
  Serial.begin(115200);
  delay0 = (1000000 / freq0 - 1000000 / defaultFreq) / 4;
  // delay for sampling period of sine
  // (Tsine – delayfrom DAC processing speed)
  Serial.flush(); // for clear buffer serial
}

void loop() {
  int tmp = analogRead(A5); // read signal from analog pin
  Serial.println(tmp);
  if (tmp - prev > r_slope && check == false) {
    maxs = 0;
    check = true; // change check status is true
  }
  if (tmp > maxs) { // update max value
    maxs = tmp;
  }
  if (maxs - tmp > r_slope) { // check for falling signal
    if (check == true) {
      if (a0min < maxs && maxs < a0max) {
        if (wave % 5 == 0)
        {
          Serial.print("0 0 ");
          count = count + 2;
        }
        wave++;
      }
      else if (a1min < maxs && maxs < a1max) {
        if (wave % 5 == 0)
        {
          /*
          Serial.print("0 1 ");
          temp = 1;
          for (int rounds = 1; rounds <= count; rounds++)  temp *= 2;
          s += temp;
          */
          count = count + 2 ;
          
        }
        wave++;
      }
      else if (a2min < maxs && maxs < a2max) {
        if (wave % 5 == 0)
        {
          Serial.print("1 0 ");
          /*
          temp = 1;
          for (int rounds = 0; rounds <= count; rounds++)  temp *= 2;
          s += temp;
          */
          count = count + 2 ;
          
        }
        wave++;
      }
      else if (a3min < maxs && maxs < a3max) {
        if (wave % 5 == 0)
        {
          Serial.print("1 1 ");
          /*
          temp = 1;
          for (int rounds = 0; rounds <= count; rounds++)  temp *= 2;
          s += temp;

          temp = 1;
          for (int rounds = 1; rounds <= count; rounds++)  temp *= 2;
          s += temp;
          */
          count = count + 2 ;
          
        }
        wave++;
      }
      if (count == 8) {
        //Serial.println(char(s));
        count = 0;

        s = 0;

      }
    }
    check = false; // change check status is false
  }
  prev = tmp; // assign temp value to previous
}
