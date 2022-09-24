#include <Arduino.h>
#line 1 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_I2C_Tx\\nRF24L01_I2C_Tx.ino"
#include <Wire.h>

int SLAVE[4] = {1, 2, 3, 4};		// 슬레이브 주소

#line 5 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_I2C_Tx\\nRF24L01_I2C_Tx.ino"
void setup();
#line 13 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_I2C_Tx\\nRF24L01_I2C_Tx.ino"
void loop();
#line 32 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_I2C_Tx\\nRF24L01_I2C_Tx.ino"
void i2c_communication(int i);
#line 5 "c:\\Users\\yoon\\Documents\\Arduino\\Projects\\Embedded_software_Contest_2022\\nRF24L01_I2C_Tx\\nRF24L01_I2C_Tx.ino"
void setup() {
	Wire.setSDA(PB9);
	Wire.setSCL(PB8);
	Wire.begin();					// Wire 라이브러리 초기화
	Serial.begin(9600);				// 직렬 통신 초기화
}


void loop() {
	if (Serial.available()) {
		char e = Serial.read();
		byte c = e - 48;
		if (c < 5) {
			Wire.beginTransmission(SLAVE[c-1]);				// I2C 통신을 통한 전송 시작
			// 슬레이브 주소를 시리얼 창에 입력시 해당 시리얼 주소로 'o'문자와 데이터 요구를 보냅니다.
			Wire.write('o');
			Wire.write('\n');
			Wire.endTransmission(SLAVE[c-1]);

			delay(10);
			// 슬레이브로 데이터 요구 및 수신 데이터 처리
			i2c_communication(c-1);
			delay(10);
		}
	}
}

void i2c_communication(int i) {
	Wire.requestFrom(SLAVE[i], 12);		// 12 바이트 크기의 데이터 요청
	for (int j = 0 ; j < 12 ; j++) {	// 12 바이트 모두 출력할 때까지 반복
		char c = Wire.read();			// 수신 데이터 읽기
		Serial.print(c);				// 수신 데이터 출력
	}
	Serial.println();
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
