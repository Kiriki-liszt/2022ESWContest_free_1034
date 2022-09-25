# 1 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\Station\\Station.ino"
/***********************************/
/*          Nucleo F103RB          */
/***********************************/

# 6 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\Station\\Station.ino" 2
# 7 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\Station\\Station.ino" 2
# 8 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\Station\\Station.ino" 2
# 9 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\Station\\Station.ino" 2
# 10 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\Station\\Station.ino" 2

// SPI 버스에 nRF24L01 라디오를 설정하기 위해 CE, CSN 선언.
// STM Tx



// Timer



//optionally, reduce the payload size.  seems to improve reliability.


//주소값을 5가지 문자열로 변경할 수 있으며, 송신기와 수신기가 동일한 주소로 해야됨.
const byte address[6] = "00001";






int SLAVE_nano[4] = {1, 2, 3, 4}; // 슬레이브 주소


char I2C_RxTx_Data[16];
int LiDAR_rad_dist[4][2];
String LiDAR_String[4][2];

///////////////////////////////////////////// Tx 시작

void nRF_make_signal();
void nRF_make_message();
void nRF_prnt_message();

void I2C_Tx (int salves);
void I2C_Req(int slaves);

RF24 radio(10, 8);

void setup() {
 Serial2.begin(9600);
 Serial2.println("Tx Start");

 Wire.setSDA(14); // nucleo-64 F103RB alt pins for I2C
 Wire.setSCL(15);
 Wire.begin(); // Wire 라이브러리 초기화, join as controller

 radio.begin();
 radio.setPayloadSize(8); //optionally, reduce the payload size.  seems to improve reliability.
 radio.openWritingPipe(address); //이전에 설정한 5글자 문자열인 데이터를 보낼 수신의 주소를 설정
 radio.setPALevel(RF24_PA_HIGH);
 // 전원공급에 관한 파워레벨을 설정합니다. 모듈 사이가 가까우면 최소로 설정합니다.
 // 거리가 가까운 순으로 RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH 등으로 설정할 수 있습니다.  RF24_PA_MAX는 레거시.
 // 높은 레벨(거리가 먼 경우)은 작동하는 동안 안정적인 전압을 가지도록 GND와 3.3V에 바이패스 커패시터 사용을 권장함.
 radio.enableDynamicAck();
 radio.stopListening(); //모듈을 송신기로 설정
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
unsigned char cast = 0; // 0: single ; 1: broad


void loop() {
 delay(1000); // Every 1sec

 // I2C 
 byte nano_addr;
 for (nano_addr = 0; nano_addr < 4; nano_addr++) {
  // I2C_Tx(nano_addr); delay(10);
  // 슬레이브로 데이터 요구 및 수신 데이터 처리
  I2C_Req(nano_addr);
  delay(10);
 }

 // RF
 nRF_make_signal();
 nRF_make_message();

 if ( cast == 0 ) { // singlecast
  rslt = radio.write( &nRF_send_buff, sizeof(nRF_send_buff), false );
  if (rslt) {
   car_flag = 1; // 차 있음
   timer_set = millis(); // 타이머 시작
   cast = 1; // to broadcast
  } else if ((millis() - timer_set) > 20000) {
   car_flag = 0;
  }
 } else { // broadcast
  radio.write( &nRF_send_buff, sizeof(nRF_send_buff), true );
  if ((millis() - timer_set) > 10000) {
   cast = 0; // to singlecast
  }
 }


 nRF_prnt_message();

 // default : singlecast
 // radio.setAutoAck(true);
}

void I2C_Tx (int slaves) {
 Wire.beginTransmission(SLAVE_nano[slaves]); // I2C 통신을 통한 전송 시작
 Wire.write("Data Send");
 Wire.endTransmission(SLAVE_nano[slaves]);
}

void I2C_Req(int slaves) {
 Wire.requestFrom(SLAVE_nano[slaves], 2/*바이트*/); // 인수로 넘겨받은 곳(I2C slave)으로 2바이트 데이터 요청
 while(Wire.available()) {
  LiDAR_rad_dist[slaves][0] = Wire.read(); // rad
  LiDAR_rad_dist[slaves][1] = Wire.read(); // distance
 }
 LiDAR_String[4][0] = String(LiDAR_rad_dist[slaves][0]); // 각도랑 거리를 string으로
 LiDAR_String[4][1] = String(LiDAR_rad_dist[slaves][1]);
 Serial2.print("rad  string : "); Serial2.println(LiDAR_String[4][0]);
 Serial2.print("dist string : "); Serial2.println(LiDAR_String[4][1]);
}

// 상태는 총 4개 : 00, 01, 10, 11
// 상황은 앞에서부터 차례대로 우회전, 비보호, 로터리 등등
void nRF_make_signal() {
 right_turn = 1;
 left_turn = 2;
 unused_1 = 0;
 unused_2 = 3;
}

void nRF_make_message() {
 left_turn <<= 2;
 unused_1 <<= 4;
 unused_2 <<= 6;

 nRF_send_buff = right_turn;
 nRF_send_buff |= left_turn;
 nRF_send_buff |= unused_1;
 nRF_send_buff |= unused_2;
}

void nRF_prnt_message() {
 Serial2.print("ss : "); Serial2.println(nRF_send_buff);
 Serial2.print("rr : "); Serial2.println(right_turn);
 Serial2.print("ll : "); Serial2.println(left_turn);
 Serial2.print("u1 : "); Serial2.println(unused_1);
 Serial2.print("u2 : "); Serial2.println(unused_2);
 Serial2.print("car_flag : "); Serial2.println(car_flag);
 Serial2.print("cast : "); Serial2.println(cast);
 Serial2.print("rslt : "); Serial2.println(rslt);
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



void I2C_Req(int slaves) {

	Wire.requestFrom(SLAVE_nano[slaves], I2C_RxTx_byte);		// n 바이트 크기의 데이터 요청

	

	for (int i = 0 ; i < I2C_RxTx_byte ; i++) {	

		I2C_RxTx_Data[i] = NULL; 

	}

	while(Wire.available()) {

		I2C_RxTx_Data[i] = Wire.read();							// 수신 데이터 읽기

	}

	

	Serial.println(I2C_RxTx_Data);

}

*/
