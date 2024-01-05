#include "control.h"

Control::Control(const u_char up_pin, const u_char down_pin, const u_char left_pin, const u_char right_pin, const u_char enter_pin)
    : up_pin(up_pin), down_pin(down_pin), left_pin(left_pin), right_pin(right_pin), enter_pin(enter_pin)
{
}

void Control::init()
{
    pinMode(up_pin, INPUT);
    pinMode(down_pin, INPUT);
    pinMode(left_pin, INPUT);
    pinMode(right_pin, INPUT);
    pinMode(enter_pin, INPUT);
}

maze_control_type Control::readControl()
{
    if (digitalRead(enter_pin) == HIGH)
        return MAZE_CONTROL_ENTER;
    else if (digitalRead(up_pin) == HIGH)
        return MAZE_CONTROL_UP;
    else if (digitalRead(down_pin) == HIGH)
        return MAZE_CONTROL_DOWN;
    else if (digitalRead(left_pin) == HIGH)
        return MAZE_CONTROL_LEFT;
    else if (digitalRead(right_pin) == HIGH)
        return MAZE_CONTROL_RIGHT;
    else
        return MAZE_CONTROL_NONE;
}

bool Control::checkPos(const int x, const int y)
{
    return (x < MAZE_WIDTH && x > 0) && (y < MAZE_HEIGHT && y > 0) && (maze[y][x] != MAZE_ENTITY_WALL);
}

maze_entity_type Control::checkEntity(const int x, const int y)
{
    switch(maze[y][x])
    {
        case MAZE_ENTITY_KEY:
            return MAZE_ENTITY_KEY;
        case MAZE_ENTITY_TRAP:
            return MAZE_ENTITY_TRAP;
        case MAZE_ENTITY_PLEASURE_PLATE:
            return MAZE_ENTITY_PLEASURE_PLATE;
        default:
            return MAZE_ENTITY_NONE;
    };
}

maze_entity_type Control::interact(const int x, const int y)
{
    switch(maze[y][x])
    {
        case MAZE_ENTITY_BTN:
            return MAZE_ENTITY_BTN;
        case MAZE_ENTITY_BOX:
            return MAZE_ENTITY_BOX;
        default:
            return MAZE_ENTITY_NONE;
    };
}