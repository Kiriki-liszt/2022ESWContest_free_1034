#include <Servo.h>  //서보 라이브러리를 불러온다        ////////// 횡단보도 쪽
#include <DFRobot_TFmini.h>
#define TRIG 2    //트리거는 2번
#define ECHO 3    //에코는 3번
Servo servo;

SoftwareSerial mySerial(8, 7); // RX, TX
DFRobot_TFmini  TFmini;

int servoDirection = 1, rad = 0; // rad는 각도를 의미합니다.
uint16_t distance, strength;   // 거리와 강도를 담는 변수
uint16_t d = 0;
//int a[2][3] = { 0 }; // 각도, 거리 + 1, 존재여부
int flag = 0; // 있다 없다


void setup() {
 Serial.begin(115200);
 //pinMode(TRIG, OUTPUT);
 //pinMode(ECHO, INPUT);
 TFmini.begin(mySerial);
 servo.attach(6); //서보모터 핀은 9번
 pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
 if (TFmini.measure()) {// 거리와 신호의 강도를 측정합니다. 성공하면 을 반환하여 if문이 작동합니다.
   if(TFmini.getDistance() < 30) {          // 최대 거리 = 90 cm
     distance = TFmini.getDistance();       // 거리값을 cm단위로 불러옵니다.
     d = distance/10;
   }
   else
   {
     d = 3;
   }

   strength = TFmini.getStrength();       // 신호의 강도를 불러옵니다. 측정 대상이 넓으면 강도가 커집니다.

 //5800이면 1m 입니다. 최대 기다리는 시간은 1,000,000 입니다.
 // 5800을 58로 나누게 되면 cm 단위가 됩니다.
 //long distance = pulseIn(ECHO, HIGH, 5800) / 58; //5800uS 동안 기다렸으므로 1미터 측정이 된다.
 //Serial.print("r");
 //Serial.print(rad);
 //Serial.print("d");
 //Serial.println(distance);

 rad += servoDirection;

 if (rad > 90) {
   rad = 89;
   servoDirection = -3;
   flag = 0;
 }
 else if (rad < 90 && rad >= 0) {
   if(d >= 0 && d < 3) { // 최대거리(90cm) 안에 들어올 경우
     flag = 1; //
   }
 }
 else if (rad < 0) {
   rad = 1;
   servoDirection = 3;
   flag = 0;
 }
 servo.write(rad);

 delay(60); //서보모터가 움직이는 걸리는 시간을 줍니다.

 }
}