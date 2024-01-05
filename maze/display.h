#ifndef MAZE_DISPLAY
#define MAZE_DISPLAY

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

class Display {
public:
    Display(const u_char lcd_sda, const u_char lcd_scl, const u_char life_leds[5], const u_char key_leds[5]);
    void init(const int lcd_cols, const int lcd_rows);
    void lcdMsg(const String& message, const int x, const int y, bool verbose = false);
    void lcdClear();
    void renderMaze(const int myuid, const int pos_x[], const int pos_y[]);
    void showLife(const int life);
    void showKey(const int key);
    ~Display();
private:
    const u_char _lcd_sda;
    const u_char _lcd_scl;
    u_char _life_leds[5];
    u_char _key_leds[5];
    LiquidCrystal_I2C *_lcd;
};

#endif