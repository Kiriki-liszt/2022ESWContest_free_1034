#include <Wire.h>

#define SLAVE_nano_1	1
#define SLAVE_nano_2	2
#define SLAVE_nano_3	3
#define SLAVE_nano_4	4
#define SLAVE_NUM		4
int SLAVE_nano[4] = {SLAVE_nano_1, SLAVE_nano_2, SLAVE_nano_3, SLAVE_nano_3};		// 슬레이브 주소

#define I2C_RxTx_byte	16
char I2C_RxTx_Data[I2C_RxTx_byte];

void I2C_Tx (int salves);
void I2C_Req(int slaves);

void setup() {
	Wire.setSDA(PB9);				// nucleo-64 F103RB alt pins for I2C
	Wire.setSCL(PB8);
	Wire.begin();					// Wire 라이브러리 초기화
	Serial.begin(9600);				// 직렬 통신 초기화
}


void loop() {
	byte nano_addr;
	for (nano_addr = 0; nano_addr < SLAVE_NUM; nano_addr++) {
		I2C_Tx(nano_addr);
		delay(10);
		// 슬레이브로 데이터 요구 및 수신 데이터 처리
		I2C_Req(nano_addr);
		delay(10);
	}
	delay(1000);
}

void I2C_Tx (int salves) {
	Wire.beginTransmission(SLAVE_nano[slaves]);				// I2C 통신을 통한 전송 시작
	// 슬레이브 주소를 시리얼 창에 입력시 해당 시리얼 주소로 'o'문자와 데이터 요구를 보냅니다.
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

/* Example pinmap for Bluepill I2Cs (by Testato)

 I2C-1 standard pins: PB7(sda) PB6(scl)
 Use it by "Wire" without pin declaration
	Wire.begin();

 I2C-1 alternative pins: PB9(sda) PB8(scl)
 Remap the first I2C before call begin()
	Wire.setSDA(PB9);
	Wire.setSCL(PB8);
	Wire.begin();

 I2C-2: PB11(sda) PB10(scl)
 Remap the second I2C before call begin()
	Wire.setSDA(PB11);
	Wire.setSCL(PB10);
	Wire.begin();

 If you want to use the two I2Cs simultaneously, create a new instance for the second I2C
	TwoWire Wire2(PB11,PB10);
	Wire2.begin();
 
*/