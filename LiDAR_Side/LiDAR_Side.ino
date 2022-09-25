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

#include	<Servo.h>  //서보 라이브러리를 불러온다
#include	<DFRobot_TFmini.h>

SoftwareSerial	TFmini_Serial(8, 7); // RX, TX

Servo			servo;
DFRobot_TFmini	TFmini;

#define		TRIG	7			//트리거는 2번
#define		ECHO	8			//에코는 3번

uint16_t		distance;

int				servoDirection = 1;
int				rad = 0; // rad는 각도를 의미합니다.

const int		row = 5;
const int		column = 5;
int 		personarr[row][column]	= {0, };
int		new_personarr[row][column]	= {0, };

boolean			first_flag = false;
 
void setup() {
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);
	//pinMode(TRIG, OUTPUT);
	//pinMode(ECHO, INPUT);
	servo.attach(6); //서보모터 핀은 9번
	TFmini.begin(TFmini_Serial);

	/*      Add      */
	Wire.begin(SLAVE_nano_2);					// Wire 라이브러리 초기화 & 슬레이브 주소 지정 
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

		switch(modular) {
			case 0:
				if(rad < 18) {
					new_personarr[4][4] = 1;
				} else if(18 <= rad && rad < 36) {
					new_personarr[3][4] = 1;
				} else if(36 <= rad && rad < 54) {
					new_personarr[2][4] = 1;
				} else if(54 <= rad && rad < 72) {
					new_personarr[1][4] = 1;
				} else if(72 <= rad && rad <= 90) {
					new_personarr[0][4] = 1;
				}
				break;

			case 1:
				if(rad<18) {
					new_personarr[4][3] = 1;
				} else if(18 <= rad && rad < 36) {
					new_personarr[3][3] = 1;
				} else if(36 <= rad && rad < 54) {
					new_personarr[2][3] = 1;
				} else if(54 <=rad && rad < 72) {
					new_personarr[1][3] = 1;
				} else if(72 <=rad && rad <= 90) {
					new_personarr[0][3] = 1;
				}
				break;	

			case 2:
				if(rad < 18) {
					new_personarr[4][2] = 1;
				} else if(18 <= rad && rad < 36) {
					new_personarr[3][2] = 1;
				} else if(36 <= rad && rad < 54) {
					new_personarr[2][2] = 1;
				} else if(54 <= rad && rad < 72) {
					new_personarr[1][2] = 1;
				} else if(72 <= rad && rad <= 90) {
					new_personarr[0][2] = 1;
				}
				break;

			case 3:
				if(rad<18)
				{
				new_personarr[4][1] = 1;
				}
				else if(18<=rad &&rad <36)
				{
				new_personarr[3][1] = 1;
				}
				else if(36<=rad &&rad <54)
				{
				new_personarr[2][1] = 1;
				}
				else if(54<=rad &&rad <72)
				{
				new_personarr[1][1] = 1;
				}
				else if(72<=rad &&rad <=90)
				{
				new_personarr[0][1] = 1;
				}
				break;

			case 4:
				if(rad<18)
				{
				new_personarr[4][0] = 1;
				}
				else if(18<=rad &&rad <36)
				{
				new_personarr[3][0] = 1;
				}
				else if(36<=rad &&rad <54)
				{
				new_personarr[2][0] = 1;
				}
				else if(54<=rad &&rad <72)
				{
				new_personarr[1][0] = 1;
				}
				else if(72<=rad &&rad <=90)
				{
				new_personarr[0][0] = 1;
				}
				break;
	
			default:
				break;
		}

		if(rad == 90 || rad ==0) {
			Serial.print("janghnassssssssssssssssssssssssssssssssssssssssssssss\n");
			if(!first_flag) {
				Serial.println("firstflag_janghanssssssssss");
				for (int k=0; k<5; k++) {
					for(int j = 0; j<5; j++)
					{
						personarr[k][j] = new_personarr[k][j];
					}
				}
			first_flag = true;
			}
			else {
				Serial.println("comparearray_janghanssssssssss");
				if(!CompareArray(new_personarr)) {
					Serial.println("comparearray_false_janghanssssssssss");
					/* 사람 감지되서 할 것 */
					LiDAR_flag = true;
					
					// digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
					delay(1000);                       // wait for a second
					// digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
				
					first_flag = false;
				} else {LiDAR_flag = false;}
			}

			for (int k=0; k<5; k++) {
				for(int j = 0; j<5; j++) {
					new_personarr[k][j] = 0;
				}
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

bool CompareArray(int ia[][5]) {
   //int size = sizeof(ia) / sizeof(int);
	for (int k=0; k<5; k++)
	{
		for(int j = 0; j<5; j++)
		{
			if(ia[k][j] !=  personarr[k][j])
		{
			return false;
		}
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
	Wire.write(2);
	// Wire.write(LiDAR_flag);
}
///