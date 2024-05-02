#include <Wire.h>
#include "SubController_I2C.h"
#include "GpsData.h"

SubController_I2C sub_controller_I2C;
GpsData gps_data;

//=======================Atmega328p 통신 핀
#define I2C_SDA 9  // GPIO 9번을 SDA로 설정
#define I2C_SCL 8  // GPIO 8번을 SCL로 설정

#define Interrupt_pin 15  //인터럽트 입력핀

int BatteryLevel;
double Latitude;
double Longitude;

int test;
long RunTime_Battery, RunTime_Gps;

void setup() {
  sub_controller_I2C.Init(I2C_SDA, I2C_SCL);  //Atmega328p 와 I2C통신 준비
  gps_data.Init(18, 17);                      //GPS 데이터 수신 준비

  Serial.begin(9600);  // 시리얼 통신 시작

  RunTime_Battery = RunTime_Gps = millis();
}

void loop() {

  if (millis() - RunTime_Battery >= 1000 * 60) {
    BatteryLevel = sub_controller_I2C.Get_Battery();

    //출력 테스트
    Serial.println(BatteryLevel);
    RunTime_Battery = millis();
  }
  if (millis() - RunTime_Gps >= 1000 * 1) {
    gps_data.UpDate();
    Latitude = gps_data.GetLatitude();
    Longitude = gps_data.GetLongitude();

    //출력 테스트
    Serial.print(Latitude, 8);
    Serial.print(", ");
    Serial.println(Longitude, 8);

    RunTime_Gps = millis();
  }
}
