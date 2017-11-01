//globals
//robot acceleration -> to be worked out (p.s 0.5 isn't probably anywhere near the actual robot acceleration just a value till we can work it out...
float robot_acceleration = 0.5; //m/s^2
float robot_width = 100; //milimetres.. to be modified to a more accurate value next session same as acceleration
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
delay(3000);
  //directions
  pinMode(left_direction, OUTPUT);
  pinMode(right_direction, OUTPUT);

  //sonar
  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
}

void robot_reverse() {
  digitalWrite(left_direction, HIGH);
  analogWrite(left_speed, 255);
  digitalWrite(right_direction, HIGH);
  analogWrite(right_speed, 255);
}

void robot_forwards() {
  digitalWrite(left_direction, LOW);
  analogWrite(left_speed, 255);
  digitalWrite(right_direction, LOW);
  analogWrite(right_speed, 255);
}

void robot_left() {
  digitalWrite(left_direction, LOW);
  analogWrite(left_speed, 255);
  digitalWrite(right_direction, HIGH);
  analogWrite(right_speed, 255);
}

void robot_right() {
  digitalWrite(left_direction, HIGH);
  analogWrite(left_speed, 255);
  digitalWrite(right_direction, LOW);
  analogWrite(right_speed, 255);
}

void robot_stop() {
  digitalWrite(left_direction, LOW);
  analogWrite(left_speed, 0);
  digitalWrite(right_direction, LOW);
  analogWrite(right_speed, 0);
}
void print_sonar_debug(float duration) {
  Serial.print("duration : ");
  Serial.println(duration);
  Serial.print("distance : ");
  Serial.println(sonar_to_distance(duration));
}

float sonar_duration() {
  long duration;
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  //if debugging uncomment
  //print_sonar_debug(duration);
  return duration;
}
float sonar_to_distance(float t) {
  float speed_of_sound = 343;
  return (((t / 2) * speed_of_sound) / 1000);
}
float sonar_distance() {
  return sonar_to_distance(sonar_duration());
}

void robot_right_degrees(float deg) {
  digitalWrite(left_direction, HIGH);
  analogWrite(left_speed, 255);
  digitalWrite(right_direction, HIGH);
  analogWrite(right_speed, 255);
  //todo: figure out a way to convert deg to delay time...
  //delay(...);
}
void robot_left_degrees(float deg) {
  digitalWrite(left_direction, LOW);
  analogWrite(left_speed, 255);
  digitalWrite(right_direction, HIGH);
  analogWrite(right_speed, 255);
  //todo: figure out a way to convert deg to delay time...
  //delay(...);
}
void robot_forwards_distance(float dist) {
  digitalWrite(left_direction, LOW);
  analogWrite(left_speed, 255);
  digitalWrite(right_direction, LOW);
  analogWrite(right_speed, 255);
  float time_to_delay = sqrt((2 * dist) / robot_acceleration); //rearrange s=ut + 1/2 at^2 so we get t = sqrt(2dist/acceleration)
  delay(time_to_delay);
}

float calc_distance_travelled(float t) {
  return (((t * t) * robot_acceleration) / 2); //using s= ut + 1/2 at^2 u=0 so s = ut^2 /2
}
void auto_pilot() {
  float cur_time_travelled, old_time_travelled = 0; //s=d/t
  float start_time = micros();
start: //disgusting goto but it should in theory work and is quite simple anyway also this is over commented but makes sure everyone get's the idea behind it.
  robot_forwards();
  if (sonar_distance() > 50) { //if there's a object less than 50 milimetres in front of us...
    robot_stop();
    robot_left_degrees(90);
    float left_dist = sonar_distance();
    if (left_dist > 50) { //check if the left of where we stopped is clear for more than 50 milimetres
      old_time_travelled = cur_time_travelled;
      cur_time_travelled = 0;
      start_time = micros();
      goto start; //repeat process
    } else { //technically right should be best option now since left and original forward aren't
      robot_right_degrees(180);
      float right_dist = sonar_distance();
      if (right_dist > 50) { //check if right of where we stopped is clear for more than 50 milimetres
        old_time_travelled = cur_time_travelled;
        cur_time_travelled = 0;
        start_time = micros();
        goto start;//repeat process
      }
      else { //even right isn't clear so we are at a dead end
        /*
          reached a dead end so we need to retrace our steps (started implementing this but still needs work,
          we can takle this by taking the time we took to reach the turn and store each last 2 turns using old and
          current values of time_travelled so we can retrace our steps calculate the distance we travelled using
          this time and then we know the distance where the fork in the maze is, we then need to decide a formula
          where every certain amount of milimetres we do a 180 and check for where the fork is, I haven't figured a
          effective way of doing that yet or if they will actually make the maze this hard....
        */
        //make the robot face backwards from where came from
        robot_right_degrees(90);
        //micro therefore 1x10^-6
        float new_time_travelled = cur_time_travelled / 1000000; //convert micro seconds to seconds
        float dist_travld_since_turn = calc_distance_travelled(new_time_travelled); //how far we travelled since our last turn in metres
        dist_travld_since_turn *= 1000; //convert to milimetres
        float should_travel_dist = dist_travld_since_turn / robot_width;
        /*Now check both sides periodically using our robot width
          so we most likely can't miss any forks which we are now looking
          for since we missed it earlier on. If we find a fork we reset to the start
          and carry on down there. This should then mean we will never get stuck/lost in dead
          ends at least for long periods of time in theory unless some calculations are wrong
          or we use wrong values for things like robot width, it's acceleration and so on
        */
travel_forward:
        robot_forwards_distance(should_travel_dist); //might want to do some error handling with the travel_dist by taking it away each time etc...
        robot_right_degrees(90);
        if (sonar_distance() > 150) {
          old_time_travelled = cur_time_travelled;
          cur_time_travelled = 0;
          start_time = micros();
          goto start;//repeat process
        }
        else {
          robot_left_degrees(180);
          if (sonar_distance() > 150) {
            old_time_travelled = cur_time_travelled;
            cur_time_travelled = 0;
            start_time = micros();
            goto start;//repeat process
          }
          else {
            //no fork at current point so repeat...
            robot_right_degrees(90); //now facing back straight again
            goto travel_forward;
          }
        }
      }
    }
  }
  else {
    cur_time_travelled = micros() - start_time;
    goto start;//repeat process
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  auto_pilot();
}


