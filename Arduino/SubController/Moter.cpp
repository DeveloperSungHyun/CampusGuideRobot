#include "Arduino.h"
#include "Moter.h"

int _moter_L_1, _moter_R_1, _moter_L_2, _moter_R_2;
void Moter::Init(int moter_L_1, int moter_R_1, int moter_L_2, int moter_R_2) {
_moter_L_1 = moter_L_1;
_moter_R_1 = moter_R_1;
_moter_L_2 = moter_L_2;
_moter_R_2 = moter_R_2;
}

void Moter::Moter_1(bool direction, int speed) {
  if (speed >= 256) speed = 256;
  else if (speed <= 0) speed = 0;

  if (direction) {
    analogWrite(_moter_L_1, speed);
    delay(1);
    analogWrite(_moter_L_2, 0);
  } else {
    analogWrite(_moter_L_1, 0);
    delay(1);
    analogWrite(_moter_L_2, speed);
  }
}

void Moter::Moter_2(bool direction, int speed) {
  if (speed >= 256) speed = 256;
  else if (speed <= 0) speed = 0;

  if (direction) {
    analogWrite(_moter_R_1, speed);
    delay(1);
    analogWrite(_moter_R_2, 0);
  } else {
    analogWrite(_moter_R_1, 0);
    delay(1);
    analogWrite(_moter_R_2, speed);
  }
}