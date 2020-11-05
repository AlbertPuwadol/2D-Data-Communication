/* head for setStation */
/* Reciver */
#include <TEA5767.h>

TEA5767 radio = TEA5767();
float frequency = 87.7;
bool setStation = false;

/* Sender */
#include <Adafruit_MCP4725.h>

#define defaultFreq 1700
#define baudRate 250

const int num_Freq = 4;
const int freq[num_Freq] = {500, 750, 1000, 1250};
int countData[num_Freq];
int delay_Freq[num_Freq];
const int deg = 4;
float S_DAC[deg];
float zeta[deg];
float S[deg];

Adafruit_MCP4725 dac;

/* amplitude diff. for detecting rising or falling signal */
#define baudTime 4800
#define baudRate 250

/* servo */
#include <Servo.h>
Servo myServo;

void setup() {
  /* set serial baudrate */
  Serial.begin(115200);
  dac.begin(0x62);
  Wire.begin();

  /* servo setup */
  myServo.attach(13);
  goStart();
  //Serial.print("Start");

  /* set Frequency */
  //  while (!setStation) {
  //    if (Serial.available() > 0) {
  //      frequency = Serial.parseFloat();
  //    }
  //    if (frequency > 0) {
  //      radio.setFrequency(frequency);
  //      Serial.print("Frequency : ");
  //      Serial.println(frequency);
  //      setStation = true;
  //    }
  //  }
  radio.setFrequency(frequency);


  /* set default value */
  for (int i = 0; i < num_Freq; i++) {
    countData[i] = freq[i] / baudRate;
  }

  /* set delay frequency */
  for (int i = 0; i < num_Freq; i++) {
    delay_Freq[i] = (1000000 / freq[i]) / deg - 147;
  }

  /* set dac */
  for (int i = 0 ; i < deg ; i++) {
    zeta[i] = i * 360 / deg;
    S[i] = sin(zeta[i] * PI / 180);
    S_DAC[i] = map(S[i] * 1000, -1000, 1000, 1800, 3000);
  }
  dac.setVoltage(2400, false); // for don't send

  Serial.println("----------------------- Ready -----------------------");
  Serial.flush();
}

/* --------------------- Variable ----------------------- */
int state = 0;
/* ---------------------- Sender ------------------------ */
uint32_t timeOUT = 0;
uint8_t dataSend = 0;
uint8_t flag = B01000010;

uint8_t castChar[3];

bool ACKsend = false;
/* --------------------- END SEND ----------------------- */

/* ---------------------- Reciver ----------------------- */
#define maxVolt 1024
#define minVolt 400

#define delta 200

bool startCycle = false;
bool startBaud = false;
int prev = 300;
int max = 0;
int count = 0;
int bitCount = 0;
int picCount = 0;
int sendPic = 0;
int tmpData[12];

uint32_t timing = 0;
uint32_t checkTime = 0; // check ACK not lose
uint32_t trashTime = 0; // check for clear noise in tmpData

uint8_t flagData = 0;
uint8_t result = 0;

int header;
uint8_t dataReciver = 0;

bool haveData = false;
bool getAck = 0;

int curSeq_Sen = 0;
int curSeq_Reciv = 0;
uint8_t pos[3];
/* -------------------- END Reciver -------------------- */

/* ----------------------- Servo ----------------------- */
char order;
/* --------------------- END SERVO --------------------- */



void loop() {
  if (state == 0) {
    reciver();
    if (haveData) {
      checkTime = millis();
      Serial.println("have data");
      convertData();
      if (isFlag()) {
        if (errorCheck()) {
          if (checkSeq_Reciv((result >> 5) & 1)) { // Sequency = (result >> 5) & 1)
            Serial.println("Send Ack");
            getBit();
            translateBit();
          }
          sending(2, 5, -1);
          ACKsend = true;
          delay(20);
        }
      }
      haveData = false;
    }
    if (millis() - trashTime > 3000) { // Clear Noise
      //Serial.println("clear");
      clearData();
    }
    if (millis() - checkTime > 6000 && ACKsend == true) {
      state++;
      Serial.print("y");
    }
  }
  else if (state == 1) {
    if (Serial.available() > 0) {
      order = Serial.read();
    }

    if (order == 's') {//start
      //
      if (myServo.read() <= 50) {
        goStart();
        //Serial.print("f");
      }
      else {
        rotateR45();
      }
    }
    else if (order == 'z') {
      state++;
    }

  }
  else if (state == 2) {
    if (picCount < 3) {
      if (Serial.available() > 0) {
        pos[picCount] = Serial.read();
        pos[picCount] = pos[picCount] & 15;
        picCount++;
      }
    }
    else {
      if (sendPic < 3) {
        if (getAck == 0) {
          sending(1, 4, 11 + pos[sendPic]);
          timeOUT = millis();
          getAck = 1;
        }
        else {
          if (millis() - timeOUT > 5000) {
            sending(1, 4, 11 + pos[sendPic]);
            timeOUT = millis();
          }
          else {
            reciver();
            if (haveData) {
              convertData();
              if (isFlag()) {
                if (errorCheck()) {
                  if (checkSeq_Send((result >> 5) & 1)) { // Sequency = (result >> 5) & 1)
                    getBit();
                    translateBit();
                  }
                }
              }
              haveData = false;
            }
            if (millis() - trashTime > 3000) { // Clear Noise
              clearData();
            }
          }
        }
      }
      else {
        state++;
        Serial.print("x");
      }
    }
  }
  else if (state == 3) {
    Serial.println("state3");
  }
}








/* ================================= Sender ================================== */
/* ============= 1_sendData =============== */
void sendData(int data) {
  delay(5);
  int input[4];
  input[0] = (data >> 0) & 3;
  input[1] = (data >> 2) & 3;
  input[2] = (data >> 4) & 3;
  input[3] = (data >> 6) & 3;
  for (int k = 3 ; k >= 0 ; k--) {
    dac.setVoltage(2400, false);
    delay(5);
    for (int cycle = 0; cycle < (freq[input[k]] / baudRate); cycle++) {
      for (int sl = 0; sl < deg; sl++) {
        dac.setVoltage(S_DAC[sl], false);
        delayMicroseconds(delay_Freq[input[k]]);
      }
    }
  }
  dac.setVoltage(2400, false); // for don't send
}
/* =============== END Send data ============ */


/* =============== 2_ setFrame ============== */
int parity = 0;
void setFrame(int format) {
  if (format == 0) {
    dataSend |= B00000000; // set header -> PC1 to PC2
  }
  else if (format == 1) {
    dataSend |= B11000000; // set header -> PC2 to PC1
    parity += 2;
  }
  else if (format == 2) {
    dataSend |= B10000000; // set header -> ack control
    parity += 1;
  }
  else if (format == 3) {
    dataSend |= B10011110; // set header -> ack data
    parity += 5;
  }
  else if (format == 4) {
    if (curSeq_Sen == 1) {
      dataSend |= B00100000;
      parity += 1;
    }
  }
  else if (format == 5) {
    if (curSeq_Reciv == 1) {
      dataSend |= B00100000;
      parity += 1;
    }
  }
  else if (format == 6) {
    dataSend |= B00000010; // set data control -> take 3 picture
    parity += 1;
  }
  else if (format == 7) {
    dataSend |= B00000100; // set data control -> choose L
    parity += 1;
  }
  else if (format == 8) {
    dataSend |= B00001000; // set data control -> choose M
    parity += 1;
  }
  else if (format == 9) {
    dataSend |= B00010000; // set data control -> choose R
    parity += 1;
  }
  else if (format == 10) { // parity check
    Serial.print("parity");
    Serial.println(parity);
    if (parity % 2 != 0) {
      dataSend |= B00000001;
    }

    parity = 0;
  }
  else if (format == 11) { //SEND UPPER
    dataSend |= B00000000;
  }
  else if (format == 12) { //SEND LOWER
    dataSend |= B00000010;
    parity += 1;
  }
  else if (format == 13) { //SEND TOP
    dataSend |= B00000100;
    parity += 1;
  }
  else if (format == 14) { //SEND BOTTOM
    dataSend |= B0000110;
    parity += 2;
  }
  else if (format == 15) { //SEND LEFT
    dataSend |= B0001000;
    parity += 1;
  }
  else if (format == 16) { //SEND RIGHT
    dataSend |= B0001010;
    parity += 2;
  }

}
/* ===================== END Set Frame =================== */


/* ====================== 3_ markData ==================== */
void markData(int header, int Seq, int data) {
  dataSend = 0;
  setFrame(header);
  setFrame(Seq);
  setFrame(data);
  setFrame(10);
}
/* ===================== END mark data =================== */


/* ===================== 4_Send ========================== */
void sending(int head, int seqMode, int dataMode) {
  sendData(flag);
  delay(5);
  markData(head, seqMode, dataMode);
  sendData(dataSend);
  delay(5);
  sendData(flag);
  delay(5);
}
/* ==================== END SEND ======================== */
/* ================================= END SENDER ============================ */












/* =============================== Reciver =================================== */
/* ====================== 1_Reciver ====================== */
void reciver() {
  int tmp = analogRead(A0);
  if (tmp > 300) {
    if (tmp - prev > 200 && startCycle == false) {
      startCycle = true;
      if (startBaud == false) {
        timing = micros();
        startBaud = true;
      }
    }
    if (max < tmp) {
      max = tmp;
    }
    if (max - tmp < 200 && startCycle == true) {
      //Serial.println(max);
      trashTime = millis();
      if (minVolt < max && max < maxVolt) {
        count++;
      }
    }
    startCycle = false;
  }
  if (micros() - timing > baudTime && startBaud == true) {
    if (count > 1) {
      Serial.println(count);
      tmpData[bitCount] = checkData(count);
      bitCount++;
    }

    if (bitCount == 12) {
      haveData = true;
      result = 0;
      bitCount = 0;
    }
    startBaud = false;
    count = 0;
  }
  prev = tmp;
}
/* =========================== END Reciver =============================== */

/* ============ 1.1_Reciver(check cycle) ============ */
int checkData(int count) {
  for (int i = 0; i < num_Freq; i++) {
    if (count == countData[i]) {
      return i;
    }
  }
}
/* ============ END Reciver(check cycle) ============ */


/* ======== 2_Convert Data ======= */
void convertData() {
  for (int i = 4; i < 8; i++) {
    result <<= 2;
    result |= tmpData[i];
  }
  for (int i = 0; i < 4; i++) {
    flagData <<= 2;
    flagData |= tmpData[i];
  }
}

/* ====== END Transform Data ======= */


/* ======== 3_Error detection ========= */
int dataCheck;

bool errorCheck() {
  int sumPrimary = 0;

  for (int i = 0; i < 8; i++) {
    dataCheck = (result >> i) & 1;
    sumPrimary += dataCheck;
  }
  Serial.print("Error checking : ");
  Serial.println(sumPrimary);

  if (sumPrimary % 2 == 0) {
    return true;
  }
  return false;
}
/* ======= END Error detection ======= */


/* ===== 4_Check Frame Sequence ====== */
bool checkSeq_Send(int seq) {
  Serial.print("check Sequency Send : ");
  Serial.println(seq);

  if (seq != curSeq_Sen) {
    curSeq_Sen++;
    curSeq_Sen %= 2;
    return true;
  }
  return false;
}
/* ==== END Check Frame Sequence ==== */
bool checkSeq_Reciv(int seq) {
  Serial.print("check Sequency Recieve : ");
  Serial.println(seq);

  if (seq == curSeq_Reciv) {
    curSeq_Reciv++;
    curSeq_Reciv %= 2;
    return true;
  }
  return false;
}

/* ========= 5_Get Bit ======== */
void getBit() {
  header = (result >> 6) & 3;
  dataReciver = (result >> 1) & 15;
}
/* ======== END Get Bit ======= */


/* ========= 6_Translate Bit ======== */
void translateBit() {
  if (header == 0) {
    if (dataReciver == 1) {
      // control servo 3 rotation
      // Serial.print("y");
    }
    else if (dataReciver == 2) {
      // control servo left
      Serial.println("control servo left");
    }
    else if (dataReciver == 4) {
      // control servo middle
      Serial.println("control servo middle");
    }
    else if (dataReciver == 8) {
      // control servo right
      Serial.println("control servo right");
    }
  }
  else if (header == 2) {
    if (dataReciver == 0) {
      state++;
    }
    else if (dataReciver == 15) {
      sendPic++;
      getAck = 0;
      //      Serial.println("Send ack");
    }
  }
  else if (header == 3) {
    // data from PC2
  }
}
/* ======== END Translate Bit ======= */


///* ============ 7_sendACK= ============ */
//void sendACK() {
//  sendData(flag);
//  delay(5);
//  markACK();
//  sendData(dataSend);
//  delay(5);
//  sendData(flag);
//  delay(5);
//}
///* =========== END sendACK ============ */
//
//
///* ============ 8_markACK ============= */
//void markACK() {
//  setFrame(2);
//  setFrame(4);
//  setFrame(9);
//}
///* =========== END markACK ============ */

/* ========== 11_clearData ============ */
void clearData() {
  result = 0;
  flagData = 0;
  bitCount = 0;
}
/* ============ END ClearData =========== */

/* ================================= END RECIVER ============================ */









/* ==================================== Servo =================================== */
/* ============== 1_Rotate45 ============== */
void rotateR45() {
  int detect = myServo.read();
  for (int i = detect; i > detect - 45; i--) {
    myServo.write(i);
    delay(50);
  }
  Serial.print("c");
}
/* ============= END ROTATE 45 ============ */


/* ============= 2_RotateAll45 ============ */
void rotateAll45(int dest) {
  int detect = myServo.read();
  if (detect <= dest) {
    for (int i = detect; i <= dest; i++) {
      myServo.write(i);
      delay(50);
    }
  }
  else {
    for (int i = detect; i > dest; i--) {
      myServo.write(i);
      delay(50);
    }
  }
  Serial.print("c");
}
/* ============= END RotatrAll45 ========== */


/* =============== 3_GoStart ============== */
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
/* ================ END GoStart ============ */
bool isFlag() {
  if (flagData == flag) {
    return true;
  }
  return false;
}

/* =================================== END SERVO ====================================== */
