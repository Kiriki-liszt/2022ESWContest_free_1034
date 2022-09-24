#include <Wire.h>

// 자신의 슬레이브 주소를 설정해 줍니다.(슬레이브 주소에 맞게 수정해야 합니다.)
#define SLAVE_nano_1 1
#define SLAVE_nano_2 2
#define SLAVE_nano_3 3
#define SLAVE_nano_4 4

#define I2C_RxTx_byte	16
char I2C_RxTx_Data[I2C_RxTx_byte]

void receiveFromMaster(int bytes);
void sendToMaster();

// 카운터
byte count = 0;
char temp;			// 마스터에서 받은 메세지

void setup() {
	Wire.begin(SLAVE_nano_3);					// Wire 라이브러리 초기화 & 슬레이브 주소 지정 
	Wire.onReceive(receiveFromMaster);			// 마스터의 데이터 수신 요구(onRev)가 있을 때 처리할 함수(revFromMas) 등록
	Wire.onRequest(sendToMaster);				// 마스터의 데이터 전송 요구(onReq)가 있을 때 처리할 함수(sendMas) 등록
	pinMode(13, OUTPUT);						// 13번 핀 = LED
	Serial.begin(9600);
}

void loop () {
	// 요청이 들어오면 13번 LED를 0.5초간 켜줍니다.
	if (temp == 'o') {
		play();
	}
}

void receiveFromMaster(int bytes) {
	char ch[2];
	for (int i = 0 ; i < bytes ; i++) {
		// 수신 버퍼 읽기
		ch[i] = Wire.read();
	}
	temp = ch[0];
}

void play() {				// 받았으니까 반응하는 부분
	digitalWrite(13, HIGH);
	delay(500);
	digitalWrite(13, LOW);
	temp = 0;
}

void sendToMaster() {
	// 마스터에게 반응할 메세지
	Wire.write("sth from sensor");
}
