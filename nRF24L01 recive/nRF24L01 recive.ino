#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <printf.h>

// SPI 버스에 nRF24L01 라디오를 설정하기 위해 CE, CSN 선언.
#define Rx_CE 15
#define Rx_SCN 2
//optionally, reduce the payload size.  seems to improve reliability.
#define Payload_size 8

RF24 radio(Rx_CE, Rx_SCN); 
const byte address[6] = "00001"; //주소값을 5가지 문자열로 변경할 수 있으며, 송신기과 수신기가 동일한 주소로 해야됨.

void setup() {

  Serial.begin(9600);
  Serial.println("Rx Start");


  radio.begin();

  radio.setPayloadSize(Payload_size);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH);
  //전원공급에 관한 파워레벨을 설정합니다. 모듈 사이가 가까우면 최소로 설정합니다.
  //거리가 가까운 순으로 RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH 등으로 설정할 수 있습니다.  RF24_PA_MAX는 레거시
  //높은 레벨(거리가 먼 경우)은 작동하는 동안 안정적인 전압을 가지도록 GND와 3.3V에 바이패스 커패시터 사용을 권장함.
  // radio.enableDynamicPayloads();
  // radio.enableDynamicAck();
  radio.setAutoAck(false);
  // radio.powerUp() ;  
  radio.startListening(); //모듈을 수신기로 설정
}

void loop() {
  if (radio.available()) {
    char readBuffer[Payload_size] = "";
    radio.read(&readBuffer, sizeof(readBuffer));
    Serial.println(readBuffer);
    // char text[32] = "";
    // radio.read(&text, sizeof(text));
    // Serial.println(text);
  }
}

