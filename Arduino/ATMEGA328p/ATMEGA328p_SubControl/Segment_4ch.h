class Segment_4ch {
public:
  void Init(int dataPin, int latchPin, int clockPin); // 시프트레지스터 출력 설정
  void print(int battery_level);                      //시프트레지스터 와 7세그먼트 디코더 처리함수

};