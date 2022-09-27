#include <Arduino.h>
#line 1 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\SIDE\\SIDE.ino"
/*      Add      */
#include	<Wire.h>

#define		SLAVE_nano_1	1
#define		SLAVE_nano_2	2
#define		SLAVE_NUM		2

int			SLAVE_nano[SLAVE_NUM] = {SLAVE_nano_1, SLAVE_nano_2};		// 슬레이브 주소

void 		receiveFromMaster(int bytes);
void		sendToMaster();

int			LiDAR_flag;
///

#include <Servo.h>  //서보 라이브러리를 불러온다
#include <DFRobot_TFmini.h>

SoftwareSerial mySerial(8, 7); // RX, TX
DFRobot_TFmini  TFmini;
#define TRIG 7   //트리거는 2번
#define ECHO 8    //에코는 3번
Servo servo;

uint16_t distance;

const int  row = 4;
const int  column = 3;
int personarr[row] = { 0};
int new_personarr[row]={0};
int rii = 30;

boolean first_flag = false;

int servoDirection = 1, rad = 0; // rad는 각도를 의미합니다.

 
#line 38 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\SIDE\\SIDE.ino"
void setup();
#line 56 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\SIDE\\SIDE.ino"
void loop();
#line 166 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\SIDE\\SIDE.ino"
void Rotatemotor();
#line 181 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\SIDE\\SIDE.ino"
bool CompareArray(int ia[]);
#line 38 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\SIDE\\SIDE.ino"
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  Wire.begin();
  //pinMode(TRIG, OUTPUT);
  //pinMode(ECHO, INPUT);
  servo.attach(6); //서보모터 핀은 9번
  TFmini.begin(mySerial);


	/*      Add      */
	Wire.begin(SLAVE_nano_2);					// Wire 라이브러리 초기화 & 슬레이브 주소 지정 
	Wire.onReceive(receiveFromMaster);			// 마스터의 데이터 수신 요구(onRev)가 있을 때 처리할 함수(revFromMas) 등록
	Wire.onRequest(sendToMaster);				// 마스터의 데이터 전송 요구(onReq)가 있을 때 처리할 함수(sendMas) 등록
	///
}

void loop() {
 if(TFmini.measure()) {

  
 
  distance= TFmini.getDistance();
 
 
 
  if(servoDirection<0)
  {
  Serial.print("r");
  Serial.print(rad+5);
  Serial.print("d");
  Serial.println(distance);
  }
  else if(distance >=30)
  {
    Serial.print("r");
  Serial.print(rad);
    Serial.print("d");
  Serial.println(rii);
  }
  else
  {
  Serial.print("r");
  Serial.print(rad);
  Serial.print("d");
  Serial.println(distance);
  }
  

if(distance <=30)
  {
    if(rad <=45 && distance <=15 )
    {
      //new_personarr[1][2] = 1;
      new_personarr[3] = 1;
    }
    else if(rad <=45  && 15<distance && distance <=30 )
    {
      //new_personarr[0][2] = 1;
      new_personarr[2] = 1;
    }
    else if(45 < rad  && rad <=90  && distance <=15 )
    {
      //new_personarr[1][1] = 1;
      new_personarr[1] = 1;
    }
    else if(40 < rad  && rad <=90  && 15<distance && distance <=30  )
    {
      //new_personarr[0][1] = 1;
      new_personarr[0] = 1;
    }
   
  }
 
  for (int k=0; k<4; k++)
    {
      Serial.println(personarr[k]) ;
    }

     for (int k=0; k<4; k++)
    {
      Serial.println( new_personarr[k]);
    }
 
  if(rad >= 90 || rad <=0)
  {
 
     if(!first_flag)
  {
   
    for (int k=0; k<4; k++)
    {
      personarr[k] = new_personarr[k];
    }
    first_flag = true;
  }
  else
  {
   
    if(!CompareArray(new_personarr)) {
   
   for (int k=0; k<4; k++)
    {
      personarr[k] = new_personarr[k];
    }
					/* 사람 감지되서 할 것 */
					LiDAR_flag = 1;
   for (int k=0; k<4; k++)
    {
      personarr[k] = new_personarr[k];
    }
  }else { LiDAR_flag = 0; }
 
  }

  for (int k=0; k<4; k++)
    {
     new_personarr[k] = 0;
    }
  }
  Rotatemotor();
 
 }
 
  delay(10); //서보모터가 움직이는 걸리는 시간을 줍니다.
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
    for (int k=0; k<4; k++)
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
	// Wire.write(2);
	Wire.write(LiDAR_flag);
}
///
