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

#define VIEW_WIDTH 20
#define VIEW_HEIGHT 4

const u_int8_t role_bitmap[8] = {B00000, B01110, B01010, B01010, B01010, B01110, B00100, B00000};
const u_int8_t wall_bitmap[8] = {B11111, B11011, B11011, B10101, B10101, B11011, B11011, B11111};
const u_int8_t key_bitmap[8] = {B00000, B01110, B01010, B01110, B00100, B00110, B00100, B00000};
const u_int8_t plate_bitmap[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B11111, B00000};
const u_int8_t trap_bitmap[8] = {B00000, B00000, B00100, B00100, B00100, B00100, B01110, B00000};
const u_int8_t box_bitmap[8] = {B00000, B01110, B11011, B10001, B10101, B11111, B10001, B11111};
const u_int8_t button_bitmap[8] = {B00000, B00000, B00000, B00100, B00100, B01110, B11111, B00000};

#endif