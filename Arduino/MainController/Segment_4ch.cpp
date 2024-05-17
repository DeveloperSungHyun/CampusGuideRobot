#include "Arduino.h"
#include "Segment_4ch.h"

int _data, _latch, _clock;
static int Segment_Ch = 0;

void Segment_4ch::Init(int _dataPin, int _latchPin, int _clockPin) {  // 시프트레지스터 출력 설정
  _data = _dataPin;
  _latch = _latchPin;
  _clock = _clockPin;

  pinMode(_data, OUTPUT);
  pinMode(_latch, OUTPUT);
  pinMode(_clock, OUTPUT);
}

void Segment_4ch::print(int battery_level) {  //시프트레지스터 와 7세그먼트 디코더 처리함수

  digitalWrite(_latch, LOW);
  shiftOut(_data, _clock, MSBFIRST, 64 + (battery_level % 10000) / 1000);
  digitalWrite(_latch, HIGH);

  delay(2);

  digitalWrite(_latch, LOW);
  shiftOut(_data, _clock, MSBFIRST, 16 + (battery_level % 1000) / 100);
  digitalWrite(_latch, HIGH);

  delay(2);

  digitalWrite(_latch, LOW);
  shiftOut(_data, _clock, MSBFIRST, 128 + (battery_level % 100) / 10);
  digitalWrite(_latch, HIGH);

  delay(2);

  digitalWrite(_latch, LOW);
  shiftOut(_data, _clock, MSBFIRST, 32 + (battery_level % 10) / 1);
  digitalWrite(_latch, HIGH);

  delay(2);

  if (Segment_Ch < 4) Segment_Ch++;
  else Segment_Ch = 0;
}
