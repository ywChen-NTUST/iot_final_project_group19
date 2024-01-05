#ifndef MAZE_GLOBAL
#define MAZE_GLOBAL

#include <Arduino.h>

enum MAZE_ENTITY_TYPE {
    MAZE_ENTITY_NONE = 0,
    MAZE_ENTITY_WALL = 1,
    MAZE_ENTITY_KEY = 2,
    MAZE_ENTITY_TRAP = 3,
    MAZE_ENTITY_PLEASURE_PLATE = 4,
    MAZE_ENTITY_BTN = 5,
    MAZE_ENTITY_BOX = 6,
};
typedef enum MAZE_ENTITY_TYPE maze_entity_type;

#define MAZE_HEIGHT 11
#define MAZE_WIDTH 11
extern u_char maze[MAZE_HEIGHT][MAZE_WIDTH];

void maze_init();

#endif