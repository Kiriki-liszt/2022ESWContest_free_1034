/***********************************/
/*             NodeMCU             */
/***********************************/

#include	<SPI.h>
#include	<RF24.h>
#include	<nRF24L01.h>
#include	<printf.h>
#include	"Arduino.h"
#include	"SoftwareSerial.h"
#include	"DFRobotDFPlayerMini.h"

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

SoftwareSerial mySoftwareSerial(5, 4); // RX, TX of Board <-> connect to Tx, Rx of Module
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup() {
	Serial.begin(115200);
	Serial.println("Rx Start");

	mySoftwareSerial.begin(9600);
	myDFPlayer.begin(mySoftwareSerial);
	myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
	myDFPlayer.volume(30);  //Set volume value (0~30).
	myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
	myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
	// 1 : 운전 그렇게 하는거 아닌데
	// 2 : 우회전 불가능
	// 3 : 우회전 가능
	// 4 : 전방 500m
	// myDFPlayer.playMp3Folder(4); // play specific mp3 in SD:/MP3/0004.mp3; File Name(0~65535)

	radio.begin();
	radio.setPayloadSize(Payload_size);
	radio.openReadingPipe(0, address);
	radio.setPALevel(RF24_PA_HIGH);
	radio.enableDynamicAck();
	radio.startListening(); //모듈을 수신기로 설정
}

// 상태는 총 4개 : 00, 01, 10, 11 (0, 1, 2, 3)
// 상황은 앞에서부터 차례대로 우회전, 비보호, 로터리 등등
unsigned char	nRF_read_buff;
unsigned char	status_before[4];
unsigned char	status_now[4];
bool 			status_flag[4] = {false, };

void loop() {
	if (radio.available()) {
		radio.read(&nRF_read_buff, sizeof(nRF_read_buff));
		nRF_message_parsing();
		nRF_message_print();
		for (int i = 0 ; i < 4 ; i++ ) {
			if (status_flag[i]) {
				myDFPlayer.playMp3Folder(status_now[i]+1);
				delay(3000);
			}
		}
	}
}

void nRF_message_parsing() {
	status_before[0]	= status_now[0];
	status_before[1]	= status_now[1];
	status_before[2]	= status_now[2];
	status_before[3]	= status_now[3];

	status_now[0]	= nRF_read_buff & 3;
	status_now[1]	= nRF_read_buff & 12;
	status_now[2]	= nRF_read_buff & 48;
	status_now[3]	= nRF_read_buff & 192;

	status_now[1]	>>= 2;
	status_now[2]	>>= 4;
	status_now[3]	>>= 6;

	for (int i = 0 ; i < 4 ; i++) {
		if (status_before[i] == status_now[i])	status_flag[i] = false;
		else 									status_flag[i] = true;
	}
}

void nRF_message_print() {
	Serial.println("start");
	Serial.print("ss : ");	Serial.println(nRF_read_buff);
	Serial.print("rr : ");	Serial.println(status_now[0]);
	Serial.print("ll : ");	Serial.println(status_now[1]);
	Serial.print("u1 : ");	Serial.println(status_now[2]);
	Serial.print("u2 : ");	Serial.println(status_now[3]);
	Serial.print("rtf : ");	Serial.println(status_flag[0]);
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