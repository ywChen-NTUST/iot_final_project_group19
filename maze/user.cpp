#include <Arduino.h>
#include "user.h"

User::User()
{
    this->_pos_x = 1;
    this->_pos_y = 1;
    this->_life = 5;
}

void User::moveto(int pos_x, int pos_y)
{
    this->_pos_x = pos_x;
    this->_pos_y = pos_y;
}

void User::getPos(int& pos_x, int& pos_y)
{
    pos_x = this->_pos_x;
    pos_y = this->_pos_y;
}

void User::setLife(int life)
{
    this->_life = life;
}

int User::getLife()
{
    return this->_life;
}

User::~User() {}