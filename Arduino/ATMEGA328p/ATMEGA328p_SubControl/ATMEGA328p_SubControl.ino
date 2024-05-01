#include <Wire.h>
#include "Segment_4ch.h"

Segment_4ch segment_4ch;

int dataPin = 8;   // DS Pin on 74HC595
int latchPin = 12; // ST_CP Pin on 74HC595
int clockPin = 11; // SH_CP Pin on 74HC595

void setup() {
  segment_4ch.Init(dataPin, latchPin, clockPin);

  Wire.begin(1);                // 슬레이브 주소를 1로 시작합니다.
  Wire.onReceive(receiveEvent); // 데이터 수신 이벤트
  Wire.onRequest(requestEvent); // 데이터 요청 이벤트
  Serial.begin(9600);           // 디버깅을 위한 시리얼 통신 시작
}

void loop() {
segment_4ch.print();
  //delay(100);
}

// 데이터를 받는 이벤트 처리
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // 데이터가 있는지 확인
    char c = Wire.read();        // 데이터 읽기
    Serial.print(c);             // 시리얼 모니터에 출력
  }
  int x = Wire.read();           // 데이터 읽기
  Serial.println(x);             // 시리얼 모니터에 출력
}

// 데이터를 요청하는 이벤트 처리
void requestEvent() {
  Wire.write("Helloasddddddddsa ");  // 마스터에게 문자열 "Hello " 전송
}
