#ifndef MAZE_DISPLAY
#define MAZE_DISPLAY

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "global.h"

enum MAZE_ANIMATION_TYPE {
    MAZE_ANIMATION_NONE = 0,
    MAZE_ANIMATION_COVERPAGE = 1,
    MAZE_ANIMATION_KEY = 2,
    MAZE_ANIMATION_TRAP = 3,
    MAZE_ANIMATION_PLEASURE_PLATE = 4,
    MAZE_ANIMATION_BTN = 5,
    MAZE_ANIMATION_BOX = 6,
    MAZE_ANIMATION_WIN = 7,
    MAZE_ANIMATION_LOSS = 8,
};
typedef enum MAZE_ANIMATION_TYPE maze_animation_type;

class Display {
public:
    Display(const u_char lcd_sda, const u_char lcd_scl, const u_char life_leds[5], const u_char key_leds[5]);
    void init(const int lcd_cols, const int lcd_rows);
    void lcdMsg(const String& message, const int x, const int y, bool verbose = false);
    void lcdWrite(u_int8_t chr, const int x, const int y, bool verbose = false);
    void lcdClear();
    void renderMaze(const int myuid, const int pos_x[], const int pos_y[]);
    void showEvent(maze_animation_type animationid);
    void showLife(const int life);
    void showKey(const int key);
    ~Display();
private:
    const u_char _lcd_sda;
    const u_char _lcd_scl;
    u_char _life_leds[5];
    u_char _key_leds[5];
    LiquidCrystal_I2C *_lcd;
    u_char _maze_display_prev[VIEW_HEIGHT][VIEW_WIDTH];
};

#endif