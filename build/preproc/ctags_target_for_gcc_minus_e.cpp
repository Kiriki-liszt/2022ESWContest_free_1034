# 1 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_RX_1\\nRF24L01_RX_1.ino"
# 2 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_RX_1\\nRF24L01_RX_1.ino" 2
# 3 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_RX_1\\nRF24L01_RX_1.ino" 2
# 4 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_RX_1\\nRF24L01_RX_1.ino" 2
# 5 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_RX_1\\nRF24L01_RX_1.ino" 2

# 7 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_RX_1\\nRF24L01_RX_1.ino" 2


# 8 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_RX_1\\nRF24L01_RX_1.ino"
// SPI 버스에 nRF24L01 라디오를 설정하기 위해 CE, CSN 선언.


//optionally, reduce the payload size.  seems to improve reliability.


RF24 radio(15, 2);
const byte address[6] = "00001"; //주소값을 5가지 문자열로 변경할 수 있으며, 송신기과 수신기가 동일한 주소로 해야됨.

void setup() {

  Serial.begin(9600);
  Serial.println("Rx Start");


  radio.begin();

  radio.setPayloadSize(8);
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

unsigned char ss;
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


    radio.read(&ss, sizeof(ss));
    Serial.print("ss : ");
    Serial.println(ss);

    right_turn = ss & 3;
    Serial.print("rr : ");
    Serial.println(right_turn);

    left_turn = ss & 12;
    left_turn >>= 2;
    Serial.print("ll : ");
    Serial.println(left_turn);

    unused_1 = ss & 48;
    unused_1 >>= 4;
    Serial.print("u1 : ");
    Serial.println(unused_1);

    unused_2 = ss & 192;
    unused_2 >>= 6;
    Serial.print("u2 : ");
    Serial.println(unused_2);

    Serial.println("end\n");

  }
}