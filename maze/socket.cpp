#include <Arduino.h>
#include <WiFi.h>
#include "socket.h"

Socket::Socket(const String& ssid, const String& password)
    : _ssid(ssid), _password(password)
{
}

void Socket::connect(const String& IP, int port, bool verbose)
{
    _IP = IP;
    _port = port;
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(); //clear previous connection
    delay(100);

    WiFi.begin(_ssid.c_str(), _password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        if (verbose) {
            Serial.print(".");
        }
    }
    if (verbose)
    {
        Serial.println("Connected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }
}

short Socket::send(const String& data, char *response, bool verbose)
{
    WiFiClient client;
    if (!client.connect(_IP.c_str(), _port)) {
        Serial.println("Connection failed");
        return -1;
    }
    if(verbose)
    {
        Serial.print("Sending data: ");
        for(int i = 0; i < data.length(); i++)
        {
            Serial.printf("%x ", data[i]);
        }
        Serial.println();
    }
    client.print(data);
    client.readBytes(response, 1024);
    if(verbose)
    {
        Serial.print("Recieved data: ");
        for(int i = 0; i < strlen(response); i++)
        {
            Serial.printf("%x ", response[i]);
        }
        Serial.println();
    }
    client.stop();
    return strlen(response);
}

void Socket::disconnect()
{
    WiFi.disconnect();
}

Socket::~Socket()
{
    WiFi.disconnect();
}