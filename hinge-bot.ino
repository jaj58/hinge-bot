//globals
//robot acceleration -> to be worked out...
float robot_acceleration = 0.5; //m/s^2
//direction and speeds
byte left_direction = 13;
byte right_direction = 12;
byte left_speed = 11;
byte right_speed = 3;
//sonar globals
int trigger = 4;
int echo = 2;
void setup() {
  // put your setup code here, to run once:

  //directions
  pinMode(left_direction, OUTPUT);
  pinMode(right_direction, OUTPUT);

  //sonar
  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
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
 digitalWrite(left_direction, HIGH);
  analogWrite(left_speed, 255);
  digitalWrite(right_direction, HIGH);
  analogWrite(right_speed, 255);
  //todo: figure out a way to convert deg to delay time...
  //delay(...); 
}
void robot_left_degrees(float deg){
  digitalWrite(left_direction, LOW);
  analogWrite(left_speed, 255);
  digitalWrite(right_direction, HIGH);
  analogWrite(right_speed, 255);
  //todo: figure out a way to convert deg to delay time...
  //delay(...); 
}
void robot_reverse_distance(float dist){
  digitalWrite(left_direction, HIGH);
  analogWrite(left_speed, 255);
  digitalWrite(right_direction, HIGH);
  analogWrite(right_speed, 255);
  float time_to_delay = sqrt((2*dist)/robot_acceleration);
  delay(time_to_delay);
}

float calc_distance_travelled(float t){
  return (((t*t) * robot_acceleration) / 2); //using s= ut + 1/2 at^2 u=0 so s = ut^2 /2
}
void auto_pilot(){
    static float cur_time_travelled,old_time_travelled = 0; //s=d/t
    start:
    robot_forwards();
    if(sonar_distance() > 50){ //50 milimetres to object in front...
      robot_stop();
      robot_left_degrees(90);
      float left_dist = sonar_distance();
      if(left_dist > 50){
        goto start;
        }else{
          robot_right_degrees(180);
          float right_dist = sonar_distance();
          if(right_dist > 50){
            goto start;
          }
          else{
            /*
            reached a dead end so we need to retrace our steps (started implementing this but still needs work,
            we can takle this by taking the time we took to reach the turn and store each last 2 turns using old and
            current values of time_travelled so we can retrace our steps calculate the distance we travelled using
            this time and then we know the distance where the fork in the maze is, we then need to decide a formula
            where every certain amount of milimetres we do a 180 and check for where the fork is, I haven't figured a 
            effective way of doing that yet or if they will actually make the maze this hard....
            */
          }
      }
    }
    else{
      goto start;
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  auto_pilot();
}


