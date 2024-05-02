#include <Wire.h>
#include "Segment_4ch.h"

Segment_4ch segment_4ch;

int dataPin = 8;    // DS Pin on 74HC595
int latchPin = 12;  // ST_CP Pin on 74HC595
int clockPin = 11;  // SH_CP Pin on 74HC595


void setup() {
  segment_4ch.Init(dataPin, latchPin, clockPin);

  Wire.begin(1);                 // 슬레이브 주소를 1로 시작합니다.
  Wire.onReceive(receiveEvent);  // 데이터 수신 이벤트
  Wire.onRequest(requestEvent);  // 데이터 요청 이벤트
  Serial.begin(9600);            // 디버깅을 위한 시리얼 통신 시작
}

void loop() {

  segment_4ch.print(16.24);//7세그먼트에 배터리 전압 출력 전송 테스트값16.24
  //delay(100);
}

// 데이터를 받는 이벤트 처리
void receiveEvent(int howMany) {
  Wire.onRequest(requestEvent);  // 데이터 요청 이벤트 처리 requestEvent 함수 호출
}

// 데이터를 요청하는 이벤트 처리
void requestEvent() {
  Wire.write(60);  // 마스터에게 배터리 데이터 전송 테스트값 60
}
