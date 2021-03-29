//SDP2021 Team 32


////Imports/////
#include <FatReader.h>
#include <SdReader.h>
#include <avr/pgmspace.h>
#include "WaveUtil.h"
#include "WaveHC.h"
#include <Wire.h>
#include "Adafruit_DRV2605.h"


//Sensors
const int pwPin1 = 9;
//const int pwPin2 = 5; //if there is no lat attached to 5
const int pwPin2 = 6;   //If the lat is attached to 5
//int triggerPin1 = 13;
long sensor1, sensor2, distance1, distance2;

//Wave Shield
SdReader card;      // This object holds the information for the card
FatVolume vol;      // This holds the information for the partition on the card
FatReader root;     // This holds the information for the filesystem on the card
FatReader f;        // This holds the information for the file we're play
WaveHC wave;        // This is the only wave (audio) object, since we will only play one at a time
#define DEBOUNCE 5  // button debouncer

//Vibration Motor
Adafruit_DRV2605 drv;

// Global variables
int num=0;
char toPlay[8];     // file to play 00.WAV to 99.WAV
uint8_t effect = 1; // Defaulting vibration motor effect to 1
int audio_time = 0;


////////////Universal SetUp/////////////
void setup() 
{
  Serial.begin(9600);
  sensor_setup();     // (1.)
  waveShield_SetUp(); // (2.)
  vibrationSetup();   // (3.)
}
////////////Universal Loop/////////////
void loop () {
  start_sensor();
  read_sensor();
  waveShield_Loop();
  printall();
}
//////////////////////////////////////////

////////Vibration Motor Setup////////////

void vibrationSetup() {
  Serial.begin(9600);
  Serial.println("DRV test");
  drv.begin();
  
  drv.selectLibrary(1);
  
  // I2C trigger by sending 'go' command 
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG); 
}
////////////////////////////////////

////////////Sensor code/////////////

//-------Sensor set up--------//
void sensor_setup()  //---> (1.)
{
  pinMode(pwPin1, INPUT); 
  pinMode(pwPin2, INPUT);
  //pinMode(triggerPin1, OUTPUT);
}
//----------------------------//
void start_sensor()
{
  //digitalWrite(triggerPin1,HIGH);
  delay(1);
  //digitalWrite(triggerPin1,LOW);
}

void read_sensor()
{
  sensor1 = pulseIn(pwPin1, HIGH);
  distance1 = sensor1/10; //makes the reported range the distance in centimeters
  delay(1); //helped make the range readings more stable
  sensor2 = pulseIn(pwPin2, HIGH);
  distance2 = sensor2/10; 
}

void printall()
{         
  if (distance1<50 && distance2<50){
  
    drv.setWaveform(0, effect); // Plays vibration effect
    drv.setWaveform(1, 0); // Ends effect
    drv.go();
    
    Serial.println("\n Obstacle in Front: ");
    Serial.print(" L = ");
    Serial.print(distance1);
    Serial.print(" R = ");
    Serial.print(distance2);
  } 
  else if (distance1<50 && !distance2<50){
    
    drv.setWaveform(0, effect); // Plays vibration effect
    drv.setWaveform(1, 0); // Ends effect
    drv.go();
    
    Serial.println("\n Obstacle to left: ");
    Serial.print(" L = ");
    Serial.print(distance1);
    Serial.print(" R = ");
    Serial.print(distance2);
  } 
  else if (!distance1<50 && distance2<50){  
    
    drv.setWaveform(0, effect); // Plays vibration effect
    drv.setWaveform(1, 0); // Ends effect
    drv.go();
    
    Serial.println("\n Obstacle to right: ");
    Serial.print(" L = ");
    Serial.print(distance1);
    Serial.print(" R = ");
    Serial.print(distance2);
  }
  else
  {
    Serial.print("\n Clear ");
    Serial.print(" L = ");
    Serial.print(distance1);
    Serial.print(" R = ");
    Serial.print(distance2);
  }
}


////////////////////////////////////////

////////////////////////////////////////
/// --- Wave Shield --- ///

void delay_audio(){
  audio_time = 0;
  while(audio_time < 3000){
    audio_time++;
  }
}

// this handy function will return the number of bytes currently free in RAM, great for debugging!   
int freeRam(void)
{
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 


void sdErrorCheck(void)
{
  if (!card.errorCode()) return;
  putstring("\n\rSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  putstring(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}

void waveShield_SetUp(){ //(2.)
  byte i;
  
  // set up serial port
  Serial.begin(9600);
  putstring_nl("WaveHC with ");

  putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
  Serial.println(freeRam());      // if this is under 150 bytes it may spell trouble!
  
  // Set the output pins for the DAC control. This pins are defined in the library
  pinMode(2, OUTPUT); //LCS
  pinMode(3, OUTPUT); //CLK
  pinMode(4, OUTPUT); //DI
  //pinMode(5, OUTPUT); //LAT
  // initilise file extension part of file to play
  toPlay[2] = '.';
  toPlay[3] = 'W';
  toPlay[4] = 'A';
  toPlay[5] = 'V';
  toPlay[6] = '\0'; // terminator

  
    //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    putstring_nl("Card init. failed!");  // Something went wrong, lets print out why
    sdErrorCheck();
    while(1);                            // then 'halt' - do nothing!
  }
  
  // enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);

// Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                             // we found one, lets bail
  }
  if (part == 5) {                       // if we ended up not finding one  :(
    putstring_nl("No valid FAT partition!");
    sdErrorCheck();      // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }
  
// Try to open the root directory
  if (!root.openRoot(vol)) {
    putstring_nl("Can't open root dir!"); // Something went wrong,
    while(1); 
  }
  
// Try to open the root directory
  if (!root.openRoot(vol)) {
    putstring_nl("Can't open root dir!"); // Something went wrong,
    while(1); 
  }
}

void waveShield_Loop(){
  if(Serial.available() > 0) {
    num = Serial.read();
    num -= 0x30;
    playfile(toPlay);
    delay(1000);
   // wave.stop();    
   }

  if (distance1<50 && distance2<50){      //need to fix the delay / count
    playcomplete( "FRONTC~1.WAV");
  } 
  else if (distance1<50 && !distance2<50){
    playcomplete( "LEFTCANE.WAV");
  } 
  else if (!distance1<50 && distance2<50){  
    playcomplete( "RIGHTC~1.WAV");
  }
}

// Plays a full file from beginning to end with no pause.
void playcomplete(char *name) {
  // call our helper to find and play this name
  playfile(name);
  while (wave.isplaying) {
  // do nothing while its playing
  }
  // now its done playing
}

void playfile(char *name) {
  // see if the wave object is currently doing something
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  // look in the root directory and open the file
  if (!f.open(root, name)) {
    putstring_nl("Couldn't open file "); Serial.print(name); return;
  }
  // OK read the file and turn it into a wave object
  if (!wave.create(f)) {
    putstring_nl("Not a valid WAV"); return;
  }
  
  // ok time to play! start playback
  wave.play();
}

//////////
