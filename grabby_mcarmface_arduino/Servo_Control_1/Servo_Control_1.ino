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
//        analog 89-474


// Pinch: 0-180
//        75-100-170
//        263-460

Servo servo1;
#define servo1pin 13

String incoming_string;
String data;
int servo_angles[5];
int inputlength = 0;
int res;

int s11;
int s22;
int s33;
int s44;
int s55;


void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2);
  servo1.attach(servo1pin);
}

void loop() {

  serial_read();
  Serial.println(s11);
  Serial.println(s22);
  delay(100);
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
      res = sscanf(inputbuffer, "s1%d s2%d s3%d s4%d s5%d", &s1, &s2, &s3, &s4, &s5);
      inputlength = 0;\
      s11 = s1;
      s22 = s2;
      s33 = s3;
      s44 = s4;
      s55 = s5;
      
    }
    else{
      inputbuffer[inputlength] = inchar;
      inputlength++;
    }
}


  
}

