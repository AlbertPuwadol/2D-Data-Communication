#include <Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;
unsigned int delay0;
#define defaultFreq 1700
#define freq 500 

const int deg = 8;
float zeta[deg];
float S[deg];
float S_DAC[deg];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dac.begin(0x62);
  //delay0=(1000000/freq - 1000000/defaultFreq)/deg;
  delay0=(1000000/freq)/deg - 147;
  Serial.print("delay0 is ");
  Serial.println(delay0);
  for(int i = 0 ; i<deg ;i++){
      zeta[i] = i*360/deg;
      S[i]=sin(zeta[i]*PI/180);
      S_DAC[i]=map(S[i]*1000,-1000,1000,0,4095);
      Serial.print(S[i]) ;
      Serial.print(" ") ;
      Serial.println(S_DAC[i]) ;  
  }
 //6400 
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<deg;i++){
    dac.setVoltage(S_DAC[i],false);
    delayMicroseconds(delay0);
  }
}
