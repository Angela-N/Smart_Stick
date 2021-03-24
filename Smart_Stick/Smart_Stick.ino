//SDP2021 Team 32
const int pwPin1 = 3;
const int pwPin2 = 5;
int triggerPin1 = 13;
long sensor1, sensor2, distance1, distance2;

////////////Universal SetUp/////////////
void setup() 
{
  Serial.begin(9600);
  sensor_setup();   // (1.)
}
////////////Universal Loop/////////////
void loop () {
  start_sensor();
  read_sensor();
}
//////////////////////////////////////////


////////////  (1.) Sensor code/////////////

//-------Sensor set up--------//
void sensor_setup()  //---> (1.)
{
  pinMode(pwPin1, INPUT); 
  pinMode(pwPin2, INPUT);
  pinMode(triggerPin1, OUTPUT);
}
//----------------------------//
void start_sensor()
{
  digitalWrite(triggerPin1,HIGH);
  delay(1);
  digitalWrite(triggerPin1,LOW);
}

void read_sensor()
{
  sensor1 = pulseIn(pwPin1, HIGH);
  distance1 = sensor1/58; //makes the reported range the distance in centimeters
  delay(1); //helped make the range readings more stable
  sensor2 = pulseIn(pwPin2, HIGH);
  distance2 = sensor2/58;
}

void printall()
{         
  if (distance1<50 && distance2<50){
    Serial.println("Obstacle Ahead /left and right: ");
    Serial.print(" L = ");
    Serial.print(distance1);
    Serial.print(" R = ");
    Serial.print(distance2);
  } 
  else if (distance1<50 && !distance2<50){
    Serial.println("Obstacle to left: ");
    Serial.print(" L = ");
    Serial.print(distance1);
    Serial.print(" R = ");
    Serial.print(distance2);
  } 
  else if (!distance1<50 && distance2<50){
    Serial.println("Obstacle to right: ");
    Serial.print(" L = ");
    Serial.print(distance1);
    Serial.print(" R = ");
    Serial.print(distance2);
  }
  else
  {
    Serial.print("Clear ");
    Serial.print(" L = ");
    Serial.print(distance1);
    Serial.print(" R = ");
    Serial.print(distance2);
  }
}
////////////////////////////////////////
