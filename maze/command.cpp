#include <Arduino.h>
#include "command.h"
#include "global.h"

Command::Command() {}

void Command::init(const String& ssid, const String& password, const String& ip, int port)
{
    _socket = new Socket(ssid, password);
    _socket->connect(ip, port, true);
    bool success = this->ping();
    if (!success) error(_socket, "Ping failed");
}

bool Command::ping()
{
    char buffer[128];
    char command[128];
    memset(buffer, 0, sizeof(buffer));
    memset(command, 0, sizeof(command));
    sprintf(command, "%c", MAZE_CMD_PING);
    short reslen = _socket->send(String(command), buffer, false);
    if (reslen == -1) error(_socket, "Connection failed");
    return buffer[0] == MAZE_CMD_RESPONSE_OK;
}

bool Command::createUser(u_char& myuid)
{
    char buffer[128];
    char command[128];
    memset(buffer, 0, sizeof(buffer));
    memset(command, 0, sizeof(command));
    sprintf(command, "%c", MAZE_CMD_CREATEUSER);
    short reslen = _socket->send(String(command), buffer, false);
    if (reslen == -1) error(_socket, "Connection failed");
    if (buffer[0] == MAZE_CMD_RESPONSE_OK)
    {
        myuid = buffer[1];
        _token = String(buffer).substring(2, 10);
        return true;
    }
    return false;
}

bool Command::getUserAttribute(int& uid, int& pos_x, int& pos_y, int& life)
{
    char buffer[128];
    char command[128];
    memset(buffer, 0, sizeof(buffer));
    memset(command, 0, sizeof(command));
    sprintf(command, "%c%c", MAZE_CMD_GETUSERATTR, uid);
    short reslen = _socket->send(String(command), buffer, false);
    if (reslen == -1) error(_socket, "Connection failed");
    if (buffer[0] == MAZE_CMD_RESPONSE_OK) {
        pos_x = buffer[1];
        pos_y = buffer[2];
        life = buffer[3];
        return true;
    }
    return false;
}

bool Command::move(int pos_x, int pos_y)
{
    char buffer[128];
    char command[128];
    memset(buffer, 0, sizeof(buffer));
    memset(command, 0, sizeof(command));
    sprintf(command, "%c%s%c%c", MAZE_CMD_SETUSERPOS, _token.c_str(), pos_x, pos_y);
    short reslen = _socket->send(String(command), buffer, false);
    if (reslen == -1) error(_socket, "Connection failed");
    if (buffer[0] == MAZE_CMD_RESPONSE_OK) {
        return true;
    }
    return false;
}

u_char Command::getKeyNum()
{
    char buffer[128];
    char command[128];
    memset(buffer, 0, sizeof(buffer));
    memset(command, 0, sizeof(command));
    sprintf(command, "%c", MAZE_CMD_GETKEYNUM);
    short reslen = _socket->send(String(command), buffer, false);
    if (reslen == -1) error(_socket, "Connection failed");
    // Serial.printf("command.getKeyNum: %d %d\n", buffer[0], buffer[1]);
    if (buffer[0] == MAZE_CMD_RESPONSE_OK) {
        return buffer[1];
    }
    return -1;
}

bool Command::doEntity(maze_entity_type entity, int pos_x, int pos_y)
{
    char buffer[128];
    char command[128];
    memset(buffer, 0, sizeof(buffer));
    memset(command, 0, sizeof(command));
    sprintf(command, "%c%s%c%c%c", MAZE_CMD_DOENTITY, _token.c_str(), entity, pos_x, pos_y);
    short reslen = _socket->send(String(command), buffer, false);
    if (reslen == -1) error(_socket, "Connection failed");
    if (buffer[0] == MAZE_CMD_RESPONSE_OK) {
        return true;
    }
    return false;
}

void Command::syncMaze()
{
    char buffer[128];
    char command[128];
    memset(buffer, 0, sizeof(buffer));
    memset(command, 0, sizeof(command));
    sprintf(command, "%c%s", MAZE_CMD_SYNCMAZE, _token.c_str());
    short reslen = _socket->send(String(command), buffer, false);
    if (reslen == -1) error(_socket, "Connection failed");
    if (buffer[0] == MAZE_CMD_RESPONSE_OK) {
        u_char querylen = buffer[1];
        for(u_char i=0; i<querylen; i++)
        {
            u_char pos_x = buffer[2+i*3];
            u_char pos_y = buffer[3+i*3];
            maze_entity_type entity = (maze_entity_type)buffer[4+i*3];
            maze[pos_y][pos_x] = entity;
        }
    }
}

Command::~Command()
{
    delete _socket;
}

void error(Socket *sock, const String& message)
{
    Serial.println(message);
    sock->disconnect();
    delete sock;
    esp_restart();
}