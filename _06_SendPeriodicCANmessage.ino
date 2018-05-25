#include <FlexCAN.h>

//Modes for each flag
#define MODE1 = 1;
#define MODE2 = 2;
#define MODE3 = 3;
#define MODE4 = 4;
#define MODE5 = 5;
#define MODE6 = 6;
#define MODE7 = 7;
#define MODE8 = 8;

int MODE = 1;

//Set up timing variables
#define TXPeriod100 100 //this sets your tx timer
elapsedMillis TXTimer100;

//
#define DOSPeriod 30000
elapsedMillis DOSTimer;

//Declare the CAN message variable that includes
//ID, Data Length Code, and Data Buffer
static CAN_message_t txmsg,rxmsg;

//messages

flag_1 = [0x66, 0x6c, 0x61, 0x67, 0x20, 0x31, 0x3a, 0x00];
hash1_1 = [0x7f, 0x36, 0x7f, 0x92, 0xf8, 0x27, 0x9d, 0xb1];
hash1_2 = [0x82, 0x6a, 0x6e, 0x5d, 0x53, 0xf4, 0x30, 0xa4];
flag_2 = [0x32, 0x73, 0x6c, 0x6f, 0x77, 0x64, 0x77, 0x6e];

void setup() {
  // put your setup code here, to run once:
  
  //Start the CAN message on the first CAN controller (works on both the Teensy 3.2 and Teensy 3.6)
  Can0.begin(250000);
  Can1.begin(500000);

  //The default filters exclude the extended IDs, so we have to set up CAN filters to allow those to pass.
  CAN_filter_t allPassFilter;
  allPassFilter.ext=1;
  for (uint8_t filterNum = 8; filterNum < 16;filterNum++){ //only use half the available filters for the extended IDs
    Can0.setFilter(allPassFilter,filterNum); 
  }

  //define the non changing parts of the CAN message to be transmitted.
  txmsg.ext=1;
  txmsg.id=0x000000F9;
  txmsg.len=8;
}

void loop() {
  //different CTF Modes
  if(MODE == 1){
    //what to do during flag 1 objective
    //this is waiting for the user to have found flag 1 and decoded it to allow the user to advance to the second flag. 
    if (Can0.available()) {
    Can0.read(rxmsg);
    if(rxmsg.id == 0x18FEF100){
      if(rxmsg.buf[2] == __ && rxmsg.buf[3] == __) { //need to find the values associated with 80mph from BEN
    MODE = MODE2;
      }
      
    //so this is the first MD5 HASHed flag. The message is "flag 1:null|7f367f92f8279db1826a6e5d53f430a4" which means "flag 1:gofaster"
    for(int i = 0; int<= 8; i++){
      txmsg.buf[i] = flag_1[i];
      Can0.write(txmsg);
      Can1.write(txmsg);
      delay(100);
    for(int i = 0; int<= 8; i++){
      txmsg.buf[i] = hash1_1[i];
      Can0.write(txmsg);
      Can1.write(txmsg);
      delay(100);
    for(int i = 0; int<= 8; i++){
      txmsg.buf[i] = hash1_2[i];
      Can0.write(txmsg);
      Can1.write(txmsg);
    }
  }
  if(MODE == 2){
    //what to do during flag 2 objective
    MODE = MODE3;
  }
  
  if(MODE == 3){
    //what to do during flag 3 objective
    MODE = MODE4;
  }
  
  if(MODE == 4){
    //what to do during flag 4 objective
    MODE = MODE5;
  }
  if(MODE == 5){
    //what to do during flag 5 objective
    MODE = MODE6;
  }
  if(MODE == 6){
    //what to do during flag 6 objective
    MODE = MODE7;
  }
  if(MODE == 7){
    //what to do during flag 7 objective
    MODE = MODE8;
  }
  if(MODE == 8){
    //what to do during flag 8 objective
    MODE = MODE9;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  //Poll for new messages
  if (Can0.available()) {
    Can0.read(rxmsg);
    char CANdataDisplay[50];
    sprintf(CANdataDisplay, "%12lu %12lu %08X %d %d", RXCount++, micros(), rxmsg.id, rxmsg.ext, rxmsg.len);
    Serial.print(CANdataDisplay);
    for (uint8_t i = 0; i < rxmsg.len; i++) {
      char CANBytes[4];
      sprintf(CANBytes, " %02X", rxmsg.buf[i]);
      Serial.print(CANBytes);
    }
    Serial.println();
  }

  if (TXTimer100 >= TXPeriod100){
    TXTimer100 = 0; //Reset the timer

    //Convert the 32-bit timestamp into 4 bytes with the most significant byte (MSB) first (Big endian).
    uint32_t sysMicros = micros();
    txmsg.buf[0] = (sysMicros & 0xFF000000) >> 24;
    txmsg.buf[1] = (sysMicros & 0x00FF0000) >> 16;
    txmsg.buf[2] = (sysMicros & 0x0000FF00) >>  8;
    txmsg.buf[3] = (sysMicros & 0x000000FF);

    //Convert the 32-bit transmit counter into 4 bytes with the most significant byte (MSB) first (Big endian). 
    TXCount++;
    txmsg.buf[4] = (TXCount & 0xFF000000) >> 24;
    txmsg.buf[5] = (TXCount & 0x00FF0000) >> 16;
    txmsg.buf[6] = (TXCount & 0x0000FF00) >>  8;
    txmsg.buf[7] = (TXCount & 0x000000FF);
    Can0.write(txmsg);
    Can1.write(txmsg);
    
    LEDstate = !LEDstate;
    digitalWrite(LED_BUILTIN, LEDstate);
  }

  /*
   * CHALLENGE Problems:
   * 
   * 1. Request component information from an ECU.  You may need to look for a response.
   * 2. Develop a bus flooding device
   * 3. 
   */


  
}
