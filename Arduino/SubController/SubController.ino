#include <Wire.h>
#include <MechaQMC5883.h>

MechaQMC5883 qmc;

int trigPins[4] = { 39, 43, 47, 51 };
int echoPins[4] = { 41, 45, 49, 53 };

int distances[4];
float previousAzimuth;

String out_data;

void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);  // Serial1은 ESP32와의 통신용
  Wire.begin();           //마스터

  qmc.init();
  qmc.setMode(Mode_Continuous, ODR_200Hz, RNG_8G, OSR_512);

  for (int i = 0; i < 4; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }
}

void loop() {
  Azimuth();
  distance();  // 거리 데이터 0 = 오른쪽, 1 = 왼쪽, 2 = 정면_왼쪽, 3 = 정면_오른쪽

  for (int i = 0; i < 4; i++) {
    out_data += String(distances[i]) + ",";
  }
  out_data += String(previousAzimuth) + "\n";
  Serial1.println("test");  // 수신한 데이터를 ESP32에 전송
  out_data = "";

  delay(1000);
}

void distance() {

  long durations[4];

  for (int i = 0; i < 4; i++) {
    digitalWrite(trigPins[i], LOW);
    delayMicroseconds(2);
    digitalWrite(trigPins[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPins[i], LOW);

    durations[i] = pulseIn(echoPins[i], HIGH);
    distances[i] = durations[i] * 0.034 / 2;
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
