/*      Add      */
#include <Wire.h>

#define SLAVE_nano_1 1
#define SLAVE_nano_2 2
#define SLAVE_nano_3 3
#define SLAVE_nano_4 4

#define I2C_RxTx_byte	16
char I2C_RxTx_Data[I2C_RxTx_byte];

void receiveFromMaster(int bytes);
void sendToMaster();
///

#include <Servo.h>  //서보 라이브러리를 불러온다
#include <DFRobot_TFmini.h> 		// https://github.com/Cesar-S/DFRobot_TFmini


SoftwareSerial mySerial(8, 7); // RX, TX
DFRobot_TFmini  TFmini;
#define TRIG 7   //트리거는 2번
#define ECHO 8    //에코는 3번
Servo servo;

uint16_t distance;

const int  row = 6;
const int  column = 3;
int personarr[row] = { 0,0,0,0,0,0};
int new_personarr[row]={0,0,0,0,0,0};

boolean first_flag = false;

int servoDirection = 1, rad = 0; // rad는 각도를 의미합니다.
 
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(TRIG, OUTPUT);
  //pinMode(ECHO, INPUT);
  servo.attach(6); //서보모터 핀은 9번
  TFmini.begin(mySerial);
  	/*      Add      */
	Wire.begin(SLAVE_nano_3);					// Wire 라이브러리 초기화 & 슬레이브 주소 지정 
	Wire.onReceive(receiveFromMaster);			// 마스터의 데이터 수신 요구(onRev)가 있을 때 처리할 함수(revFromMas) 등록
	Wire.onRequest(sendToMaster);				// 마스터의 데이터 전송 요구(onReq)가 있을 때 처리할 함수(sendMas) 등록
	///

}

void loop() {
 if(TFmini.measure()) {
 
  distance= TFmini.getDistance();
  int modular = distance %5;
  distance = distance - modular;
  if(servoDirection<0)
  {
  Serial.print("r");
  Serial.print(rad+5);
  }
  else
  {
  Serial.print("r");
  Serial.print(rad);
  }
  Serial.print("d");
  Serial.println(distance);

if(distance <=40)
  {
	if(rad <=30  && distance <=20 )
	{
	  //new_personarr[1][2] = 1;
	  new_personarr[5] = 1;
	}
	else if(rad <=30  && 20<distance && distance <=40 )
	{
	  //new_personarr[0][2] = 1;
	  new_personarr[4] = 1;
	}
	else if(30 < rad  && rad <=60  && distance <=20 )
	{
	  //new_personarr[1][1] = 1;
	  new_personarr[3] = 1;
	}
	else if(30 < rad  && rad <=60  && 20<distance && distance <=40  )
	{
	  //new_personarr[0][1] = 1;
	  new_personarr[2] = 1;
	}
	else if(60 < rad  &&rad <=90  && distance <=20 )
	{
	 //new_personarr[1][0] = 1;
	 new_personarr[1] = 1;
	}
	else if(60 < rad  &&rad <=90  && 20<distance && distance <=40 )
	{
	  //new_personarr[0][0] = 1;
	  new_personarr[0] = 1;
	}
  }
 
  for (int k=0; k<6; k++)
	{
	 Serial.println(personarr[k]);
	}
	Serial.println("***************************************************");

	for (int k=0; k<6; k++)
	{
	 Serial.println(new_personarr[k]);
	}
   
  if(rad == 60)
  {
	Serial.print("60도일 때의 거리값: ");
	Serial.println(distance);
  }
 
 
  if(rad == 90 || rad ==0)
  {
	Serial.print("janghnassssssssssssssssssssssssssssssssssssssssssssss\n");
	 if(!first_flag)
  {
	Serial.println("firstflag_janghanssssssssss");
	for (int k=0; k<6; k++)
	{
	  personarr[k] = new_personarr[k];
	}
	first_flag = true;
  }
  else
  {
	Serial.println("comparearray_janghanssssssssss");
	if(!CompareArray(new_personarr))
  {
	Serial.println("comparearray_false_janghanssssssssss");
   for (int k=0; k<6; k++)
	{
	  personarr[k] = new_personarr[k];
	}
	digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(2000);    
  first_flag = false;
  }
 
  }

  for (int k=0; k<6; k++)
	{
	 new_personarr[k] = 0;
	}
  }
  Rotatemotor();
 
 }
 
  delay(30); //서보모터가 움직이는 걸리는 시간을 줍니다.
}

void Rotatemotor()
{
   rad += servoDirection;
  if (rad > 90) {
	servoDirection = -1;
	rad -=2;
   
  }
  else if (rad < 0) {
   servoDirection = +1;
	rad +=2;
  }
  servo.write(rad);
}

bool CompareArray(int ia[]) {
   int size = sizeof(ia) / sizeof(int);
	for (int k=0; k<6; k++)
	{
	  if(ia[k] !=  personarr[k])
	  {
		return false;
	  }
	 
	}

	return true;
	   
}


/*      Add      */
void receiveFromMaster(int bytes) {
	char ch[2];
	for (int i = 0 ; i < bytes ; i++) {
		// 수신 버퍼 읽기
		ch[i] = Wire.read();
	}
}

void sendToMaster() {
	// 마스터에게 반응할 메세지
	Wire.write(rad);
	Wire.write(distance);
}
///
