#include "Arduino.h"
#include "Moter.h"

void Moter::Init(int moter_L_1, int moter_R_1, int moter_L_2, int moter_R_2) {

  ledcSetup(0, 5000, 10);
  ledcSetup(1, 5000, 10);

  ledcSetup(2, 5000, 10);
  ledcSetup(3, 5000, 10);

  // PWM 핀 할당
  ledcAttachPin(moter_L_1, 0);
  ledcAttachPin(moter_R_1, 1);

  ledcAttachPin(moter_L_2, 2);
  ledcAttachPin(moter_R_2, 3);
}

void Moter::Moter_1(bool direction, int speed) {
  if (speed >= 1024) speed = 1024;
  else if (speed <= 0) speed = 0;

  if (direction) {
    ledcWrite(0, speed);
    delay(1);
    ledcWrite(1, 0);
  } else {
    ledcWrite(0, 0);
    delay(1);
    ledcWrite(1, speed);
  }
}

void Moter::Moter_2(bool direction, int speed) {
  if (speed >= 1024) speed = 1024;
  else if (speed <= 0) speed = 0;

  if (direction) {
    ledcWrite(2, speed);
    delay(1);
    ledcWrite(3, 0);
  } else {
    ledcWrite(2, 0);
    delay(1);
    ledcWrite(3, speed);
  }
}