#include <Wire.h>
#include <math.h>
#include "Arduino.h"
#include "GpsData.h"
#include "Moter.h"
#include "BatteryLevel.h"
#include <MechaQMC5883.h>

MechaQMC5883 qmc;

GpsData gps_data;
Moter moter;
BatteryLevel battery_level;


//GPS센서 통신핀
#define GPS_SDA 34  // GPIO 9번을 SDA로 설정
#define GPS_SCL 35  // GPIO 8번을 SCL로 설정

//왼쪽 모터
#define MOTER_R_A 10
#define MOTER_R_B 11
//오른쪽 모터
#define MOTER_L_A 12
#define MOTER_L_B 13

#define BATTERY_LEVEL_PIN 27

int setting_speed = 700;

int Ste_Moter_1_speed = 0, Ste_Moter_2_speed = 0;  //설정한 모터 속도
bool MoterRotation_1 = true;                       //true = 정회전, false = 역회전
bool MoterRotation_2 = true;                       //true = 회전, false = 역회전

int L_moter_speed = 0;  //실제 속도
int R_moter_speed = 0;  //실제 속도

int GPS_GetTime = 5000;  //ms

long RunTime_Gps;  //GPS데이터를 주기적으로 가져오는 시간을 계산하기 위한 변수
long L_moter_speed_timer;
long R_moter_speed_timer;

int trigPins[4] = { 15, 6, 4, 2 };
int echoPins[4] = { 14, 5, 3, 1 };

int distances[4];
float previousAzimuth;

String in_data;

void setup() {
  Wire.begin();          //마스터
  Serial.begin(115200);  //시리얼 출력 준비

  moter.Init(MOTER_R_A, MOTER_R_B, MOTER_L_A, MOTER_L_B);
  battery_level.Init(BATTERY_LEVEL_PIN);
  gps_data.Init(GPS_SDA, GPS_SCL);

  analogReadResolution(12);

  qmc.init();
  qmc.setMode(Mode_Continuous, ODR_200Hz, RNG_8G, OSR_512);

  for (int i = 0; i < 4; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }

  RunTime_Gps = millis();  //현재 RunTime 저장
  L_moter_speed_timer = millis();
  R_moter_speed_timer = millis();
}

void loop() {


  //Moter_move();
  gps_data.UpDate();

  Azimuth();
  distance();  // 거리 데이터 0 = 오른쪽, 1 = 왼쪽, 2 = 정면_왼쪽, 3 = 정면_오른쪽

  // Serial.print(distances[0]);
  // Serial.print(" , ");
  // Serial.print(distances[1]);
  Serial.print(gps_data.GetLatitude(), 5);
  Serial.print(" , ");
  Serial.println(gps_data.GetLongitude(), 5);
  // Serial.print(" , ");
  Serial.println(previousAzimuth);

  if (distances[2] < 50 || distances[3] < 50) {
    if ((distances[2] > distances[3])) {
      //Serial.println("L");
      if (setting_speed - 300 < 0) Ste_Moter_1_speed = 0;
      else Ste_Moter_1_speed = setting_speed - 300;
      Ste_Moter_2_speed = setting_speed;
      Moter_move();
    } else if ((distances[2] < distances[3])) {
      //Serial.println("R");
      if (setting_speed - 300 < 0) Ste_Moter_2_speed = 0;
      else Ste_Moter_2_speed = setting_speed - 300;
      Ste_Moter_1_speed = setting_speed;
      Moter_move();
    }
  } else if (distances[0] < 20 || distances[1] < 20) {
    if (distances[0] < distances[1]) {
      if (setting_speed - 100 < 0) Ste_Moter_1_speed = 0;
      else Ste_Moter_1_speed = setting_speed - 100;
      Ste_Moter_2_speed = setting_speed;
    } else {
      if (setting_speed - 100 < 0) Ste_Moter_2_speed = 0;
      else Ste_Moter_2_speed = setting_speed - 100;
      Ste_Moter_1_speed = setting_speed;
    }
    Moter_move();
  } else {
    Ste_Moter_1_speed = setting_speed;
    Ste_Moter_2_speed = setting_speed;
    Moter_move();
  }
}


void Moter_move() {
  if (L_moter_speed <= Ste_Moter_1_speed) {
    if (millis() - L_moter_speed_timer >= 7) {
      L_moter_speed += (int)(millis() - L_moter_speed_timer) / 7;
      moter.Moter_1(MoterRotation_1, L_moter_speed);
      L_moter_speed_timer = millis();
    }
  } else {
    L_moter_speed = Ste_Moter_1_speed;
    moter.Moter_1(MoterRotation_1, L_moter_speed);
  }

  if (R_moter_speed <= Ste_Moter_2_speed) {
    if (millis() - R_moter_speed_timer >= 7) {
      R_moter_speed += (int)(millis() - R_moter_speed_timer) / 7;
      moter.Moter_2(MoterRotation_2, R_moter_speed);
      R_moter_speed_timer = millis();
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




void distance() {

  long duration;

  // 초음파 펄스를 보내기 위해 Trig 핀을 LOW로 설정
  for (int i = 0; i < 4; i++) {
    digitalWrite(trigPins[i], LOW);
    delayMicroseconds(2);

    // Trig 핀을 HIGH로 설정한 후, 10마이크로초 후 다시 LOW로 설정
    digitalWrite(trigPins[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPins[i], LOW);

    // Echo 핀의 펄스 지속 시간을 측정
    duration = pulseIn(echoPins[i], HIGH);

    // 거리 계산 (소리의 속도를 이용, 거리 = 시간 * 속도 / 2)
    distances[i] = duration * 0.034 / 2;
  }
}

void Azimuth() {
  int x, y, z;
  qmc.read(&x, &y, &z);

  // 디클리네이션 보정 (서울 기준 약 -7.5도)
  float declinationAngle = -8.23;
  float azimuth = atan2((float)y, (float)x) * 180.0 / PI + declinationAngle;

  // 방위 값이 0-360도 범위를 벗어날 경우 보정
  if (azimuth < 0) {
    azimuth += 360.0;
  } else if (azimuth >= 360.0) {
    azimuth -= 360.0;
  }

  // 노이즈 제거
  static const float alpha = 0.3;  // 노이즈 필터링 강도 (0.0 - 1.0)
  float filteredAzimuth = alpha * azimuth + (1 - alpha) * previousAzimuth;
  previousAzimuth = filteredAzimuth;
}