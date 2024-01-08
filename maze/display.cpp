#include "display.h"
#include "global.h"

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
    _lcd->createChar(MAZE_ENTITY_WALL, (u_int8_t *)wall_bitmap);
    _lcd->createChar(MAZE_ENTITY_KEY, (u_int8_t *)key_bitmap);
    _lcd->createChar(MAZE_ENTITY_TRAP, (u_int8_t *)trap_bitmap);
    _lcd->createChar(MAZE_ENTITY_PLEASURE_PLATE, (u_int8_t *)plate_bitmap);
    _lcd->createChar(MAZE_ENTITY_BTN, (u_int8_t *)button_bitmap);
    _lcd->createChar(MAZE_ENTITY_BOX, (u_int8_t *)box_bitmap);
    _lcd->createChar(7, (u_int8_t *)role_bitmap);

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

void Display::lcdWrite(u_int8_t chr, const int x, const int y, bool verbose)
{
    if (verbose)
        Serial.printf("Display: %x at (%d, %d)\n", chr, x, y);
    _lcd->setCursor(x, y);
    _lcd->write(chr);
}

void Display::lcdClear()
{
    _lcd->clear();
    _lcd->home();
    memset(_maze_display_prev, 0xff, sizeof(_maze_display_prev));
}

void Display::renderMaze(const int myuid, const int pos_x[], const int pos_y[])
{
    int startX, startY, endX, endY;
    int userX = pos_x[myuid];
    int userY = pos_y[myuid];
    int user2X = pos_x[3 - myuid];
    int user2Y = pos_y[3 - myuid];

    if(userX - VIEW_WIDTH / 2 < 0)
        startX = 0;
    else if(userX + VIEW_WIDTH / 2 > MAZE_WIDTH-1)
        startX = MAZE_WIDTH - VIEW_WIDTH;
    else
        startX = userX - VIEW_WIDTH / 2;
    endX = startX + VIEW_WIDTH;
    if(endX >= MAZE_WIDTH)
        endX = MAZE_WIDTH;

    if(userY - VIEW_HEIGHT / 2 < 0)
        startY = 0;
    else if(userY + VIEW_HEIGHT / 2 > MAZE_HEIGHT-1)
        startY = MAZE_HEIGHT - VIEW_HEIGHT;
    else
        startY = userY - VIEW_HEIGHT / 2;
    endY = startY + VIEW_HEIGHT;
    if(endY >= MAZE_HEIGHT)
        endY = MAZE_HEIGHT;

    // Serial.printf("Display: startX: %d, startY: %d, endX: %d, endY: %d\n", startX, startY, endX, endY);

    for(int y = startY; y < endY; ++y){
        for(int x = startX; x < endX; ++x){
            if(maze[y][x] != _maze_display_prev[y - startY][x - startX])
            {
                _maze_display_prev[y - startY][x - startX] = maze[y][x];
                if(maze[y][x] != MAZE_ENTITY_NONE)
                    lcdWrite(maze[y][x], x - startX, y - startY);
                else
                    lcdWrite(' ', x - startX, y - startY);
            }
            // if(maze[y][x] != MAZE_ENTITY_NONE)
            //     lcdWrite(maze[y][x], x - startX, y - startY);
                // displayMaze[y - startY][x - startX] = maze[y][x];
            // else
            //     displayMaze[y - startY][x - startX] = ' ';
        }
    }

    _maze_display_prev[userY - startY][userX - startX] = 7;
    lcdWrite(7, userX - startX, userY - startY);
    if (user2X >= startX && user2X < endX && user2Y >= startY && user2Y < endY)
    {
        _maze_display_prev[user2Y - startY][user2X - startX] = 7;
        lcdWrite(7, user2X - startX, user2Y - startY);
    }
    
    // _lcd->setCursor(0, 0);
}

void Display::showEvent(maze_animation_type animationid)
{
    u_char entityId;
    switch (animationid)
    {
    case MAZE_ANIMATION_COVERPAGE:
        lcdClear();
        lcdMsg("Welcome to Maze", 2, 0);
        lcdMsg("Press enter", 4, 2);
        lcdMsg("to create user", 3, 3);
        break;
    case MAZE_ANIMATION_KEY:
        entityId = MAZE_ENTITY_KEY;
        lcdClear();
        lcdMsg("You found a key!", 2, 1);
        for(int i = 0; i < VIEW_WIDTH; i++)
        {
            lcdWrite(entityId, i, 0);
            lcdWrite(entityId, i, 3);
        }
        lcdWrite(entityId, 0, 1); lcdWrite(entityId, VIEW_WIDTH-1, 1);
        lcdWrite(entityId, 0, 2); lcdWrite(entityId, VIEW_WIDTH-1, 2);
        break;
    case MAZE_ANIMATION_TRAP:
        entityId = MAZE_ENTITY_TRAP;
        lcdClear();
        lcdMsg("You trigger a trap", 1, 1);
        lcdMsg("Ouch", 8, 2);
        for(int i = 0; i < VIEW_WIDTH; i++)
        {
            lcdWrite(entityId, i, 0);
            lcdWrite(entityId, i, 3);
        }
        lcdWrite(entityId, 0, 1); lcdWrite(entityId, VIEW_WIDTH-1, 1);
        lcdWrite(entityId, 0, 2); lcdWrite(entityId, VIEW_WIDTH-1, 2);
        break;
    case MAZE_ANIMATION_PLEASURE_PLATE:
        entityId = MAZE_ENTITY_PLEASURE_PLATE;
        lcdClear();
        lcdMsg("You step on a plat", 1, 1);
        lcdMsg("Something happened", 1, 2);
        for(int i = 0; i < VIEW_WIDTH; i++)
        {
            lcdWrite(entityId, i, 0);
            lcdWrite(entityId, i, 3);
        }
        lcdWrite(entityId, 0, 1); lcdWrite(entityId, VIEW_WIDTH-1, 1);
        lcdWrite(entityId, 0, 2); lcdWrite(entityId, VIEW_WIDTH-1, 2);
        break;
    case MAZE_ANIMATION_BTN:
        entityId = MAZE_ENTITY_BTN;
        lcdClear();
        lcdMsg("You push a button", 2, 1);
        lcdMsg("Something happened", 1, 2);
        for(int i = 0; i < VIEW_WIDTH; i++)
        {
            lcdWrite(entityId, i, 0);
            lcdWrite(entityId, i, 3);
        }
        lcdWrite(entityId, 0, 1); lcdWrite(entityId, VIEW_WIDTH-1, 1);
        lcdWrite(entityId, 0, 2); lcdWrite(entityId, VIEW_WIDTH-1, 2);
        break;
    case MAZE_ANIMATION_BOX:
        entityId = MAZE_ENTITY_BOX;
        lcdClear();
        lcdMsg("You open a box", 3, 1);
        lcdMsg("Are you lucky?", 3, 2);
        for(int i = 0; i < VIEW_WIDTH; i++)
        {
            lcdWrite(entityId, i, 0);
            lcdWrite(entityId, i, 3);
        }
        lcdWrite(entityId, 0, 1); lcdWrite(entityId, VIEW_WIDTH-1, 1);
        lcdWrite(entityId, 0, 2); lcdWrite(entityId, VIEW_WIDTH-1, 2);
        break;
    case MAZE_ANIMATION_WIN:
        lcdClear();
        lcdMsg("You win!", 6, 0);
        lcdMsg("Congratulation", 3, 2);
        break;
    case MAZE_ANIMATION_LOSS:
        lcdClear();
        lcdMsg("You lose!", 6, 0);
        lcdMsg("Try again next time", 0, 2);
        break;
    default:
        break;
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

