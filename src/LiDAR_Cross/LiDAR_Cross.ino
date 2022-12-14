/*      Add      */
#include	<Wire.h>

#define		SLAVE_nano_1	1
#define		SLAVE_nano_2	2
#define		SLAVE_NUM		2

int			SLAVE_nano[SLAVE_NUM] = {SLAVE_nano_1, SLAVE_nano_2};		// 슬레이브 주소

void 		receiveFromMaster(int bytes);
void		sendToMaster();

int			LiDAR_flag;
int         flag0;
float		W = 12;
float		H = 10;
float		radi;
float		width;
float		height;
///

////////// 횡단보도 쪽
#include	<Servo.h>  //서보 라이브러리를 불러온다
#include	<DFRobot_TFmini.h>

#define		TRIG		2    //트리거는 2번
#define		ECHO		3    //에코는 3번

SoftwareSerial	TFmini_Serial(8, 7); // RX, TX

DFRobot_TFmini	TFmini;
Servo			servo;

int	 servoDirection = 1;
int 			rad = 0; // rad는 각도를 의미합니다.
uint16_t distance, strength;   // 거리와 강도를 담는 변수
uint16_t d = 0;
//int a[2][3] = { 0 }; // 각도, 거리 + 1, 존재여부


void setup() {
	Serial.begin(115200);
	//pinMode(TRIG, OUTPUT);
	//pinMode(ECHO, INPUT);
	servo.attach(6); //서보모터 핀은 9번
	pinMode(LED_BUILTIN, OUTPUT);
	
	TFmini.begin(TFmini_Serial);

	/*      Add      */
	Wire.begin(SLAVE_nano_1);					// Wire 라이브러리 초기화 & 슬레이브 주소 지정 
	Wire.onReceive(receiveFromMaster);			// 마스터의 데이터 수신 요구(onRev)가 있을 때 처리할 함수(revFromMas) 등록
	Wire.onRequest(sendToMaster);				// 마스터의 데이터 전송 요구(onReq)가 있을 때 처리할 함수(sendMas) 등록
	///
}

void loop() {
	if (TFmini.measure()) {						// 거리와 신호의 강도를 측정합니다. 성공하면 을 반환하여 if문이 작동합니다.
        if(TFmini.getDistance()<14){
		distance = TFmini.getDistance();
		flag0 = 1;	// 거리값을 cm단위로 불러옵니다.
	    }
		//add
		//distance = TFmini.getDistance();
		if(TFmini.getDistance() >= 14){
			distance = 14;
		} 
		
		rad += servoDirection;

//		radi = rad*PI/180;
//		width =  distance*cos(radi);
//		height = distance*sin(radi);

//		if(width<W && height<H){
//			flag0 = 1;
//		}


		// 5800이면 1m 입니다. 최대 기다리는 시간은 1,000,000 입니다.
		// 5800을 58로 나누게 되면 cm 단위가 됩니다.
		// long distance = pulseIn(ECHO, HIGH, 5800) / 58; //5800uS 동안 기다렸으므로 1미터 측정이 된다.
		Serial.print("r");
		Serial.print(rad + 90);
		Serial.print("d");
		Serial.println(distance);


		if (rad > 90) {
			LiDAR_flag = flag0;
			flag0 = 0;
			rad = 89;
			servoDirection = -1;
			//LiDAR_flag = 0;		
		}
		else if (rad < 0) {
			LiDAR_flag = flag0;
			flag0 = 0;
			rad = 1;
			servoDirection = 1;
			//LiDAR_flag = 0;		
		}
		servo.write(rad);

		delay(20); //서보모터가 움직이는 걸리는 시간을 줍니다.
	}
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
	// Wire.write(1);
	Wire.write(LiDAR_flag);
}
///