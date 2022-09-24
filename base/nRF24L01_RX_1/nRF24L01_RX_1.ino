// 동일한 부분

#include	<SPI.h>
#include	<RF24.h>
#include	<nRF24L01.h>
#include	<printf.h>
#include	<Wire.h>

// SPI 버스에 nRF24L01 라디오를 설정하기 위해 CE, CSN 선언.
// STM Tx
#define		Tx_CE		10
#define		Tx_SCN		8
// nodeMCU Rx
#define		Rx_CE		15
#define		Rx_SCN		2
// Timer
#define		TIME_BCT	10000
#define		TIME_CAR	20000

//optionally, reduce the payload size.  seems to improve reliability.
#define		Payload_size	8

//주소값을 5가지 문자열로 변경할 수 있으며, 송신기와 수신기가 동일한 주소로 해야됨.
const byte address[6] = "00001";

#define		SLAVE_nano_1	1
#define		SLAVE_nano_2	2
#define		SLAVE_nano_3	3
#define		SLAVE_nano_4	4
#define		SLAVE_NUM		4
int SLAVE_nano[4] = {SLAVE_nano_1, SLAVE_nano_2, SLAVE_nano_3, SLAVE_nano_3};		// 슬레이브 주소

#define I2C_RxTx_byte	16
char I2C_RxTx_Data[I2C_RxTx_byte];


///////////////////////////////////////////// Rx 시작

void receiveFromMaster(int bytes);
void sendToMaster();

void nRF_message_parsing();
void nRF_message_print();

RF24 radio(Rx_CE, Rx_SCN); 

void setup() {

	Serial.begin(9600);
	Serial.println("Rx Start");

	Wire.begin(SLAVE_nano_3);					// Wire 라이브러리 초기화 & 슬레이브 주소 지정 
	Wire.onReceive(receiveFromMaster);			// 마스터의 데이터 수신 요구(onRev)가 있을 때 처리할 함수(revFromMas) 등록
	Wire.onRequest(sendToMaster);				// 마스터의 데이터 전송 요구(onReq)가 있을 때 처리할 함수(sendMas) 등록
	pinMode(13, OUTPUT);						// 13번 핀 = LED

	radio.begin();
	radio.setPayloadSize(Payload_size);
	radio.openReadingPipe(0, address);
	radio.setPALevel(RF24_PA_HIGH);
	radio.enableDynamicAck();
	radio.startListening(); //모듈을 수신기로 설정
}

unsigned char nRF_read_buff;
unsigned char right_turn;
unsigned char left_turn;
unsigned char unused_1;
unsigned char unused_2;


void loop() {
	if (radio.available()) {
		radio.read(&nRF_read_buff, sizeof(nRF_read_buff));
		nRF_message_parsing();
		nRF_message_print();
	}
}

void play() {				// 받았으니까 반응하는 부분
	digitalWrite(13, HIGH);
	delay(500);
	digitalWrite(13, LOW);
	temp = 0;
}

void receiveFromMaster(int bytes) {
	/*
	char ch[2];
	for (int i = 0 ; i < bytes ; i++) {
		// 수신 버퍼 읽기
		ch[i] = Wire.read();
	}
	temp = ch[0];
	*/
	play();
}

void sendToMaster() {
	// 마스터에게 반응할 메세지
	Wire.write("sth from sensor");
}


void nRF_message_parsing() {
	right_turn  = nRF_read_buff & 3;
	left_turn   = nRF_read_buff & 12;
	unused_1    = nRF_read_buff & 48;
	unused_2    = nRF_read_buff & 192;

	left_turn >>= 2;
	unused_1  >>= 4;
	unused_2  >>= 6;
}

void nRF_message_print() {
	Serial.println("start");
	Serial.print("ss : ");	Serial.println(nRF_read_buff);
	Serial.print("rr : ");	Serial.println(right_turn);
	Serial.print("ll : ");	Serial.println(left_turn);
	Serial.print("u1 : ");	Serial.println(unused_1);
	Serial.print("u2 : ");	Serial.println(unused_2);
	Serial.println("end\n");
}







/*
loop
		// char readBuffer[Payload_size] = "";
		// radio.read(&readBuffer, sizeof(readBuffer));
		// Serial.println(readBuffer);
		// char text[32] = "";
		// radio.read(&text, sizeof(text));
		// Serial.println(text);
*/