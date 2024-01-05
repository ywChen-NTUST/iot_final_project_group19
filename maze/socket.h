#ifndef MAZE_SOCKET
#define MAZE_SOCKET

#include <Arduino.h>

class Socket
{
public:
    Socket(const String& ssid, const String& password);
    void connect(const String& IP, int port, bool verbose = false);
    short send(const String& data, char *response, bool verbose = false);
    void disconnect();
    ~Socket();

private:
    String _ssid;
    String _password;
    String _IP;
    int _port;
};

#endif