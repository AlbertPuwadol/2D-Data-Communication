#include <Wire.h>
#include <Adafruit_MCP4725.h>


#define defaultFreq 1700 // dac speed (Hz)
#define baudRate 100

const int num_Freq = 4;
const int freq[num_Freq] = {200, 300, 400, 500};
int delay_Freq[num_Freq];

/*S_DAC : Amplitude (12bit) of sine wave at 0,90,180,270*/
const int deg = 4;
float S_DAC[deg];
float zeta[deg];
float S[deg];

Adafruit_MCP4725 dac;

void setup() {
  Serial.begin(115200);
  dac.begin(0x62);

  for (int i = 0; i < num_Freq; i++) {
    delay_Freq[i] = (1000000 / freq[i]) / deg - 147;
    Serial.print("Freq : ");
    Serial.print(freq[i]);
    Serial.print(" Delay : ");
    Serial.print(delay_Freq[i]);
    Serial.print(" Cycle : ");
    Serial.println(freq[i] / baudRate);
  }

  for (int i = 0 ; i < deg ; i++) {
    zeta[i] = i * 360 / deg;
    S[i] = sin(zeta[i] * PI / 180);
    S_DAC[i] = map(S[i] * 1000, -1000, 1000, 2000, 2800);
    Serial.print(S[i]) ;
    Serial.print(" ") ;
    Serial.println(S_DAC[i]) ;
  }
  dac.setVoltage(2400, false); // for don't send
  Serial.flush(); // for clear buffer serial
}

void loop() {
  if (Serial.available() > 0) { // for get input
    delay(5);
    int in = Serial.read(); // get Dec from Serial
    int input[4];
    input[0] = (in >> 0) & 3;
    input[1] = (in >> 2) & 3;
    input[2] = (in >> 4) & 3;
    input[3] = (in >> 6) & 3;
    for (int k = 3 ; k >= 0 ; k--) {
      dac.setVoltage(2400, false);
      delay(3);
      for (int cycle = 0; cycle < (freq[input[k]] / baudRate); cycle++) {
        for (int sl = 0; sl < deg; sl++) {
          dac.setVoltage(S_DAC[sl], false);
          delayMicroseconds(delay_Freq[input[k]]);
        }
      }
    }
    dac.setVoltage(2400, false); // for don't send
  }
}
