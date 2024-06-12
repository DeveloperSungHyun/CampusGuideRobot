#include "ESP32Client.h"

ESP32Client::ESP32Client(const char* ssid, const char* password, const char* serverIP)
    : ssid(ssid), password(password), serverIP(serverIP) {}

void ESP32Client::begin() {
    WiFi.begin(ssid, password);
    Serial.print("WiFi 연결 중");

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi 연결 완료");
    Serial.print("IP 주소: ");
    Serial.println(WiFi.localIP());
}

void ESP32Client::sendMessage(const char* message) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(String("http://") + serverIP + "/send");

        http.addHeader("Content-Type", "text/plain");
        int httpResponseCode = http.POST(message);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("서버 응답: " + response);
        } else {
            Serial.println("서버에 연결 실패");
        }

        http.end();
    }
}

void ESP32Client::sendInteger(int value) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(String("http://") + serverIP + "/send_integer");

        http.addHeader("Content-Type", "text/plain");
        int httpResponseCode = http.POST(String(value));

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("서버 응답: " + response);
        } else {
            Serial.println("서버에 연결 실패");
        }

        http.end();
    }
}

void ESP32Client::sendFloat(float value) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(String("http://") + serverIP + "/send_float");

        http.addHeader("Content-Type", "text/plain");
        int httpResponseCode = http.POST(String(value));

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("서버 응답: " + response);
        } else {
            Serial.println("서버에 연결 실패");
        }

        http.end();
    }
}

void ESP32Client::receiveMessage(void (*callback)(String)) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(String("http://") + serverIP + "/get_message");

        int httpCode = http.GET();
        if (httpCode > 0) {
            if (httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println("서버로부터 받은 문자열: " + payload);
                if (callback) {
                    callback(payload);
                }
            }
        } else {
            Serial.println("서버에 연결 실패");
        }

        http.end();
    }
}

void ESP32Client::receiveInteger(void (*callback)(int)) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(String("http://") + serverIP + "/get_integer");

        int httpCode = http.GET();
        if (httpCode > 0) {
            if (httpCode == HTTP_CODE_OK) {
                int payload = http.getString().toInt();
                Serial.println("서버로부터 받은 정수: " + String(payload));
                if (callback) {
                    callback(payload);
                }
            }
        } else {
            Serial.println("서버에 연결 실패");
        }

        http.end();
    }
}

void ESP32Client::receiveFloat(void (*callback)(float)) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(String("http://") + serverIP + "/get_float");

        int httpCode = http.GET();
        if (httpCode > 0) {
            if (httpCode == HTTP_CODE_OK) {
                float payload = http.getString().toFloat();
                Serial.println("서버로부터 받은 실수: " + String(payload));
                if (callback) {
                    callback(payload);
                }
            }
        } else {
            Serial.println("서버에 연결 실패");
        }

        http.end();
    }
}

