#include "Arduino.h"
#include "CurrentAngle.h"

#define PI 3.1415926535

int X_PIN, Y_PIN, Z_PIN;

// 센서 오프셋 값 (이 값을 조정하여 센서의 오프셋을 보정합니다)
const float X_OFFSET = 1.65;
const float Y_OFFSET = 1.65;
const float Z_OFFSET = 1.65;

// 가속도 범위 (이 값을 조정하여 센서의 범위를 보정합니다)
const float ACCEL_RANGE = 0.330;


class LowPassFilter {
public:
    LowPassFilter(float alpha) : alpha(alpha), filteredValue(0) {}

    float filter(float newValue) {
        filteredValue = alpha * newValue + (1 - alpha) * filteredValue;
        return filteredValue;
    }

private:
    float alpha; // 필터 계수 (0과 1 사이의 값)
    float filteredValue; // 이전 필터 값을 저장
};

LowPassFilter filter1(0.1); // 필터 객체 생성, alpha 값은 0.1로 설정
LowPassFilter filter2(0.1); // 필터 객체 생성, alpha 값은 0.1로 설정
LowPassFilter filter3(0.1); // 필터 객체 생성, alpha 값은 0.1로 설정

void CurrentAngle::Init(int _X_PIN, int _Y_PIN, int _Z_PIN) {
  X_PIN = _X_PIN;
  Y_PIN = _Y_PIN;
  Z_PIN = _Z_PIN;
}

float CurrentAngle::Get_Angle() {
  int x = filter1.filter(map(analogRead(X_PIN), 2100, 3200, 0, 1100));
  int y = filter2.filter(map(analogRead(Y_PIN), 2100, 3200, 0, 1100));
  int z = filter3.filter(map(analogRead(Z_PIN), 2100, 3200, 0, 1100));

  // 센서 값에서 전압 계산 (12비트 해상도, 3.3V 참조 전압)
  float voltageX = (x / 1100.0) * 3.3;
  float voltageY = (y / 1100.0) * 3.3;
  float voltageZ = (z / 1100.0) * 3.3;

  // 가속도 계산 (g 단위)
  float accelX = (voltageX - X_OFFSET) / ACCEL_RANGE;
  float accelY = (voltageY - Y_OFFSET) / ACCEL_RANGE;
  float accelZ = (voltageZ - Z_OFFSET) / ACCEL_RANGE;
Serial.print(x);
Serial.print(", ");
Serial.print(y);
Serial.print(", ");
Serial.println(z);
  // 현재 각도 계산 (라디안 단위)
  float angle = atan2(accelY, accelX);

  // 각도를 도 단위로 변환
  return angle * 180 / PI;
}
