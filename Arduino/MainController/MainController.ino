#include <Wire.h>
#include "Arduino.h"
#include "GpsData.h"
#include "Moter.h"
#include "BatteryLevel.h"

GpsData gps_data;
Moter moter;
BatteryLevel battery_level;
//GPS센서 통신핀
#define GPS_SDA 12  // GPIO 9번을 SDA로 설정
#define GPS_SCL 13  // GPIO 8번을 SCL로 설정

//왼쪽 모터
#define MOTER_R_A 15
#define MOTER_R_B 2

//오른쪽 모터
#define MOTER_L_A 4
#define MOTER_L_B 16

#define BATTERY_LEVEL_PIN 27

int LiDAR_distance[3];

double Latitude;   //위도값 저장
double Longitude;  //경도값 저장

int Ste_Moter_1_speed = 0, Ste_Moter_2_speed = 0;  //설정한 모터 속도
bool MoterRotation_1 = true;                       //true = 정회전, false = 역회전
bool MoterRotation_2 = true;                       //true = 회전, false = 역회전

long RunTime_Gps;  //GPS데이터를 주기적으로 가져오는 시간을 계산하기 위한 변수

int GPS_GetTime = 5000;  //ms

int L_moter_speed = 0;  //실제 속도
int R_moter_speed = 0;  //실제 속도

long L_moter_speed_timer;
long R_moter_speed_timer;

void setup() {

  Wire.begin(8);                 // I2C 통신 Slave로 시작, 주소는 8
  Wire.onReceive(receiveEvent);  // 수신 이벤트 등록

  gps_data.Init(GPS_SDA, GPS_SCL);                //GPS 데이터 수신 준비
  Serial.begin(115200);                             //시리얼 출력 준비
  moter.Init(MOTER_R_A, MOTER_R_B, MOTER_L_A, MOTER_L_B);
  battery_level.Init(BATTERY_LEVEL_PIN);
  analogReadResolution(12);
  //Wire.begin(25, 33);  // SDA, SCL 핀 지정(현재는 사용 X)

  RunTime_Gps = millis();  //현재 RunTime 저장

  L_moter_speed_timer = millis();
  R_moter_speed_timer = millis();
}

void loop() {
  Serial.print(LiDAR_distance[0]);
  Serial.print(", ");
  Serial.print(LiDAR_distance[1]);
  Serial.print(", ");
  Serial.println(LiDAR_distance[2]);
  
  if (L_moter_speed <= Ste_Moter_1_speed) {
    if (millis() - L_moter_speed_timer >= 20) {
      L_moter_speed_timer = millis();
      L_moter_speed++;
      moter.Moter_1(MoterRotation_1, L_moter_speed);
    }
  } else {
    L_moter_speed = Ste_Moter_1_speed;
    moter.Moter_1(MoterRotation_1, L_moter_speed);
  }

  if (R_moter_speed <= Ste_Moter_2_speed) {
    if (millis() - R_moter_speed_timer >= 20) {
      R_moter_speed_timer = millis();
      R_moter_speed++;
      moter.Moter_2(MoterRotation_2, R_moter_speed);
    }
  } else {
    R_moter_speed = Ste_Moter_2_speed;
    moter.Moter_2(MoterRotation_2, R_moter_speed);
  }

  battery_level.UpDate();

  //   if (millis() - RunTime_Gps >= GPS_GetTime) {  //GPS_GetTime 변수 값 시간동안 주기적으로 실행
  //     gps_data.UpDate();
  //     Latitude = gps_data.GetLatitude();    //위도값 가져오기
  //     Longitude = gps_data.GetLongitude();  //경도값 가져오기

  //     //출력 테스트
  //     Serial.print(Latitude, 8);
  //     Serial.print(", ");
  //     Serial.println(Longitude, 8);

  //     RunTime_Gps = millis();
  //   }

  //delay(100);
}

void receiveEvent(int bytes) {  //인터럽트로 라이다센서 데이터 받
  String receivedData = "";
  while (Wire.available()) {
    char c = Wire.read();
    receivedData += c;
  }

  // 받은 데이터를 파싱하여 출력
  int index = receivedData.indexOf(':');
  if (index != -1) {
    String label = receivedData.substring(0, index);
    String valueStr = receivedData.substring(index + 1);
    int value = valueStr.toInt();  // 문자열을 정수로 변환
    
    if(label == "L"){
      LiDAR_distance[0] = value;
    }else if(label == "F"){
      LiDAR_distance[1] = value;
    }else if(label == "R"){
      LiDAR_distance[2] = value;
    }
  }
}