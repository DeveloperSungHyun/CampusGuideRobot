#include "Arduino.h"
#include "Segment_4ch.h"

int data, latch, clock;
static int Segment_Ch = 0;

void Segment_4ch::Init(int dataPin, int latchPin, int clockPin) { // 시프트레지스터 출력 설정
  data = dataPin;
  latch = latchPin;
  clock = clockPin;

  pinMode(data, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);
}

void Segment_4ch::print(int battery_level) {                    //시프트레지스터 와 7세그먼트 디코더 처리함수

  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, 0b10000001);
  digitalWrite(latch, HIGH);

  delay(2);

  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, 0b01000010);
  digitalWrite(latch, HIGH);

  delay(2);

  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, 0b00100011);
  digitalWrite(latch, HIGH);

  delay(2);

  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, 0b00010100);
  digitalWrite(latch, HIGH);

  delay(2);

  if (Segment_Ch < 4) Segment_Ch++;
  else Segment_Ch = 0;
}