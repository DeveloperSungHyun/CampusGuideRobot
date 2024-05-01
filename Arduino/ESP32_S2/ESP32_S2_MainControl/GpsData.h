class GpsData{
  private:
  void extractLatLong(String data);
  public:
  void Init(int RX, int TX);
  void UpDate();
  double GetLatitude();
  double GetLongitude();
};