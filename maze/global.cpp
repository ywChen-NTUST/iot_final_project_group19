#include "global.h"

u_char maze[MAZE_HEIGHT][MAZE_WIDTH];

void maze_init()
{
    const u_char temp[MAZE_HEIGHT][MAZE_WIDTH] = 
    {
        {1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,2,0,0,0,0,1},
        {1,0,1,1,1,1,1,1,4,1},
        {1,0,1,0,4,4,0,1,0,1},
        {1,0,1,0,1,1,0,0,0,1},
        {1,0,1,0,1,1,0,1,0,1},
        {1,0,3,0,6,0,0,1,0,1},
        {1,0,1,1,1,1,1,1,0,1},
        {1,5,5,0,0,0,0,0,2,1},
        {1,1,1,1,1,1,1,1,1,1}
    };
    for (int i = 0; i < MAZE_HEIGHT; i++)
    {
        for (int j = 0; j < MAZE_WIDTH; j++)
        {
            maze[i][j] = temp[i][j];
        }
    }
}