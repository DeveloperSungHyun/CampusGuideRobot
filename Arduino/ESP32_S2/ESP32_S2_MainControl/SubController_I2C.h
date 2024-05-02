class SubController_I2C {  //서브 아두이노(atmega328p)와 통신기능을 구현
public:
  void Init(int I2C_SDA, int I2C_SCL);  //서브 아두이노와 I2C방식의 시리얼 통신을 하기위한 초기설정 함수
  int Get_Battery();                    //서브 아두이노가 계산한 배터리 용량을 %단위로 반환하는 함수
};