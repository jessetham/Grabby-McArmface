#include <Servo.h>

// Torso: 0-180
//        0-210 degrees
//       analog 75-632

// Shoulder: 0-160
//           theta1 vertical = 80
//            analog 68-560
// Elbow: 0-180
//        theta2 straight = 110
//        analog 93-483

// Wrist: 0-180
//        no limits
//        analog 106-456


// Pinch: 0-180
//        75-100-170
//        263-460

Servo servo5;
Servo servo4;
Servo servo3;
Servo servo2;
Servo servo1;

#define servo5pin 9
#define servo5apin 4
#define servo4pin 13
#define servo4apin 0
#define servo3pin 12
#define servo3apin 1
#define servo2pin 11
#define servo2apin 2
#define servo1pin 10
#define servo1apin 3


String incoming_string;
String data;
int servo_angles[5];
int inputlength = 0;
int res;

int s11 = 90;
int s22 = 90;
int s33 = 90;
int s44 = 90;
int s55 = 90;

int s11p;
int s22p;
int s33p;
int s44p;
int s55p;

int s5_amin = 107;
int s5_amax = 481;
int s4_amin = 108;
int s4_amax = 455;
int s3_amin = 93;
int s3_amax = 483;
int s2_amin = 68;
int s2_amax = 560;
int s1_amin = 75;
int s1_amax = 632;

int s5_cur_analog;
int s5_cur_degree;
int s4_cur_analog;
int s4_cur_degree;
int s3_cur_analog;
int s3_cur_degree;
int s2_cur_analog;
int s2_cur_degree;
int s1_cur_analog;
int s1_cur_degree;


float a;
float b;
float groundsmash;

int servoPositions [6];
bool recordflag = 0;

int s1_storage[600];
int s2_storage[600];
int s3_storage[600];
int s4_storage[600];
int s5_storage[600];
int frames = 600;
int frame_delay = 35;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2);
  servo5.attach(servo5pin);
  servo4.attach(servo4pin);
  servo3.attach(servo3pin);
  servo2.attach(servo2pin);
  servo1.attach(servo1pin);

  pinMode(8, OUTPUT);
  pinMode(18, INPUT_PULLUP);
//  attachInterrupt(digitalPinToInterrupt(18), record, FALLING);
  pinMode(44, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(46, OUTPUT);
  pinMode(47, OUTPUT);
  digitalWrite(44, LOW);
  digitalWrite(45, LOW);
  digitalWrite(46, LOW);
  digitalWrite(47, LOW);
  
}

void loop() {
  
  serial_read();
  analog_read();
  servo_write();
  record2();
  if (recordflag == 1){
    playback();
  }

  Serial.print(a);
  Serial.print(' ');
  Serial.print(b);
  Serial.print(' ');
  Serial.print(groundsmash);
  Serial.print(' ');
  Serial.print(s1_cur_degree);
  Serial.print(' ');
  Serial.print(s2_cur_degree);
  Serial.print(' ');
  Serial.print(s3_cur_degree);
  Serial.print(' ');
  Serial.print(s4_cur_degree);
  Serial.print(' ');
  Serial.print(s5_cur_degree);
  Serial.print('\n');
    
  delay(10);
}

void playback(){
  delay(1000);
  for (int x = 0; x<frames;x++){
     servo1.write(s1_storage[x]);
     servo2.write(s2_storage[x]);
     servo3.write(s3_storage[x]);
     servo4.write(s4_storage[x]);
     servo5.write(s5_storage[x]);
     Serial.println("playback");
     delay(frame_delay);
    }
    recordflag = 0;
}

void record2(){
  
  if(digitalRead(18) == 0){
  servo1.detach();
  servo2.detach();
  servo3.detach();
  servo4.detach();
  servo5.detach();
  
   digitalWrite(8, HIGH);
   delay(100);
   digitalWrite(8, LOW);
   delay(100);
   
   for (int x = 0; x<frames;x++){
     s1_storage[x] = 90;
     s2_storage[x] = 90;
     s3_storage[x] = 90;
     s4_storage[x] = 90;
     s5_storage[x] = 90;
    }
    
   int count = 0;
   while(digitalRead(18) == 0)
   {
      s5_cur_analog = analogRead(servo5apin);
      s5_cur_degree = map(s5_cur_analog, s5_amin, s5_amax, 0, 180);
      s4_cur_analog = analogRead(servo4apin);
      s4_cur_degree = map(s4_cur_analog, s4_amin, s4_amax, 0, 180);
      s3_cur_analog = analogRead(servo3apin);
      s3_cur_degree = map(s3_cur_analog, s3_amin, s3_amax, 0, 180);
      s2_cur_analog = analogRead(servo2apin);
      s2_cur_degree = map(s2_cur_analog, s2_amin, s2_amax, 0, 180);
      s1_cur_analog = analogRead(servo1apin);
      s1_cur_degree = map(s1_cur_analog, s1_amin, s1_amax, 0, 180);
//      Serial.print("s1");
//      Serial.print(s1_cur_degree;
//      Serial.print("s2");
//      Serial.print(s2_cur_degree);
//      Serial.print("s3");
//      Serial.print(s3_cur_degree);
//      Serial.print("s4");
//      Serial.print(s4_cur_degree);
//      Serial.print("s5");
//      Serial.print(s5_cur_degree);
//      Serial.print('\n');
      if (count < frames){
      s1_storage[count] = s1_cur_degree;
      s2_storage[count] = s2_cur_degree;
      s3_storage[count] = s3_cur_degree;
      s4_storage[count] = s4_cur_degree;
      s5_storage[count] = s5_cur_degree;
      }
      Serial.println("recording");
      count++;
      delay(frame_delay);
   }
  servo5.attach(servo5pin);
  servo4.attach(servo4pin);
  servo3.attach(servo3pin);
  servo2.attach(servo2pin);
  servo1.attach(servo1pin);
  recordflag = 1;
  }
  
  
}

void record()
{
  servo1.detach();
  servo2.detach();
  servo3.detach();
  servo4.detach();
  servo5.detach();
  
   digitalWrite(13, HIGH);
   delay(100);
   digitalWrite(13, LOW);
   delay(100);

   int count = 0;
   while(digitalRead(18) == 0)
   {

      s5_cur_analog = analogRead(servo5apin);
      s5_cur_degree = map(s5_cur_analog, s5_amin, s5_amax, 0, 180);
      s4_cur_analog = analogRead(servo4apin);
      s4_cur_degree = map(s4_cur_analog, s4_amin, s4_amax, 0, 180);
      s3_cur_analog = analogRead(servo3apin);
      s3_cur_degree = map(s3_cur_analog, s3_amin, s3_amax, 0, 180);
      s2_cur_analog = analogRead(servo2apin);
      s2_cur_degree = map(s2_cur_analog, s2_amin, s2_amax, 0, 180);
      s1_cur_analog = analogRead(servo1apin);
      s1_cur_degree = map(s1_cur_analog, s1_amin, s1_amax, 0, 180);
//      Serial.print("s1");
//      Serial.print(s1_cur_degree;
//      Serial.print("s2");
//      Serial.print(s2_cur_degree);
//      Serial.print("s3");
//      Serial.print(s3_cur_degree);
//      Serial.print("s4");
//      Serial.print(s4_cur_degree);
//      Serial.print("s5");
//      Serial.print(s5_cur_degree);
//      Serial.print('\n');
      s1_storage[count] = s1_cur_degree;

      Serial.println(s1_storage[count]);
      count++;
      delayMicroseconds(1000000);
   }
  servo5.attach(servo5pin);
  servo4.attach(servo4pin);
  servo3.attach(servo3pin);
  servo2.attach(servo2pin);
  servo1.attach(servo1pin);
   
}

void servo_write(){

  s55 = constrain(s55, 75, 170);
  s44 = constrain(s44, 0, 180);
  s33 = constrain(s33, 0, 180);
  s22 = constrain(s22, 0, 160);
  s11 = constrain(s11, 0, 180);

  a = s22 + 10;
  b = s33 - 110; 
  groundsmash = 6.00 + 16.00*sin(a*PI/180.00) + 21.00*cos(b*PI/180.00)*sin(a*PI/180.00) + 21.00*sin(b*PI/180.00)*cos(a*PI/180.00);

  
  if (groundsmash > 5){
    servo3.write(s33);
    servo2.write(s22);
    servo5.write(s55);
    servo4.write(s44);
    servo1.write(s11);
    
  }
  else {
    servo5.write(s55);
    servo4.write(s44);
    servo1.write(s11);
    Serial.println("naaaaa");
  }  
}



void analog_read(){
  
  s5_cur_analog = analogRead(servo5apin);
  s5_cur_degree = map(s5_cur_analog, s5_amin, s5_amax, 0, 180);
  s4_cur_analog = analogRead(servo4apin);
  s4_cur_degree = map(s4_cur_analog, s4_amin, s4_amax, 0, 180);
  s3_cur_analog = analogRead(servo3apin);
  s3_cur_degree = map(s3_cur_analog, s3_amin, s3_amax, 0, 180);
  s2_cur_analog = analogRead(servo2apin);
  s2_cur_degree = map(s2_cur_analog, s2_amin, s2_amax, 0, 180);
  s1_cur_analog = analogRead(servo1apin);
  s1_cur_degree = map(s1_cur_analog, s1_amin, s1_amax, 0, 180);
  
}

void serial_read() {
char inputbuffer[20];
  while (Serial.available()){
    char inchar = Serial.read();

    if (inchar == '\n'){
      inputbuffer[inputlength] = '\0';
      int s1;
      int s2;
      int s3;
      int s4;
      int s5;
      res = sscanf(inputbuffer, "s1%ds2%ds3%ds4%ds5%d", &s1, &s2, &s3, &s4, &s5);
      inputlength = 0;
      s11 = s1;
      s22 = s2;
      s33 = s3;
      s44 = s4;
      s55 = s5;
//      Serial.print(',');
//      Serial.print(s1);
//      Serial.print(' ');
//      Serial.print(s2);
//      Serial.print(' ');
//      Serial.print(s4);
//      Serial.print(' ');
//      Serial.print(s4);
//      Serial.print(' ');
//      Serial.print(s5);
//      Serial.print('\n');
      
    }
    else{
      inputbuffer[inputlength] = inchar;
      inputlength++;
    }
}


  
}

