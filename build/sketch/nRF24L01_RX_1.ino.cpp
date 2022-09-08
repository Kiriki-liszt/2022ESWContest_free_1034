#include <Arduino.h>
#line 1 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_RX_1\\nRF24L01_RX_1.ino"
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <printf.h>

#include <string.h>

// SPI 버스에 nRF24L01 라디오를 설정하기 위해 CE, CSN 선언.
// STM Tx
#define Tx_CE 10
#define Tx_SCN 8
// nodeMCU Rx
#define Rx_CE 15
#define Rx_SCN 2

//optionally, reduce the payload size.  seems to improve reliability.
#define Payload_size 8

//주소값을 5가지 문자열로 변경할 수 있으며, 송신기와 수신기가 동일한 주소로 해야됨.
const byte address[6] = "00001";



RF24 radio(Rx_CE, Rx_SCN); 

#line 26 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_RX_1\\nRF24L01_RX_1.ino"
void setup();
#line 54 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_RX_1\\nRF24L01_RX_1.ino"
void loop();
#line 79 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_RX_1\\nRF24L01_RX_1.ino"
void message_parsing();
#line 26 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_RX_1\\nRF24L01_RX_1.ino"
void setup() {

  Serial.begin(9600);
  Serial.println("Rx Start");


  radio.begin();

  radio.setPayloadSize(Payload_size);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH);
  // 전원공급에 관한 파워레벨을 설정합니다. 모듈 사이가 가까우면 최소로 설정합니다.
  // 거리가 가까운 순으로 RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH 등으로 설정할 수 있습니다.  RF24_PA_MAX는 레거시
  // 높은 레벨(거리가 먼 경우)은 작동하는 동안 안정적인 전압을 가지도록 GND와 3.3V에 바이패스 커패시터 사용을 권장함.
  // radio.enableDynamicPayloads();
  // radio.enableDynamicAck();
  radio.setAutoAck(false);
  // radio.powerUp() ;  
  radio.startListening(); //모듈을 수신기로 설정
}

unsigned char read_buff;
unsigned char right_turn;
unsigned char left_turn;
unsigned char unused_1;
unsigned char unused_2;


void loop() {
  if (radio.available()) {
    // char readBuffer[Payload_size] = "";
    // radio.read(&readBuffer, sizeof(readBuffer));
    // Serial.println(readBuffer);
    // char text[32] = "";
    // radio.read(&text, sizeof(text));
    // Serial.println(text);

    Serial.println("start");
    Serial.print("rb : ");
    Serial.println(read_buff);
    Serial.print("rr : ");
    Serial.println(right_turn);
    Serial.print("ll : ");
    Serial.println(left_turn);
    Serial.print("u1 : ");
    Serial.println(unused_1);
    Serial.print("u2 : ");
    Serial.println(unused_2);
    Serial.println("end\n");

  }
}

void message_parsing() {
  radio.read(&read_buff, sizeof(read_buff));

  right_turn  = read_buff & 3;
  left_turn   = read_buff & 12;
  unused_1    = read_buff & 48;
  unused_2    = read_buff & 192;

  left_turn >>= 2;
  unused_1  >>= 4;
  unused_2  >>= 6;

}
