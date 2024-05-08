#include <Wire.h>
#include "Arduino.h"
#include "Segment_4ch.h"
#include "GpsData.h"
#include "Moter.h"

Segment_4ch segment_4ch;
GpsData gps_data;
Moter moter;
//GPS센서 통신핀
#define GPS_SDA 26  // GPIO 9번을 SDA로 설정
#define GPS_SCL 27  // GPIO 8번을 SCL로 설정

//시프트레지스터를 통해 7세그먼트 제어핀
#define dataPin 12   // DS Pin on 74HC595
#define latchPin 13  // ST_CP Pin on 74HC595
#define clockPin 14  // SH_CP Pin on 74HC595

//왼쪽 모터
#define MOTER_R_A 2
#define MOTER_R_B 4

//오른쪽 모터
#define MOTER_L_A 16
#define MOTER_L_B 17

double Latitude;   //위도값 저장
double Longitude;  //경도값 저장

int Moter_1_speed, Moter_2_speed;  //모터 속도
bool MoterRotation_1 = true;       //true = 정회전, false = 역회전
bool MoterRotation_2 = true;       //true = 회전, false = 역회전

long RunTime_Gps;  //GPS데이터를 주기적으로 가져오는 시간을 계산하기 위한 변수

int GPS_GetTime = 5000;  //ms

void setup() {
  segment_4ch.Init(dataPin, latchPin, clockPin);  //7세그먼트 출력 준비
  gps_data.Init(GPS_SDA, GPS_SCL);                //GPS 데이터 수신 준비
  Serial.begin(9600);                             //시리얼 출력 준비
  moter.Init(MOTER_R_A, MOTER_R_B, MOTER_L_A, MOTER_L_B);

  //Wire.begin(25, 33);  // SDA, SCL 핀 지정(현재는 사용 X)

  RunTime_Gps = millis();  //현재 RunTime 저장
}

void loop() {
  moter.Moter_1(MoterRotation_1, Moter_1_speed);  //모터1 제어
  moter.Moter_2(MoterRotation_2, Moter_2_speed);  //모터2 제어

  if (millis() - RunTime_Gps >= GPS_GetTime) {  //GPS_GetTime 변수 값 시간동안 주기적으로 실행
    gps_data.UpDate();
    Latitude = gps_data.GetLatitude();    //위도값 가져오기
    Longitude = gps_data.GetLongitude();  //경도값 가져오기

    //출력 테스트
    Serial.print(Latitude, 8);
    Serial.print(", ");
    Serial.println(Longitude, 8);

    RunTime_Gps = millis();
  }

  segment_4ch.print(16);  //7세그먼트에 배터리 전압 출력 전송 테스트값16.24
  //delay(100);
}