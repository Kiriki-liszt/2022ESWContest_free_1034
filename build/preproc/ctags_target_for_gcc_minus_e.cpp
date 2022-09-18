# 1 "/home/yoon/Documents/Arduino Project/Embedded_software_Contest_2022/nRF24L01_TX/nRF24L01_TX.ino"
# 2 "/home/yoon/Documents/Arduino Project/Embedded_software_Contest_2022/nRF24L01_TX/nRF24L01_TX.ino" 2
# 3 "/home/yoon/Documents/Arduino Project/Embedded_software_Contest_2022/nRF24L01_TX/nRF24L01_TX.ino" 2
# 4 "/home/yoon/Documents/Arduino Project/Embedded_software_Contest_2022/nRF24L01_TX/nRF24L01_TX.ino" 2
# 5 "/home/yoon/Documents/Arduino Project/Embedded_software_Contest_2022/nRF24L01_TX/nRF24L01_TX.ino" 2

// SPI 버스에 nRF24L01 라디오를 설정하기 위해 CE, CSN 선언.
// STM Tx


// nodeMCU Rx


// Timer



//optionally, reduce the payload size.  seems to improve reliability.


//주소값을 5가지 문자열로 변경할 수 있으며, 송신기와 수신기가 동일한 주소로 해야됨.
const byte address[6] = "00001";



RF24 radio(10, 8);

void setup() {

 Serial2.begin(9600);
 Serial2.println("Tx Start");

 radio.begin();

 radio.setPayloadSize(8); //optionally, reduce the payload size.  seems to improve reliability.
 radio.openWritingPipe(address); //이전에 설정한 5글자 문자열인 데이터를 보낼 수신의 주소를 설정
 // if( radio.setDataRate( RF24_250KBPS ) ) {
 //   printf( "Data rate 250KBPS set!\n\r" );
 // } else {
 //   printf( "Data rate 250KBPS set FAILED!!\n\r" );
 // }
 // radio.setDataRate( RF24_2MBPS );
 radio.setPALevel(RF24_PA_HIGH);
 // 전원공급에 관한 파워레벨을 설정합니다. 모듈 사이가 가까우면 최소로 설정합니다.
 // 거리가 가까운 순으로 RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH 등으로 설정할 수 있습니다.  RF24_PA_MAX는 레거시.
 // 높은 레벨(거리가 먼 경우)은 작동하는 동안 안정적인 전압을 가지도록 GND와 3.3V에 바이패스 커패시터 사용을 권장함.
 // radio.enableDynamicPayloads();
 radio.enableDynamicAck();
 // radio.setRetries(5, 0);
 // radio.setAutoAck(false);
 // radio.powerUp() ;
 radio.stopListening(); //모듈을 송신기로 설정
}

// char sendBuffer[Payload_size];
// uint8_t counter = 1;
// 상태는 총 4개 : 00, 01, 10, 11
// 상황은 앞에서부터 차례대로 우회전, 비보호, 로터리 등등
unsigned char send_buff;
unsigned char right_turn;
unsigned char left_turn;
unsigned char unused_1;
unsigned char unused_2;

unsigned char car_flag;

unsigned long timer_set = 0;

bool rslt;
unsigned char cast = 0; // 0: single ; 1: broad


void loop() {
 delay(1000); // Every 1sec
 // sprintf(sendBuffer, "00011011");
 // sprintf(sendBuffer, "%d|root", counter);
 // const char text[] = "Hi";
 // radio.write(&text, sizeof(text));  //해당 메시지를 수신자에게 보냄
 // radio.write( sendBuffer, sizeof(sendBuffer) );
 // counter++;
 // delay(1000); // Every 1sec

 // messege write
 right_turn = 1;
 left_turn = 2;
 unused_1 = 0;
 unused_2 = 3;

 make_message();

 Serial2.println("start");
 if ( cast == 0 ) { // singlecast
  rslt = radio.write( &send_buff, sizeof(send_buff), false );
  if (rslt) {
   car_flag = 1; // 차 있음
   timer_set = millis(); // 타이머 시작
   cast = 1; // broadcast
  } else if ((millis() - timer_set) > 20000) {
   car_flag = 0;
  }
 } else { // broadcast
  radio.write( &send_buff, sizeof(send_buff), true );
  if ((millis() - timer_set) > 10000) {
   cast = 0; // to singlecast
  }
 }

 prnt_message();

 Serial2.println("end\n");


 // default : singlecast
 radio.setAutoAck(true);

}

void make_message() {
 left_turn <<= 2;
 unused_1 <<= 4;
 unused_2 <<= 6;

 send_buff = right_turn;
 send_buff |= left_turn;
 send_buff |= unused_1;
 send_buff |= unused_2;
}

void prnt_message() {
 Serial2.print("ss : "); Serial2.println(send_buff);
 Serial2.print("rr : "); Serial2.println(right_turn);
 Serial2.print("ll : "); Serial2.println(left_turn);
 Serial2.print("u1 : "); Serial2.println(unused_1);
 Serial2.print("u2 : "); Serial2.println(unused_2);
 Serial2.print("car_flag : "); Serial2.println(car_flag);
 Serial2.print("cast : "); Serial2.println(cast);
 Serial2.print("rslt : "); Serial2.println(rslt);
 Serial2.print("timer_set : "); Serial2.println(timer_set);
 Serial2.print("time : "); Serial2.println(millis());
}
