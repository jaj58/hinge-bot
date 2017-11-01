//globals
//direction and speeds
byte left_direction = 13;
byte right_direction = 12;
byte left_speed = 11;
byte right_speed = 3;
//sonar globals
int trigger = 4;
int echo = 2;
//distance state (used for wall checks)
int orig_state = 0;
void setup() {
  // put your setup code here, to run once:

  //directions
  pinMode(left_direction, OUTPUT);
  pinMode(right_direction, OUTPUT);

  //sonar
  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

   Serial.print("finished setup");
   robot_right_degrees(90);
}

void robot_reverse(){
  digitalWrite(left_direction, HIGH);
  analogWrite(left_speed, 255);
  digitalWrite(right_direction, HIGH);
  analogWrite(right_speed, 255);
}

void robot_forwards(){
  digitalWrite(left_direction, LOW);
  analogWrite(left_speed, 255);
  digitalWrite(right_direction, LOW);
  analogWrite(right_speed, 255);
}

void robot_left(){
  digitalWrite(left_direction, LOW);
  analogWrite(left_speed, 255);
  digitalWrite(right_direction, HIGH);
  analogWrite(right_speed, 255);
}

void robot_right(){
  digitalWrite(left_direction, HIGH);
  analogWrite(left_speed, 255);
  digitalWrite(right_direction, LOW);
  analogWrite(right_speed, 255);
}

void robot_stop(){
  digitalWrite(left_direction, LOW);
  analogWrite(left_speed, 0);
  digitalWrite(right_direction, LOW);
  analogWrite(right_speed, 0);
}
void print_sonar_debug(float duration){
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
  //print_sonar_debug(duration);
  return duration;
}
float sonar_to_distance(float t){
 float speed_of_sound = 343;
 return (((t/2) * speed_of_sound) / 1000);
}
float sonar_distance(){
  return sonar_to_distance(sonar_duration());
}

void robot_right_degrees(float deg){
  robot_right();
 //21seconds for 5 rotations 360 = 21/5 
 //4.2 for 360
  float time_to_delay = deg * (4100 /360);
  delay(time_to_delay);
  robot_stop();
}
void robot_left_degrees(float deg){
  robot_left();
  //21seconds for 5 rotations 360 = 21/5 
   float time_to_delay = deg * (4100 /360);
  delay(time_to_delay);
  robot_stop();
}

void auto_pilot(){
  if(sonar_distance() > 150){
  robot_forwards();
  else{
    robot_left_degrees(60);
    float left_dist = sonar_distance();
    robot_right_degrees(120);
    float right_dist = sonar_distance();
    if(left_dist > right_dist){
      robot_left_degrees(120);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  auto_pilot();
}


