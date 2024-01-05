#ifndef MAZE_CONTROL
#define MAZE_CONTROL

#include <Arduino.h>
#include "global.h"

enum MAZE_CONTROL_TYPE {
    MAZE_CONTROL_NONE,
    MAZE_CONTROL_UP,
    MAZE_CONTROL_DOWN,
    MAZE_CONTROL_LEFT,
    MAZE_CONTROL_RIGHT,
    MAZE_CONTROL_ENTER,
};
typedef enum MAZE_CONTROL_TYPE maze_control_type;

class Control
{
public:
    Control(const u_char up_pin, const u_char down_pin, const u_char left_pin, const u_char right_pin, const u_char enter_pin);
    void init();
    maze_control_type readControl();
    bool checkPos(const int x, const int y);
    maze_entity_type checkEntity(const int x, const int y);
    maze_entity_type interact(const int x, const int y);

private:
    const u_char up_pin;
    const u_char down_pin;
    const u_char left_pin;
    const u_char right_pin;
    const u_char enter_pin;
};

#endif