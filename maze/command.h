#ifndef MAZE_COMMAND
#define MAZE_COMMAND

#include <Arduino.h>
#include "socket.h"
#include "user.h"
#include "control.h"
#include "global.h"

enum MAZE_CMD {
    MAZE_CMD_PING = 1,
    MAZE_CMD_CREATEUSER = 2,
    MAZE_CMD_GETUSERATTR = 3,
    MAZE_CMD_SETUSERPOS = 4,
    MAZE_CMD_GETKEYNUM = 5,
    MAZE_CMD_DOENTITY = 6,
    MAZE_CMD_SYNCMAZE = 7,
};

enum MAZE_CMD_RESPONSE {
    MAZE_CMD_RESPONSE_OK = 1,
    MAZE_CMD_RESPONSE_ERROR = 2
};

class Command {
public:
    Command();
    void init(const String& ssid, const String& password, const String& ip, int port);
    bool ping();
    bool createUser(u_char& myuid);
    bool getUserAttribute(int& uid, int& pos_x, int& pos_y, int& life);
    bool move(int pos_x, int pos_y);
    u_char getKeyNum();
    bool doEntity(maze_entity_type entity, int pos_x, int pos_y);
    void syncMaze();
    ~Command();
    Socket* _socket;
private:
    String _token;
};

void error(Socket *sock, const String& message);

#endif