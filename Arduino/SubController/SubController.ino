#include <Wire.h>
#include <RPLidar.h>
#include <MechaQMC5883.h>

MechaQMC5883 qmc;

#define M_PI 3.1415926535


#define RPLIDAR_MOTOR 3

RPLidar lidar;

const float distanceThreshold = 2000;
const float frontAngleMin = 325;
const float frontAngleMax = 35;
const float leftAngleMin = 290;
const float leftAngleMax = 325;
const float rightAngleMin = 35;
const float rightAngleMax = 70;

String dataString = "";  // 전송할 데이터를 저장할 문자열 변수


class LowPassFilter {
public:
  LowPassFilter(float alpha)
    : alpha(alpha), filteredValue(0) {}

  float filter(float newValue) {
    filteredValue = alpha * newValue + (1 - alpha) * filteredValue;
    return filteredValue;
  }

private:
  float alpha;          // 필터 계수 (0과 1 사이의 값)
  float filteredValue;  // 이전 필터 값을 저장
};

LowPassFilter filter1(0.05);  // 필터 객체 생성, alpha 값은 0.1로 설정
LowPassFilter filter2(0.05);  // 필터 객체 생성, alpha 값은 0.1로 설정
LowPassFilter filter3(0.05);  // 필터 객체 생성, alpha 값은 0.1로 설정


void setup() {
  Wire.begin();  // I2C 통신을 마스터로 초기화
  lidar.begin(Serial1);
  Serial.begin(115200);
  pinMode(RPLIDAR_MOTOR, OUTPUT);
  startMotor();
  lidar.startScan();
}

void loop() {
  dataString = "";  // 데이터 문자열 초기화

  if (IS_OK(lidar.waitPoint())) {
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;

    if (distance > 0 && distance < distanceThreshold) {
      if (isInLeftRange(angle)) {
        dataString += "L:";
        dataString += String((int)filter1.filter(distance));
        dataString += ",";
        //Serial.println(dataString);
      }

      if (isInFrontRange(angle)) {
        dataString += "F:";
        dataString += String((int)filter2.filter(distance));
        dataString += ",";
        Serial.println(dataString);
      } else {
        dataString += "F:";
        dataString += String((int)0);
        dataString += ",";
        Serial.println(dataString);
      }

      if (isInRightRange(angle)) {
        dataString += "R:";
        dataString += String((int)filter3.filter(distance));
        dataString += ",";
        //Serial.println(dataString);
      }
    }
  } else {
    stopMotor();
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
      lidar.startScan();
      startMotor();
      delay(1000);
    }
  }

  // 데이터 문자열을 I2C 통신으로 전송
  Wire.beginTransmission(8);       // 장치 주소 8로 전송
  Wire.write(dataString.c_str());  // 문자열을 전송
  Wire.endTransmission();

  //delay(100); // 필요에 따라 지연 시간 조절
}

bool isInFrontRange(float angle) {
  return (angle >= frontAngleMin || angle <= frontAngleMax);
}

bool isInLeftRange(float angle) {
  return (angle >= leftAngleMin && angle <= leftAngleMax);
}

bool isInRightRange(float angle) {
  return (angle >= rightAngleMin && angle <= rightAngleMax);
}

void startMotor() {
  analogWrite(RPLIDAR_MOTOR, 255);
}

void stopMotor() {
  analogWrite(RPLIDAR_MOTOR, 0);
}