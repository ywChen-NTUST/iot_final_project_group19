#include "display.h"

Display::Display(const u_char lcd_sda, const u_char lcd_scl, const u_char life_leds[5], const u_char key_leds[5])
 : _lcd_sda(lcd_sda), _lcd_scl(lcd_scl)
{
    memcpy(_life_leds, life_leds, sizeof(_life_leds));
    memcpy(_key_leds, key_leds, sizeof(_key_leds));
}

void Display::init(const int lcd_cols, const int lcd_rows)
{
    Wire.begin(_lcd_sda, _lcd_scl);
    _lcd = new LiquidCrystal_I2C(0x27, lcd_cols, lcd_rows);
    _lcd->backlight();
    _lcd->clear();
    _lcd->home();
    for (int i = 0; i < 5; i++)
    {
        pinMode(_life_leds[i], OUTPUT);
        pinMode(_key_leds[i], OUTPUT);
    }
}

void Display::lcdMsg(const String& message, const int x, const int y, bool verbose)
{
    if (verbose)
        Serial.printf("Display: %s at (%d, %d)\n", message.c_str(), x, y);
    _lcd->setCursor(x, y);
    _lcd->print(message);
}

void Display::lcdClear()
{
    _lcd->clear();
}

void Display::renderMaze(const int myuid, const int pos_x[], const int pos_y[])
{
    for(int uid = 1; uid <= 2; uid++)
    {
        lcdMsg("uid: " + String(uid), 0, 2*uid-2);
        if (uid == myuid)
            lcdMsg("*", 19, 2*uid-2);
        lcdMsg("x: " + String(pos_x[uid]), 0, 2*uid-1);
        lcdMsg("y: " + String(pos_y[uid]), 8, 2*uid-1);
    }
}

void Display::showLife(const int life)
{
    for (int i = 0; i < 5; i++)
        digitalWrite(_life_leds[i], i < life);
}

void Display::showKey(const int key)
{
    for (int i = 0; i < 5; i++)
        digitalWrite(_key_leds[i], i < key);
}

Display::~Display()
{
    _lcd->clear();
    _lcd->noBacklight();
    _lcd->noDisplay();
    delete _lcd;
}

