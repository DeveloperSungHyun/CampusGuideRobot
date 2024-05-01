#include "Arduino.h"
#include "Segment_4ch.h"

int data, latch, clock;
static Segment_Ch = 0;

void Segment_4ch::Init(int dataPin, int latchPin, int clockPin) {
  data = dataPin;
  latch = latchPin;
  clock = clockPin;

  pinMode(data, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);
}

void Segment_4ch::print(int battery_level) {
  
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, 0b10000001);  // 데이터를 시프트 아웃
  digitalWrite(latch, HIGH);

  delay(2);

  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, 0b01000010);  // 데이터를 시프트 아웃
  digitalWrite(latch, HIGH);

  delay(2);

  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, 0b00100011);  // 데이터를 시프트 아웃
  digitalWrite(latch, HIGH);

  delay(2);

  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, 0b00010100);  // 데이터를 시프트 아웃
  digitalWrite(latch, HIGH);

  delay(2);

  if(Segment_Ch < 4)Segment_Ch++;
  else Segment_Ch = 0;
}