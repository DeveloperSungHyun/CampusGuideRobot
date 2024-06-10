#include <Wire.h>
#include <math.h>
#include "Arduino.h"
#include "GpsData.h"
#include "Moter.h"
#include "BatteryLevel.h"

#define M_PI 3.14159265358979323846

GpsData gps_data;
Moter moter;
BatteryLevel battery_level;

double point_x;  //목적지 위치
double point_y;  //목적지 위치

//GPS센서 통신핀
#define GPS_SDA 12  // GPIO 9번을 SDA로 설정
#define GPS_SCL 13  // GPIO 8번을 SCL로 설정

//왼쪽 모터
#define MOTER_R_A 6
#define MOTER_R_B 7

//오른쪽 모터
#define MOTER_L_A 4
#define MOTER_L_B 5

#define BATTERY_LEVEL_PIN 27

int LiDAR_distance[3];

int speed = 70;

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

long a;
int c = 0;

void setup() {

  Wire.begin(8);                 // I2C 통신 Slave로 시작, 주소는 8
  Wire.onReceive(receiveEvent);  // 수신 이벤트 등록

  gps_data.Init(GPS_SDA, GPS_SCL);  //GPS 데이터 수신 준비
  Serial.begin(115200);             //시리얼 출력 준비
  moter.Init(MOTER_R_A, MOTER_R_B, MOTER_L_A, MOTER_L_B);
  battery_level.Init(BATTERY_LEVEL_PIN);
  analogReadResolution(12);
  //Wire.begin(25, 33);  // SDA, SCL 핀 지정(현재는 사용 X)

  RunTime_Gps = millis();  //현재 RunTime 저장

  L_moter_speed_timer = millis();
  R_moter_speed_timer = millis();

  a = millis();
}

void loop() {
  Serial.print(LiDAR_distance[0]);
  Serial.print(", ");
  Serial.print(LiDAR_distance[1]);
  Serial.print(", ");
  Serial.println(LiDAR_distance[2]);

  delay(10);

  //double bearing = calculateBearing(point_x, point_y, lat2, lon2);//이동각도



  battery_level.UpDate();

  Moter_move();

  if (millis() - a >= 2000) {
    a = millis();
    c++;
  }

  if(c == 0){
    Ste_Moter_1_speed = 500;
    Ste_Moter_2_speed = 500;
  }
    if(c == 1){
    Ste_Moter_1_speed = 500;
    Ste_Moter_2_speed = 500;
  }
    if(c == 2){
    Ste_Moter_1_speed = 500;
    Ste_Moter_2_speed = 300;
  }
    if(c == 3){
    Ste_Moter_1_speed = 300;
    Ste_Moter_2_speed = 500;
    
  }
  if(c == 4){
    Ste_Moter_1_speed = 300;
    Ste_Moter_2_speed = 500;
    c = 0;
  }




  // if ((LiDAR_distance[1] <= 700 && LiDAR_distance[1]) || LiDAR_distance[1] != 0) {
  //   if (LiDAR_distance[0] > LiDAR_distance[2]) {
  //     Ste_Moter_2_speed = (int)(map(speed, 0, 100, 0, 1024) / map(LiDAR_distance[1], 600, 1500, 1000, 10));
  //     Ste_Moter_1_speed = (int)(map(speed, 0, 100, 0, 1024));
  //     Serial.println(1);
  //   } else {
  //     Ste_Moter_1_speed = (int)(map(speed, 0, 100, 0, 1024) / map(LiDAR_distance[2], 600, 1500, 1000, 10));
  //     Ste_Moter_2_speed = (int)(map(speed, 0, 100, 0, 1024));
  //     Serial.println(2);
  //   }
  // }else{
  //   Ste_Moter_2_speed = (int)(map(speed, 0, 100, 0, 1024));
  //   Ste_Moter_1_speed = (int)(map(speed, 0, 100, 0, 1024));
  // }

  // if (LiDAR_distance[1] > LiDAR_distance[0] && LiDAR_distance[1] > LiDAR_distance[2]) {  //정면이 가장 가깝다면
  //   if (LiDAR_distance[1] <= 1500) {
  //     if (LiDAR_distance[0] > LiDAR_distance[2]) {
  //       Ste_Moter_1_speed = (int)(map(speed, 0, 100, 0, 1024) / map(LiDAR_distance[1], 600, 1500, 100, 1));
  //       Ste_Moter_2_speed = (int)(map(speed, 0, 100, 0, 1024));
  //     } else {
  //       Ste_Moter_2_speed = (int)(map(speed, 0, 100, 0, 1024) / map(LiDAR_distance[2], 600, 1500, 100, 1));
  //       Ste_Moter_1_speed = (int)(map(speed, 0, 100, 0, 1024));
  //     }
  //   }
  // } else {
  //   if (LiDAR_distance[0] > LiDAR_distance[2]) {
  //     Ste_Moter_1_speed = (int)(map(speed, 0, 100, 0, 1024) / map(LiDAR_distance[2], 600, 1500, 100, 1));
  //       Ste_Moter_2_speed = (int)(map(speed, 0, 100, 0, 1024));
  //   } else {
  //     Ste_Moter_2_speed = (int)(map(speed, 0, 100, 0, 1024) / map(LiDAR_distance[1], 600, 1500, 100, 1));
  //       Ste_Moter_1_speed = (int)(map(speed, 0, 100, 0, 1024));
  //   }
  // }


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

    if (label == "L") {
      LiDAR_distance[0] = value;
    } else if (label == "F") {
      LiDAR_distance[1] = value;
    } else if (label == "R") {
      LiDAR_distance[2] = value;
    }
  }
}

void Moter_move() {
  if (L_moter_speed <= Ste_Moter_1_speed) {
    if (millis() - L_moter_speed_timer >= 0) {
      L_moter_speed_timer = millis();
      L_moter_speed++;
      moter.Moter_1(MoterRotation_1, L_moter_speed);
    }
  } else {
    L_moter_speed = Ste_Moter_1_speed;
    moter.Moter_1(MoterRotation_1, L_moter_speed);
  }

  if (R_moter_speed <= Ste_Moter_2_speed) {
    if (millis() - R_moter_speed_timer >= 0) {
      R_moter_speed_timer = millis();
      R_moter_speed++;
      moter.Moter_2(MoterRotation_2, R_moter_speed);
    }
  } else {
    R_moter_speed = Ste_Moter_2_speed;
    moter.Moter_2(MoterRotation_2, R_moter_speed);
  }
}


double degToRad(double deg) {
  return deg * M_PI / 180.0;
}
double calculateBearing(double lat1, double lon1, double lat2, double lon2) {  //이동경로 각도계산
  double phi1 = degToRad(lat1);
  double phi2 = degToRad(lat2);
  double deltaLambda = degToRad(lon2 - lon1);

  double y = sin(deltaLambda) * cos(phi2);
  double x = cos(phi1) * sin(phi2) - sin(phi1) * cos(phi2) * cos(deltaLambda);
  double theta = atan2(y, x);

  // 라디안 단위를 도 단위로 변환
  double bearing = theta * 180.0 / M_PI;

  // 양수 값으로 변환
  bearing = fmod((bearing + 360.0), 360.0);

  return bearing;
}