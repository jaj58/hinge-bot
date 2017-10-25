
//globals
//direction and speeds
byte leftDirection = 13;
byte rightDirection = 12;
byte leftSpeed = 11;
byte rightSpeed = 3;
//sonar globals
int trigger = 4;
int echo = 2;
//states
int orig_state = 0;

void setup() {
  // put your setup code here, to run once:

  //direction and speeds
  pinMode(leftDirection, OUTPUT);
  pinMode(rightDirection, OUTPUT);

  //sonar
  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
}

void robotReverse(){
  digitalWrite(leftDirection, HIGH);
  analogWrite(leftSpeed, 255);
  digitalWrite(rightDirection, HIGH);
  analogWrite(rightSpeed, 255);
}

void robotForwards(){
  digitalWrite(leftDirection, LOW);
  analogWrite(leftSpeed, 255);
  digitalWrite(rightDirection, LOW);
  analogWrite(rightSpeed, 255);
}

void robotLeft(){
   digitalWrite(leftDirection, LOW);
  analogWrite(leftSpeed, 255);
  digitalWrite(rightDirection, HIGH);
  analogWrite(rightSpeed, 255);
}

void robotRight(){
  digitalWrite(leftDirection, HIGH);
  analogWrite(leftSpeed, 255);
  digitalWrite(rightDirection, LOW);
  analogWrite(rightSpeed, 255);
}

void robotStop(){
  digitalWrite(leftDirection, LOW);
  analogWrite(leftSpeed, 0);
  digitalWrite(rightDirection, LOW);
  analogWrite(rightSpeed, 0);
}
void print_sonar_debug(){
  Serial.print("duration : ");
  Serial.println(duration);
  Serial.print("distance : ");
  Serial.println(sonar_to_distance(duration));
}

float sonar_duration(){
  long duration;
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  //if debugging uncomment 
  //print_sonar_debug();
  return duration;
}
float sonar_to_distance(float t){
 float speed_of_sound = 343;
 return (((t/2) * speed_of_sound) / 1000);
}
float sonar_distance(){
  return (sonar_to_distance(sonar_duration());
}

enum distance_states{
  FORWARD_CLEAR,
  FORWARD_BLOCKED,
  RIGHT_BLOCKED,
  LEFT_BLOCKED
}
void check_for_wall(){
  switch(orig_state){
    case 0:
    robotForwards();
    if(sonar_distance() > 100){
      robotStop();
      robotLeft();
      
    }
  }
}

void robot_right_degrees(float deg){
  
}
void robot_left_degrees(float deg){
  
}

void loop() {
  // put your main code here, to run repeatedly:
  robotRight();
}


