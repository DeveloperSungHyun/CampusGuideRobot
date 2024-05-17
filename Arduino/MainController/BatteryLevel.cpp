#include "Arduino.h"
#include "BatteryLevel.h"

int pin;
static float Voltage;
void BatteryLevel::Init(int Level_v) {
  pin = Level_v;
}
void BatteryLevel::UpDate() {
  for (int i = 0; i < 10; i++) {
    Voltage += analogRead(pin) * (float)26 / 4095;
  }
  Voltage = Voltage / 10;
}
float BatteryLevel::Get_Voltage() {
  return Voltage;
}
int BatteryLevel::Get_Level() {
  return 0;
}