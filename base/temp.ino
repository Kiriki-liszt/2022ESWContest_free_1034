#include <Wire.h>

// 자신의 슬레이브 주소를 설정해 줍니다.(슬레이브 주소에 맞게 수정해야 합니다.)
#define SLAVE 4  

// 카운터
byte count = 0;
char temp;

void setup() {
	// Wire 라이브러리 초기화
	// 슬레이브로 참여하기 위해서는 주소를 지정해야 한다.
	Wire.begin(SLAVE);
	Wire.onReceive(receiveFromMaster);
	// 마스터의 데이터 전송 요구가 있을 때 처리할 함수 등록
	Wire.onRequest(sendToMaster);
	pinMode(13, OUTPUT);
	Serial.begin(9600);
}

void loop () {
	// 요청이 들어오면 13번 LED를 0.5초간 켜줍니다.
	if (temp == 'o') {
		play();
	}
}

void receiveFromMaster(int bytes) {
	char ch[2];
	for (int i = 0 ; i < bytes ; i++) {
		// 수신 버퍼 읽기
		ch[i] = Wire.read();
	}
	temp = ch[0];
}

void play() {
	digitalWrite(13, HIGH);
	delay(500);
	digitalWrite(13, LOW);
	temp = 0;
}

void sendToMaster() {
	// 자신의 슬레이브 주소를 담은 메세지를 마스터에게 보냅니다. 슬레이브 주소에 맞게 수정해야 합니다.
	Wire.write("4th Arduino ");
}



///////////////////////////////////////////////////////////////////////////////////////////


#include <Wire.h>

byte Daten[58];
float array_attiny[29] = {39.3459, 0, 27.85, 16215, 1, 33, 36, 192, 5116, 10734, 15000, 80, 55, 19718, 'ltg7l09450043', 0, 'battery', 'Li-Ion', 49, 225, 84, 0, 0, 10, 255, 0b11000000, 4000, 12450, 255};
int wert = 0;
byte Register;

void setup() {

  Wire.begin(0x0B);                // join i2c bus with address #B
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  Serial.println("Los gehts");

  berechnen();
}

void loop() {
  //delay(300);
  //  Serial.println("Event");
  //  Serial.println(Register, HEX);

}

void berechnen() {

  //für Adresse 09 - Spannung
  wert = round((array_attiny[0] * 1000) / 3.3);
  Daten[0] = wert & 0xFF;
  Daten[1] = (wert >> 8) & 0xFF;

  //für Adresse 10 - Gesamtkapazität
  wert = round(array_attiny[9] / 2);
  Daten[2] = wert & 0xFF;
  Daten[3] = (wert >> 8) & 0xFF;

  //für Adresse 16 - Batteriestatus
  wert = array_attiny[7];
  Daten[4] = wert & 0xFF;
  Daten[5] = (wert >> 8) & 0xFF;

  //für Adresse 0F - verbleibende Kapazität
  wert = round(array_attiny[8] / 2);
  Daten[6] = wert & 0xFF;
  Daten[7] = (wert >> 8) & 0xFF;

  //verbleibende Ladung in Prozent (Restkap)
  wert = array_attiny[11];
  Daten[8] = wert & 0xFF;
  Daten[9] = (wert >> 8) & 0xFF;

  //für Adresse 0A - Strom
  wert = round(array_attiny[1] / 2);
  Daten[10] = wert & 0xFF;
  Daten[11] = (wert >> 8) & 0xFF;

  //für Adresse 08 - Temperatur
  wert = round((array_attiny[2] + 273.15) * 10);
  Daten[12] = wert & 0xFF;
  Daten[13] = (wert >> 8) & 0xFF;

  //für Adresse 1B - Herstelldatum
  wert = array_attiny[3];
  Daten[14] = wert & 0xFF;
  Daten[15] = (wert >> 8) & 0xFF;

  //für Adresse 1C - Seriennummer
  wert = array_attiny[4];
  Daten[16] = wert & 0xFF;
  Daten[17] = (wert >> 8) & 0xFF;

  //für Adresse 17 - Zykluszahl
  wert = array_attiny[5];
  Daten[18] = wert & 0xFF;
  Daten[19] = (wert >> 8) & 0xFF;

  //für Adresse 19 - Werksspannung
  wert = array_attiny[6];
  Daten[20] = wert & 0xFF;
  Daten[21] = (wert >> 8) & 0xFF;

  //für Adresse 0E - absolute Ladung
  wert = array_attiny[12];
  Daten[22] = wert & 0xFF;
  Daten[23] = (wert >> 8) & 0xFF;

  //für Adresse 20 - Herstellername
  wert = array_attiny[13];
  Daten[24] = wert & 0xFF;
  Daten[25] = (wert >> 8) & 0xFF;

  //für Adresse 18 - Werkskapazität
  wert = array_attiny[10];
  Daten[26] = wert & 0xFF;
  Daten[27] = (wert >> 8) & 0xFF;

  //für Adresse 23 - Herstellerdaten
  wert = array_attiny[14];
  Daten[28] = wert & 0xFF;
  Daten[29] = (wert >> 8) & 0xFF;

  //für Adresse 25 - Herstellerinfo
  wert = array_attiny[15];
  Daten[30] = wert & 0xFF;
  Daten[31] = (wert >> 8) & 0xFF;

  //für Adresse 21 - Gerätetyp
  wert = array_attiny[16];
  Daten[32] = wert & 0xFF;
  Daten[33] = (wert >> 8) & 0xFF;

  //für Adresse 22 - Chemie
  wert = array_attiny[17];
  Daten[34] = wert & 0xFF;
  Daten[35] = (wert >> 8) & 0xFF;

  //für Adresse 1A - Spezifikationsinformation
  wert = array_attiny[18];
  Daten[36] = wert & 0xFF;
  Daten[37] = (wert >> 8) & 0xFF;

  //für Adresse 1A - Zeit bis vollgeladen
  wert = array_attiny[19];
  Daten[38] = wert & 0xFF;
  Daten[39] = (wert >> 8) & 0xFF;

  //für Adresse 3F - Spannung Zelle 1
  wert = round(array_attiny[20]);
  Daten[40] = wert & 0xFF;
  Daten[41] = (wert >> 8) & 0xFF;

  //für Adresse 3E - Spannung Zelle 2
  wert = round(array_attiny[21]);
  Daten[42] = wert & 0xFF;
  Daten[43] = (wert >> 8) & 0xFF;

  //für Adresse 3D - Spannung Zelle 3
  wert = round(array_attiny[22]);
  Daten[44] = wert & 0xFF;
  Daten[45] = (wert >> 8) & 0xFF;

  //für Adresse 3C - Spannung Zelle 4
  wert = round(array_attiny[23]);
  Daten[46] = wert & 0xFF;
  Daten[47] = (wert >> 8) & 0xFF;

  //für Adresse 4F - state of health
  wert = array_attiny[24];
  Daten[48] = wert & 0xFF;
  Daten[49] = (wert >> 8) & 0xFF;

  //für Adresse 03 - Batteriemodus
  wert = array_attiny[25];
  Daten[50] = wert & 0xFF;
  Daten[51] = (wert >> 8) & 0xFF;

  //für Adresse 14 - Ladestrom
  wert = round(array_attiny[26]);
  Daten[52] = wert & 0xFF;
  Daten[53] = (wert >> 8) & 0xFF;

  //für Adresse 15 - Ladespannung
  wert = round(array_attiny[27]);
  Daten[54] = wert & 0xFF;
  Daten[55] = (wert >> 8) & 0xFF;

  //für Adresse 2A - Stromregister
  wert = round(array_attiny[28]);
  Daten[56] = wert & 0xFF;
  Daten[57] = (wert >> 8) & 0xFF;
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {

  Register;
  //Spannung
  if (Register == 0x09) {
    Wire.write(Daten[0]);
    Wire.write(Daten[1]);
  }

  //Gesamtkapazität
  if (Register == 0x10) {
    Wire.write(Daten[2]);
    Wire.write(Daten[3]);
  }

  //Batteriestatus
  if (Register == 0x16) {
    Wire.write(Daten[4]);
    Wire.write(Daten[5]);
  }

  //verbleibende Kapazität
  if (Register == 0x0F) {
    Wire.write(Daten[6]);
    Wire.write(Daten[7]);
  }

  //verbleibende Ladung in Prozent (Restkap)
  if (Register == 0x0D) {
    Wire.write(Daten[8]);
    Wire.write(Daten[9]);
  }

  //Strom
  if (Register == 0x0A) {
    Wire.write(Daten[10]);
    Wire.write(Daten[11]);
  }

  //Temperatur
  if (Register == 0x08) {
    Wire.write(Daten[12]);
    Wire.write(Daten[13]);
  }

  //Herstelldatum
  if (Register == 0x1B) {
    Wire.write(Daten[14]);
    Wire.write(Daten[15]);
  }

  //Seriennummer
  if (Register == 0x1C) {
    Wire.write(Daten[16]);
    Wire.write(Daten[17]);
  }

  //Zykluszahl
  if (Register == 0x17) {
    Wire.write(Daten[18]);
    Wire.write(Daten[19]);
  }

  //Werksspannung
  if (Register == 0x19) {
    Wire.write(Daten[20]);
    Wire.write(Daten[21]);
  }

  //absolute Ladung
  if (Register == 0x0E) {
    Wire.write(Daten[22]);
    Wire.write(Daten[23]);
  }

  //Herstellername
  if (Register == 0x20) {
    Wire.write(Daten[24]);
    Wire.write(Daten[25]);
  }

  //Werkskapazität
  if (Register == 0x18) {
    Wire.write(Daten[26]);
    Wire.write(Daten[27]);
  }

  //Herstellerdaten
  if (Register == 0x23) {
    Wire.write(Daten[28]);
    Wire.write(Daten[29]);
  }

  //Herstellerinfo
  if (Register == 0x25) {
    Wire.write(Daten[30]);
    Wire.write(Daten[31]);
  }

  //Gerätetyp
  if (Register == 0x21) {
    Wire.write(Daten[32]);
    Wire.write(Daten[33]);
  }

  //Chemie
  if (Register == 0x22) {
    Wire.write(Daten[34]);
    Wire.write(Daten[35]);
  }

  //Spezifikationsinformation
  if (Register == 0x1A) {
    Wire.write(Daten[36]);
    Wire.write(Daten[37]);
  }

  //Zeit bis vollgeladen
  if (Register == 0x13) {
    Wire.write(Daten[38]);
    Wire.write(Daten[39]);
  }

  //Spannung Zelle 1
  if (Register == 0x3F) {
    Wire.write(Daten[40]);
    Wire.write(Daten[41]);
  }

  //Spannung Zelle 2
  if (Register == 0x3E) {
    Wire.write(Daten[42]);
    Wire.write(Daten[43]);
  }

  //Spannung Zelle 3
  if (Register == 0x3D) {
    Wire.write(Daten[44]);
    Wire.write(Daten[45]);
  }

  //Spannung Zelle 4
  if (Register == 0x3C) {
    Wire.write(Daten[46]);
    Wire.write(Daten[47]);
  }

  //state of health
  if (Register == 0x4F) {
    Wire.write(Daten[48]);
    Wire.write(Daten[49]);
  }

  //Batteriemodus
  if (Register == 0x03) {
    Wire.write(Daten[50]);
    Wire.write(Daten[51]);
  }

  //Ladestrom
  if (Register == 0x14) {
    Wire.write(Daten[52]);
    Wire.write(Daten[53]);
  }

  //Ladespannung
  if (Register == 0x15) {
    Wire.write(Daten[54]);
    Wire.write(Daten[55]);
  }

  //Stromregister
  if (Register == 0x2A) {
    Wire.write(Daten[56]);
    Wire.write(Daten[57]);
  }

}

void receiveEvent(int howMany) {
    Register = Wire.read();
}