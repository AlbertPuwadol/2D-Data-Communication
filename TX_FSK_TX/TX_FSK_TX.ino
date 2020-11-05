#include <Adafruit_MCP4725.h>
#include <Wire.h>
Adafruit_MCP4725 dac;
/*freq0 : frequency of carrier sine wave (Hz)*/
/*A[0]-A[3] : ASK Amplitude (0,5] (V)*/
/*S_DAC : Amplitude (12bit) of sine wave at 0,90,180,270*/
int frequency[4] = {500, 750, 1000, 1250};
const uint16_t S_DAC[4] = {2048, 4095, 2048, 0};
int delays[4] = {0, 0, 0, 0};        // 2000,1333,1000,800 microsec
int cycle[4] = {0, 0, 0, 0};
char inData[100];

void setup( ) {
  /* set buadrate serial is 115200 */
  Serial.begin(115200);
  dac.begin(0x62); // set to default
  // delay for sampling period of sine
  // (Tsine – delayfrom DAC processing speed)
  Serial.flush(); // for clear buffer serial
  for(int i = 0;i < 4;i++){
    cycle[i] = frequency[i] / 125;
    delays[i] = (1000000/frequency[i] - 1000000/1700)/4;
  }
}


void loop( ) {
  if (Serial.available() > 0) {
    /*use a cycle loop receive inData : message input */
    for (int i = 0; i == 0 || inData[i - 1] != 0xFFFFFFFF; i++) {
      inData[i] = Serial.read(); // Read a character
    }
    /*use a cycle loop i for send data 8 bits*/
    for (int i = 0; inData[i] != 0xFFFFFFFF; i++ ) {
      /*
        use a cycle loop k for 1 ASK signal element (2 bit)
        - map inData[i] to tmp (2 bit)
        - from LSB to MSB
      */
      for (int k = 7; k >= 0; k -= 2) {
        int tmp = inData[i] & 3; // 00, 01, 10, 11
        /*use a cycle loop sl to send 5 cycle/baud*/
        for (int f = 0; f < cycle[tmp]; f++) {
          for (int sl = 0; sl < 4; sl++ ) {
            /*
              Use the selected amplitude above to modify
              sine amplitude
            */
            dac.setVoltage(S_DAC[sl], false);
            delayMicroseconds(delays[tmp]); // sampling delay
          }
        }
        inData[i] >>= 2;
      }
    }
    dac.setVoltage(0, false); // for don't send
  }
}
