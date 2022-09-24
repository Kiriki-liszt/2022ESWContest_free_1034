/***********************************/
/*             NodeMCU             */
/***********************************/

#include	<SPI.h>
#include	<RF24.h>
#include	<nRF24L01.h>
#include	<printf.h>

// SPI 버스에 nRF24L01 라디오를 설정하기 위해 CE, CSN 선언.
// nodeMCU Rx
#define		Rx_CE		15
#define		Rx_SCN		2

//optionally, reduce the payload size.  seems to improve reliability.
#define		Payload_size	8

//주소값을 5가지 문자열로 변경할 수 있으며, 송신기와 수신기가 동일한 주소로 해야됨.
const byte address[6] = "00001";

///////////////////////////////////////////// Rx 시작

void nRF_message_parsing();
void nRF_message_print();

RF24 radio(Rx_CE, Rx_SCN); 

void setup() {

	Serial.begin(9600);
	Serial.println("Rx Start");

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