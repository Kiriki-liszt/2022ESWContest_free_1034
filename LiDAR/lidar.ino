#include <Servo.h>
#include <DFRobot_TFmini.h>		// https://github.com/Cesar-S/DFRobot_TFmini


/*      Add      */
#include <Wire.h>

#define SLAVE_nano_1 1
#define SLAVE_nano_2 2
#define SLAVE_nano_3 3
#define SLAVE_nano_4 4

#define I2C_RxTx_byte	16
char I2C_RxTx_Data[I2C_RxTx_byte]

void receiveFromMaster(int bytes);
void sendToMaster();
///

SoftwareSerial  mySerial(8, 7);	// RX, TX
DFRobot_TFmini  TFmini;
Servo			servo;

#define TRIG 7					// 트리거는 2번
#define ECHO 8					// 에코는 3번

void Rotatemotor();
bool CompareArray(int ia[]);

uint16_t distance;
String dist_string;
String rad_string;

const int			row = 6;
const int		 column = 3;
int 	personarr[row]	= {0, 0, 0, 0, 0, 0};
int new_personarr[row]	= {0, 0, 0, 0, 0, 0};

boolean first_flag = false;

int servoDirection = 1;
int rad = 0; // rad는 각도를 의미합니다.


void setup() {
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);

	// pinMode(TRIG, OUTPUT);
	// pinMode(ECHO, INPUT);
	servo.attach(6); //서보모터 핀은 9번
	TFmini.begin(mySerial);

	/*      Add      */
	Wire.begin(SLAVE_nano_3);					// Wire 라이브러리 초기화 & 슬레이브 주소 지정 
	Wire.onReceive(receiveFromMaster);			// 마스터의 데이터 수신 요구(onRev)가 있을 때 처리할 함수(revFromMas) 등록
	Wire.onRequest(sendToMaster);				// 마스터의 데이터 전송 요구(onReq)가 있을 때 처리할 함수(sendMas) 등록
	///
}

void loop() {
	if (TFmini.measure()) {
		distance = TFmini.getDistance();

		int modular = distance % 5;
		distance = distance - modular;
		
		// print Radian
		if ( servoDirection < 0 ) {
			Serial.print("r");
			Serial.print(rad + 5);
		} else {
			Serial.print("r");
			Serial.print(rad);
		}

		// print Distance
		Serial.print("d");
		Serial.println(distance);

		Rotatemotor();
	}
	delay(30); //서보모터가 움직이는 걸리는 시간을 줍니다.
}

void Rotatemotor() {
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
	for (int k=0; k<6; k++) {
		if(ia[k] !=  personarr[k]) {
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
	temp = ch[0];
}

void sendToMaster() {
	// 마스터에게 반응할 메세지
	Wire.write(radstring);
	Wire.write(diststring);
}
///