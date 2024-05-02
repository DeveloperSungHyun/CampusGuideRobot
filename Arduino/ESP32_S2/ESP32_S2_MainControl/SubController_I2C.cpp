#include <Wire.h>     //I2C 통신을 쉽게 할 수 있도록 해주는 라이브러리
#include "Arduino.h"  //기본 아두이노에서 제공하는 함수들을 사용할 수 있게 해주는 라이브러리
#include "SubController_I2C.h"

#define ADDRESS 1  //서브 아두이노(atmega328p)와 I2C 통신을 할때 사용할 주소

void SubController_I2C::Init(int I2C_SDA, int I2C_SCL) {
  Wire.begin(I2C_SDA, I2C_SCL);  // SDA와 SCL 핀을 설정하고 I2C 초기화
}
int SubController_I2C::Get_Battery() {
  Wire.requestFrom(ADDRESS, 2);  // 슬레이브에게 2바이트 요청
  return Wire.read();            //반환된 BatteryLevel 데이터를 리턴
}