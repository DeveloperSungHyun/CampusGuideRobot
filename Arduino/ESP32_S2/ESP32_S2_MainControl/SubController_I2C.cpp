#include <Wire.h>
#include "Arduino.h"
#include "SubController_I2C.h"

#define ADDRESS 1

void SubController_I2C::Init(int I2C_SDA, int I2C_SCL) {
  Wire.begin(I2C_SDA, I2C_SCL);                           // SDA와 SCL 핀을 설정하여 I2C 초기화
}
int SubController_I2C::Get_Battery() {
  Wire.requestFrom(ADDRESS, 1);                           // 슬레이브에게 2바이트 요청
  return Wire.read();                                     //반환된 BatteryLevel 데이터를 리턴
}