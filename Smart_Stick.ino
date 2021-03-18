//SDP2021 Team 32

const int pwPin1 = 3;
const int pwPin2 = 5;
int triggerPin1 = 13;
long sensor1, sensor2, distance1, distance2;
void start_sensor(){
  digitalWrite(triggerPin1,HIGH);
  delay(1);
  digitalWrite(triggerPin1,LOW);
}
void read_sensor(){
  sensor2 = pulseIn(pwPin2, HIGH);
  distance1 = sensor2/58;
}
void setup() {
  Serial.begin(9600);
  pinMode(pwPin1, INPUT); 
  pinMode(pwPin2, INPUT);
  pinMode(triggerPin1, OUTPUT);
}
void printall(){         
  if (distance1 >100 ){
    Serial.println("clear: ");
    Serial.print(distance1);
  } 
   else {
    Serial.println("obstacle: ");
    Serial.print(distance1);  
  }
}
void loop () {
  start_sensor();
  read_sensor();
  printall();
  delay(2);  //delay before the void loop starts the section again
  delay(5);

  date();
  delay(500);     // 1/2 second delay

  introduction();
  delay(500);       // 1/2 second delay
  
  leftOfCane();     //There is an obj on the left of the cane
  delay(500);       // 1/2 second delay
  
  rightOfCane();    //There is an obj on the right of the cane
  delay(500);       // 1/2 second delay
  
  distanceOfObj();  //There is an obj ___ ahead -- Currently hardwired
    
  while(1)      // Demonstration complete!
  {
    delay(500);
    digitalWrite(ledPin, HIGH);
    delay(500);              
    digitalWrite(ledPin, LOW);
  }
}


void changeVoice(){
  //Change Voice
  emicSerial.print('N');
  emicSerial.print('2');              // change voice [0,8]
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
}

void changeLanguage(){
  //Change Language
  emicSerial.print('P');
  emicSerial.print('0');              // change language/dialect [0,2]
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
}

void date(){
  // Test 1: Say today's date
  changeVoice();
  changeLanguage();
  emicSerial.print('S');
  emicSerial.print("Hello. This is test number 1. Today is November seven teen twenty twenty.");  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
}

void introduction(){
  // Introduction
  emicSerial.print('S');
  emicSerial.print("This is Linda's MDR Deliverable.");  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
}

void leftOfCane(){
  // Object is on the left
  emicSerial.print('S');
  emicSerial.print("There is an object on the left.");  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
}

void rightOfCane(){
  // Object is on the right
  emicSerial.print('S');
  emicSerial.print("There is an object on the right");  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
  
}

void distanceOfObj(){
  // Object is 2 feet away
  emicSerial.print('S');
  emicSerial.print("There is an object that is 2 feet away");  // Send the desired string to convert to speech
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);         // Turn on LED while Emic is outputting audio
  while (emicSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  digitalWrite(ledPin, LOW);
}
