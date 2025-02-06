/*
* Name: color_detection_program
* Author: Elias Johnsson
* Date: 2025-02-06
* Description: The program uses a Color detection sensor to sort objects based on their color. 
* After detecting the color the program will direct the cube away and sort it, by using a system of servos.
*/

#include <Servo.h>

Servo servo_door; // Servo object 1
Servo direction_servo; // Servo object 2
Servo release_servo; // Servo object 3

const int servo_door_pin = 10;
const int direction_servo_pin = 9;
const int release_servo_pin = 11;
const int button_pin = 12;
const int S0 = 4;
const int S1 = 5;
const int S2 = 6;
const int S3 = 7;
const int sensorOut = 8;
int color_val_array[3];
int frequency = 0;
int red_index = 0;
int green_index = 1;
int blue_index = 2;
int color_result;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(button_pin,INPUT);
  servo_door.attach(servo_door_pin);
  direction_servo.attach(direction_servo_pin);
  release_servo.attach(release_servo_pin);
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(button_pin)){
    detect_color();
    servo_sort(); 
  }
}
/*
* Description: Collects and maps color value from the color sensor to a RGB-scale. 
* Parameters:
* - Index: the index of color values, 1 = red, 2 = green, 3 = blue
* Returns:¨
* - Returns one of three collected color values for a specific color. 
*/
int get_color_val(int index){
  // Red frequency
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  frequency = pulseIn(sensorOut, LOW);
  color_val_array[0] = map(frequency, 745,900,255,0);
  // Green frequency
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  frequency = pulseIn(sensorOut, LOW);
  color_val_array[1] = map(frequency,458,550,255,0);
  // Blue frequency
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  frequency = pulseIn(sensorOut, LOW);
  color_val_array[2] = map(frequency, 494,603,255,0);
  return color_val_array[index];
  
}
/*
* Description: Opens servo door to let an object through
* Parameters: Void
* Returns: Void
*/
void servo_open(){
  servo_door.write(90);
  delay(500);
  servo_door.write(180);
}
/*
* Description: Resets servo positions to original
* Parameters: Void
* Returns: Void
*/
void servo_reset(){
  release_servo.write(0);
  delay(2000);
  direction_servo.write(130);
  delay(1000);
  servo_opne();
}
/*
* Description: directs two servo motors to move and sort objects depending on the detected color
* Parameters: Void
* Returns: Void 
*/
void servo_sort(){
  servo_reset();
  if (color_result == 0) {
    delay(500);
    direction_servo.write(270);
    delay(1000);
    Serial.println(" - RED detected!");
    release_servo.write(180);
  }
  if (color_result == 1){
    delay(500);
    direction_servo.write(90);
    delay(1000);
    Serial.println(" - GREEN detected!");
    release_servo.write(180);
  }
  if (color_result == 2){
    direction_servo.write(180);
    delay(1000);
    Serial.println(" - BLUE detected!");
    release_servo.write(180);
  }
    delay(800);

}

/*
* Description: Uses color values from get_color_val to deterime the color that the sensor has sensed.
* Parameters: Void
* Returns: Void 
*/
void detect_color(){
  int r_frequency = get_color_val(0);
  int g_frequency = get_color_val(1);
  int b_frequency = get_color_val(2);
  if(r_frequency > g_frequency && r_frequency > b_frequency){
    color_result = red_index;
  }
  if(g_frequency > r_frequency && g_frequency > b_frequency){
    color_result = green_index;
  }
  if(b_frequency > r_frequency && b_frequency > g_frequency){
    color_result = blue_index;
  }
}