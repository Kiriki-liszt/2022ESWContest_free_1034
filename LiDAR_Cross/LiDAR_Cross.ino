/*      Add      */
#include	<Wire.h>

#define		SLAVE_nano_1	0
#define		SLAVE_nano_2	1
#define		SLAVE_NUM		2

int			SLAVE_nano[SLAVE_NUM] = {SLAVE_nano_1, SLAVE_nano_2};		// 슬레이브 주소

void 		receiveFromMaster(int bytes);
void		sendToMaster();

bool		LiDAR_flag;
///

#include	<Servo.h>  //서보 라이브러리를 불러온다
#include	<DFRobot_TFmini.h>

SoftwareSerial	TFmini_Serial(8, 7); // RX, TX

Servo			servo;
DFRobot_TFmini	TFmini;

#define		TRIG	7			//트리거는 2번
#define		ECHO	8			//에코는 3번

uint16_t		distance;

int				servoDirection = 3;
int				rad = 0;				// rad는 각도를 의미합니다.

float			W = 10;
float			H = 50;
const int		row = 5;
const int		column = 5;
int					personarr[row][column]	= {0, };
int				new_personarr[row][column]	= {0, };

boolean			first_flag = false;
 
void setup() {
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);
	//pinMode(TRIG, OUTPUT);
	//pinMode(ECHO, INPUT);
	servo.attach(6); //서보모터 핀은 9번

	TFmini.begin(TFmini_Serial);

	/*      Add      */
	Wire.begin(SLAVE_nano_1);					// Wire 라이브러리 초기화 & 슬레이브 주소 지정 
	Wire.onReceive(receiveFromMaster);			// 마스터의 데이터 수신 요구(onRev)가 있을 때 처리할 함수(revFromMas) 등록
	Wire.onRequest(sendToMaster);				// 마스터의 데이터 전송 요구(onReq)가 있을 때 처리할 함수(sendMas) 등록
	///
}

void loop() {
	if(TFmini.measure()) {
		distance= TFmini.getDistance();
		int modular = distance /10;

		if(servoDirection<0) {
			Serial.print("r");
			Serial.print(rad+5);
		} else {
			Serial.print("r");
			Serial.print(rad);
		}
		Serial.print("d");
		Serial.println(distance);

		float radi=rad*PI /180;

		float width = distance * cos(radi);
		float height = distance * sin(radi);

		Serial.print("width : ");
		Serial.println(width);
		Serial.print("height : ");
		Serial.println(height);

		if(width < W && height < H)	{			// 감지 시 할 것
			LiDAR_flag = true;
			/*
			digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
			delay(1000);                       // wait for a second
			digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
			*/
		} else {LiDAR_flag = false;}

		Rotatemotor();
	}
	delay(30); //서보모터가 움직이는 걸리는 시간을 줍니다.
}


void Rotatemotor() {
	rad += servoDirection;
	if (rad > 90) {
		servoDirection = -3;
		rad -=6;
	}
	else if (rad < 0) {
		ervoDirection = +6;
		rad +=6;
	}
	servo.write(rad);
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
	Wire.write(LiDAR_flag);
}
///