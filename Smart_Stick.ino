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
}
