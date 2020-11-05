/* cbi this for increase analogRead Speed */
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
/*amplitude diff. for detecting rising or falling signal*/
#define r_slope 100


#define baudTime 9900
#define baudRate 100
const int num_Freq = 4;
const int freq[num_Freq] = {200, 300, 400, 500};
int countData[num_Freq];


void setup() {
  // put your setup code here, to run once:
  sbi(ADCSRA, ADPS2) ; // this for increase analogRead Speed
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;
  /* set serial baudrate the same as in TX */
  Serial.begin(115200);
  for (int i = 0; i < num_Freq; i++) {
    countData[i] = freq[i] / baudRate;
    Serial.print("Freq : ");
    Serial.print(freq[i]);
    Serial.print(" Cycle : ");
    Serial.println(freq[i] / baudRate);
  }
  Serial.println("Ready");
  Serial.flush();
}

bool check = false;
bool start = false;
int prev = 0;
int count = 0;
int bitCount = 0;
int max = 0;

uint32_t startFreq = 0;

uint8_t result = 0;
uint8_t data = 0;

void loop() {
  int tmp = analogRead(A0);

  if (tmp > r_slope && prev < r_slope && check == false) {
    check = true; // change check status is true
    max = 0;
    if (start == false) {
      startFreq = micros();
      start = true;
    }
  }
  if (max < tmp) {
    max = tmp;
    //Serial.println(max);
  }
  if (max - tmp > r_slope) {
    if (check == true) {
      if (240 < max && max <= 600) {
        //Serial.println(max);
        count++;
      }
    }
    if (micros() - startFreq > baudTime && start == true) {
      //Serial.println(count);
      data = checkData(count);
      result <<= 2;
      result |= data;
      bitCount++;
      if (bitCount == 4) {
        Serial.println(char(result));
        result = 0;
        bitCount = 0;
      }
      start = false;
      count = 0;
    }
    check = false;
  }
  prev = tmp;
}

int checkData(int count) {
  for (int i = 0; i < num_Freq; i++) {
    if (count == countData[i]) {
      return i;
    }
  }
}
