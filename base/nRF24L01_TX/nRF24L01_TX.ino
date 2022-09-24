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


///////////////////////////////////////////// Tx 시작

void nRF_make_signal();
void nRF_make_message();
void nRF_prnt_message();

void I2C_Tx (int salves);
void I2C_Req(int slaves);

RF24 radio(Tx_CE, Tx_SCN); 

void setup() {
	Serial.begin(9600);
	Serial.println("Tx Start");
	
	Wire.setSDA(PB9);				// nucleo-64 F103RB alt pins for I2C
	Wire.setSCL(PB8);
	Wire.begin();					// Wire 라이브러리 초기화

	radio.begin();
	radio.setPayloadSize(Payload_size);     //optionally, reduce the payload size.  seems to improve reliability.
	radio.openWritingPipe(address);         //이전에 설정한 5글자 문자열인 데이터를 보낼 수신의 주소를 설정
	radio.setPALevel(RF24_PA_HIGH);
	// 전원공급에 관한 파워레벨을 설정합니다. 모듈 사이가 가까우면 최소로 설정합니다.
	// 거리가 가까운 순으로 RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH 등으로 설정할 수 있습니다.  RF24_PA_MAX는 레거시.
	// 높은 레벨(거리가 먼 경우)은 작동하는 동안 안정적인 전압을 가지도록 GND와 3.3V에 바이패스 커패시터 사용을 권장함.
	radio.enableDynamicAck();
	radio.stopListening();  //모듈을 송신기로 설정
}


// 상태는 총 4개 : 00, 01, 10, 11
// 상황은 앞에서부터 차례대로 우회전, 비보호, 로터리 등등
unsigned char nRF_send_buff;
unsigned char right_turn;
unsigned char left_turn;
unsigned char unused_1;
unsigned char unused_2;

unsigned char car_flag;

unsigned long timer_set = 0;

bool rslt;
unsigned char cast = 0;	// 0: single ; 1: broad


void loop() {
	delay(1000); // Every 1sec

	// I2C 
	byte nano_addr;
	for (nano_addr = 0; nano_addr < SLAVE_NUM; nano_addr++) {
		I2C_Tx(nano_addr);
		delay(10);
		// 슬레이브로 데이터 요구 및 수신 데이터 처리
		I2C_Req(nano_addr);
		delay(10);
	}

	// RF
	nRF_make_signal();
	nRF_make_message();

	if ( cast == 0 ) {		// singlecast
		rslt = radio.write( &send_buff, sizeof(send_buff), false );
		if (rslt) {
			car_flag = 1;					// 차 있음
			timer_set = millis();			// 타이머 시작
			cast = 1;						// to broadcast
		} else if ((millis() - timer_set) > TIME_CAR) {
			car_flag = 0;
		}
	} else {				// broadcast
		radio.write( &send_buff, sizeof(send_buff), true );
		if ((millis() - timer_set) > TIME_BCT) {
			cast = 0;			// to singlecast
		}
	}
	

	nRF_prnt_message();

	// default : singlecast
	// radio.setAutoAck(true);
}

void I2C_Tx (int salves) {
	Wire.beginTransmission(SLAVE_nano[slaves]);				// I2C 통신을 통한 전송 시작
	Wire.write("Data Send");
	Wire.endTransmission(SLAVE_nano[slaves]);
}

void I2C_Req(int slaves) {
	Wire.requestFrom(SLAVE_nano[slaves], I2C_RxTx_byte);		// n 바이트 크기의 데이터 요청
	for (int i = 0 ; i < I2C_RxTx_byte ; i++) {	I2C_RxTx_Data[i] = NULL	}
	for (int i = 0 ; i < I2C_RxTx_byte ; i++) {				// n 바이트 모두 출력할 때까지 반복
		I2C_RxTx_Data[i] = Wire.read();						// 수신 데이터 읽기
	}
	Serial.println(I2C_RxTx_Data);
}

void nRF_make_signal() {
	right_turn	= 1;
	left_turn	= 2;
	unused_1	= 0;
	unused_2	= 3;
}

void nRF_make_message() {
	left_turn	<<= 2;
	unused_1	<<= 4;
	unused_2	<<= 6;

	nRF_send_buff	= right_turn;
	nRF_send_buff	|= left_turn;
	nRF_send_buff	|= unused_1;
	nRF_send_buff	|= unused_2;
}

void nRF_prnt_message() {
	Serial.print("ss : ");	Serial.println(nRF_send_buff);
	Serial.print("rr : ");	Serial.println(right_turn);
	Serial.print("ll : ");	Serial.println(left_turn);
	Serial.print("u1 : ");	Serial.println(unused_1);
	Serial.print("u2 : ");	Serial.println(unused_2);
	Serial.print("car_flag : ");	Serial.println(car_flag);
	Serial.print("cast : ");	Serial.println(cast);
	Serial.print("rslt : ");	Serial.println(rslt);
}



/*
setup {
	// if( radio.setDataRate( RF24_250KBPS ) ) { printf( "Data rate 250KBPS set!\n\r" ); } 
	// else { printf( "Data rate 250KBPS set FAILED!!\n\r" ); }
	// radio.setDataRate( RF24_2MBPS );
	// radio.enableDynamicPayloads();
	// radio.setRetries(5, 0);
	// radio.setAutoAck(false);
	// radio.powerUp();
}

// char sendBuffer[Payload_size];
// uint8_t counter = 1;
void loop() {
	// sprintf(sendBuffer, "00011011");
	// sprintf(sendBuffer, "%d|root", counter);
	// const char text[] = "Hi";
	// radio.write(&text, sizeof(text));  //해당 메시지를 수신자에게 보냄
	// radio.write( sendBuffer, sizeof(sendBuffer) );
	// counter++;
	// delay(1000); // Every 1sec
}
*/