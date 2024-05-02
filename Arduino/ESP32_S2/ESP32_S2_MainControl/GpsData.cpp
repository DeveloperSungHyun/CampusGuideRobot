#include "Arduino.h"
#include "GpsData.h"
#include <HardwareSerial.h>

static double Latitude_Data = 0;
static double Longitude_Data = 0;
//
HardwareSerial Serial2(1);  // UART2를 위한 객체 생성

void GpsData::Init(int RX, int TX) {
  Serial2.begin(9600, SERIAL_8N1, RX, TX);  // RX, TX 순으로 핀 번호 지정
}
void GpsData::UpDate() {  //GPS데이터 갱신
  if (Serial2.available() > 0) {
    String data = Serial2.readStringUntil('\n');

    if (data.startsWith("$GPGGA,")) {
      extractLatLong(data);
    }
  }
}
double GpsData::GetLatitude() {  //위도 좌표 반환함수
  return Latitude_Data;
}
double GpsData::GetLongitude() {  //경도 좌표 반환함수
  return Longitude_Data;
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
/*
GPS데이터를 가공해야 하는 이유
GPS데이터를 가져오게 되면 다양한 데이터들이 압축되어 들어오게 되어 원하는 정보를 얻기 휘애서는 필요한 부분을 추출하여 가공해야 한다.

$GPRMC,061939.00,A,3629.24620,N,12724.66289,E,0.072,,010524,,,A*7E
$GPVTG,,T,,M,0.072,N,0.134,K,A*20
$GPGGA,061939.00,3629.24620,N,12724.66289,E,1,08,1.12,55.8,M,20.0,M,,*65
$GPGSA,A,3,19,17,06,22,03,09,14,20,,,,,1.92,1.12,1.56*0D
$GPGSV,3,1,12,02,01,081,,03,28,042,27,04,26,086,11,06,56,306,25*71
$GPGSV,3,2,12,09,26,132,40,11,24,274,08,12,10,321,,14,21,185,29*7E
$GPGSV,3,3,12,17,70,104,18,19,75,348,23,20,10,225,31,22,40,205,28*7E
$GPGLL,3629.24620,N,12724.66289,E,061939.00,A,A*60

위도 경도 데이터 같은 경우 
$GPGGA,061939.00,3629.24620,N,12724.66289,E,1,08,1.12,55.8,M,20.0,M,,*65 라인에서3629.24620 와 12724.66289 를 추출해서
36 29.24620 자리를 분리하고 36 + 29.24620 / 60 을 해야 위도좌표를 얻을 수 있고 12724.66289 는 127 24.66289 를 분리하여 127 + 24.66289 / 60 을 계산해야
경도 좌표를 구할 수 있다.
*/