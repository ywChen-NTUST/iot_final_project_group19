#ifndef MAZE_USER
#define MAZE_USER

#include <Arduino.h>

class User {
public:
    User();
    void moveto(int pos_x, int pos_y);
    void getPos(int& pos_x, int& pos_y);
    void setLife(int life);
    int getLife();
    ~User();
private:
    int _pos_x;
    int _pos_y;
    int _life;
};

#endif