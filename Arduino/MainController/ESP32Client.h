#ifndef ESP32CLIENT_H
#define ESP32CLIENT_H

#include <WiFi.h>
#include <HTTPClient.h>

class ESP32Client {
public:
    ESP32Client(const char* ssid, const char* password, const char* serverIP);

    void begin();
    void sendMessage(const char* message);
    void sendInteger(int value);
    void sendFloat(float value);
    void receiveMessage(void (*callback)(String));
    void receiveInteger(void (*callback)(int));
    void receiveFloat(void (*callback)(float));

private:
    const char* ssid;
    const char* password;
    const char* serverIP;
};

#endif // ESP32CLIENT_H
