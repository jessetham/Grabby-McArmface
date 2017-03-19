#include <Servo.h>
#include <SPI.h>
#include <SD.h>

File myFile;

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
int s2_amax = 622;
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

int servoStorage [5][600];
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
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);
  
  pinMode(44, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(46, OUTPUT);

  pinMode(40, INPUT_PULLUP);
  pinMode(41, INPUT_PULLUP);

  //50 (MISO), 51 (MOSI), 52 (SCK), 53 (SS).
  SD.begin(53);
  
}

void loop() {
  serial_read();
  analog_read();
  servo_write();
  
  int lengthOfRecording;
  if(digitalRead(18) == 0){
    lengthOfRecording = record();
    while(digitalRead(18) == 0) {}
  }
  if (recordflag == 1){
    playback(lengthOfRecording);
  }

  if(digitalRead(40) == 0){
    String voice_command2 = get_voice();
    lengthOfRecording = voice_record();
    while(digitalRead(40) == 0) {}
    save_recording(lengthOfRecording, voice_command2);
  }

  if(digitalRead(41) == 0){
    Serial.print(",record");
    Serial.print('\n');
//    myFile = SD.open("asdf.txt");
//    while (myFile.available()) {
//      Serial.write(myFile.read());
//    }
//    myFile.close();
    
    String voice_command2 = get_voice();
    voice_play(voice_command2);
  }
    
  

//  Serial.print(a);
//  Serial.print(' ');
//  Serial.print(b);
//  Serial.print(' ');
//  Serial.print(groundsmash);
//  Serial.print(' ');
//  Serial.print(s1_cur_degree);
//  Serial.print(' ');
//  Serial.print(s2_cur_degree);
//  Serial.print(' ');
//  Serial.print(s3_cur_degree);
//  Serial.print(' ');
//  Serial.print(s4_cur_degree);
//  Serial.print(' ');
//  Serial.print(s5_cur_degree);
//  Serial.print('\n');
    
  delay(10);
}

void voice_play_parse(const char* c){

//      unsigned int l = line.length();
//      char c[l];
//      line.toCharArray(c, l);
  
      int s1;
      int s2;
      int s3;
      int s4;
      int s5;
      res = sscanf(c, "s1%ds2%ds3%ds4%ds5%d", &s1, &s2, &s3, &s4, &s5);
      s11 = s1;
      s22 = s2;
      s33 = s3;
      s44 = s4;
      s55 = s5;
      Serial.println(s11);
}

void voice_play(String filename){
  unsigned int l = filename.length();
  char c[l];
  filename.toCharArray(c, l);
  
  myFile = SD.open(c);
  if (myFile){
    
  while(myFile.available()){
char inputbuffer[20];
  while (myFile.available()){
    char inchar = myFile.read();

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
          servo3.write(s33);
    servo2.write(s22);
    servo5.write(s55);
    servo4.write(s44);
    servo1.write(s11);
    delay(frame_delay);
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
  myFile.close();
  }
  }
  else{
    Serial.println("error opening SDcard");
  }


}

String get_voice(){
  Serial.print(",record");
  Serial.print('\n');
  while( Serial.available() == 0){
    delay(10);
    if (digitalRead(18) == LOW){
      break;
      }
      }
    String voice_command = Serial.readString();
    Serial.println(voice_command);
    return voice_command;
    
}

void playback(int lengthOfRecording){
  delay(1000);
  digitalWrite(45, LOW);
  digitalWrite(46, HIGH);
  for (int x = 0; x<lengthOfRecording;x++){
     servo1.write(servoStorage[0][x]);
     servo2.write(servoStorage[1][x]);
     servo3.write(servoStorage[2][x]);
     servo4.write(servoStorage[3][x]);
     servo5.write(servoStorage[4][x]);
     delay(frame_delay);
    }
    recordflag = 0;
    digitalWrite(46, LOW);

    s11 = servoStorage[0][lengthOfRecording-1];
    s22 = servoStorage[1][lengthOfRecording-1];
    s33 = servoStorage[2][lengthOfRecording-1];
    s44 = servoStorage[3][lengthOfRecording-1];
    s55 = servoStorage[4][lengthOfRecording-1];
}

int voice_record(){

  digitalWrite(45, HIGH);
  delay(250);
  digitalWrite(45, LOW);
  delay(250);
  digitalWrite(45, HIGH);
  
  servo1.detach();
  servo2.detach();
  servo3.detach();
  servo4.detach();
  servo5.detach();
  
 for (int x = 0; x < 5; x++){
  for(int y = 0; y < frames; y++){
    servoStorage[x][y] = 90;
  }
 }
    
 int count = 0;
 while(digitalRead(40) == 0 && count < frames)
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
    servoStorage[0][count] = s1_cur_degree;
    servoStorage[1][count] = s2_cur_degree;
    servoStorage[2][count] = s3_cur_degree;
    servoStorage[3][count] = s4_cur_degree;
    servoStorage[4][count] = s5_cur_degree;
    count++;
    delay(frame_delay);
 }
  servo5.attach(servo5pin);
  servo4.attach(servo4pin);
  servo3.attach(servo3pin);
  servo2.attach(servo2pin);
  servo1.attach(servo1pin);
//  recordflag = 1;
  return count;
}

int record(){
  
  digitalWrite(45, HIGH);
  delay(250);
  digitalWrite(45, LOW);
  delay(250);
  digitalWrite(45, HIGH);
  
  servo1.detach();
  servo2.detach();
  servo3.detach();
  servo4.detach();
  servo5.detach();
  
 for (int x = 0; x < 5; x++){
  for(int y = 0; y < frames; y++){
    servoStorage[x][y] = 90;
  }
 }
    
 int count = 0;
 while(digitalRead(18) == 0 && count < frames)
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
    servoStorage[0][count] = s1_cur_degree;
    servoStorage[1][count] = s2_cur_degree;
    servoStorage[2][count] = s3_cur_degree;
    servoStorage[3][count] = s4_cur_degree;
    servoStorage[4][count] = s5_cur_degree;
    count++;
    delay(frame_delay);
 }
  servo5.attach(servo5pin);
  servo4.attach(servo4pin);
  servo3.attach(servo3pin);
  servo2.attach(servo2pin);
  servo1.attach(servo1pin);
  recordflag = 1;
  return count;
  
}

void save_recording(int lengthOfRecording, String filename){
  
  unsigned int l = filename.length();
  char c[l];
  filename.toCharArray(c, l);
  SD.remove(c);
  Serial.println("writing to file: ");
  Serial.println(c);
  
  myFile = SD.open(c, FILE_WRITE);
  if (myFile){
  
  for(int i = 0; i < lengthOfRecording; i++){
    myFile.print("s1");
    myFile.print(servoStorage[0][i]);
    myFile.print("s2");
    myFile.print(servoStorage[1][i]);
    myFile.print("s3");
    myFile.print(servoStorage[2][i]);
    myFile.print("s4");
    myFile.print(servoStorage[3][i]);
    myFile.print("s5");
    myFile.print(servoStorage[4][i]);
    myFile.print('\n');
    }
    myFile.close();
  }
  else{
    Serial.println("error opening SDcard");
  }
}

void readRecording(){
  myFile = SD.open("test.txt");
  if (myFile){
    Serial.println("test.txt:");
    
     while (myFile.available()) {
      Serial.print(myFile.read());
    }
    
  }
}


void sendRecording(int lengthOfRecording){
  
  for(int i = 0; i < lengthOfRecording; i++){
//    Serial.print("s1");
//    Serial.print(servoStorage[0][i]);
//    Serial.print("s2");
//    Serial.print(servoStorage[1][i]);
//    Serial.print("s3");
//    Serial.print(servoStorage[2][i]);
//    Serial.print("s4");
//    Serial.print(servoStorage[3][i]);
//    Serial.print("s5");
//    Serial.print(servoStorage[4][i]);
//    Serial.print('\n');

    myFile.print("s1");
    myFile.print(servoStorage[0][i]);
    myFile.print("s2");
    myFile.print(servoStorage[1][i]);
    myFile.print("s3");
    myFile.print(servoStorage[2][i]);
    myFile.print("s4");
    myFile.print(servoStorage[3][i]);
    myFile.print("s5");
    myFile.print(servoStorage[4][i]);
    myFile.print('\n');
    }
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
