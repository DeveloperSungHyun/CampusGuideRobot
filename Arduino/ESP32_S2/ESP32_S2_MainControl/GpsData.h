class GpsData {
private:
  void extractLatLong(String data);  //gps모듈에서 받은 데이터를 가공하는 함수(해당 클래서 내부에서만 사용)
public:
  void Init(int RX, int TX);  //GPS모듈과 I2C통신을 초기설정하는 함수
  void UpDate();              //GPS 데이터 갱신하는 함수
  double GetLatitude();       //위도 데이터 반환
  double GetLongitude();      //경도 데이터 반환
};