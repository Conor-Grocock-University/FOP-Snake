#include "Utility.h"

Position getRandomPosition(const char grid[][SIZEX])
{
    int  x, y;
    bool positionEmpty = false;

    do
    {
        x = random(SIZEX - 2);
        y = random(SIZEY - 2);
        // positionEmpty = validPosition(grid, x, y);
    } while (!validPosition(grid, x, y));

    return { x, y };
}

void setRandomItemPosition(const char grid[][SIZEX], item& item)
{
    const Position itemPosition = getRandomPosition(grid);
    item.x = itemPosition.x;
    item.y = itemPosition.y;
}

bool validPosition(const char grid[][SIZEX], int x, int y)
{
    return grid[y][x] == TUNNEL;
}

