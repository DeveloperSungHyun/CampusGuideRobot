#include "Arduino.h"
#include "GpsData.h"
#include <HardwareSerial.h>

static double decimalDegrees_a = 0;
static double decimalDegrees_b = 0;

HardwareSerial Serial2(1);                    // UART2를 위한 객체 생성

void GpsData::Init(int RX, int TX) {
  Serial2.begin(9600, SERIAL_8N1, RX, TX);   // RX, TX 순으로 핀 번호 지정
}
void GpsData::UpDate() {                     //GPS데이터 갱신
  if (Serial2.available() > 0) {
    String data = Serial2.readStringUntil('\n');

    if (data.startsWith("$GPGGA,")) {
      extractLatLong(data);
    }
  }
}
double GpsData::GetLatitude() {              //위도 좌표 반환함수
  return decimalDegrees_a;
}
double GpsData::GetLongitude() {             //경도 좌표 반환함수
  return decimalDegrees_b;
}

void GpsData::extractLatLong(String data) {  //gps모듈에서 받은 데이터를 가공해서 위도, 경도만 추출
  int firstComma = data.indexOf(',');
  int secondComma = data.indexOf(',', firstComma + 1);
  int thirdComma = data.indexOf(',', secondComma + 1);
  int fourthComma = data.indexOf(',', thirdComma + 1);
  int fifthComma = data.indexOf(',', fourthComma + 1);

  double latitude = data.substring(secondComma + 1, thirdComma).toDouble();
  double longitude = data.substring(fourthComma + 1, fifthComma).toDouble();

  int degrees_a = (int)(latitude / 100);
  double minutes_a = latitude - (degrees_a * 100);
  decimalDegrees_a = degrees_a + (minutes_a / 60);

  int degrees_b = (int)(longitude / 100);
  double minutes_b = longitude - (degrees_b * 100);
  decimalDegrees_b = degrees_b + (minutes_b / 60);
}