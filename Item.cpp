#include "Item.h"

item::item(int _x, int _y, char _symbol)
{
    x      = _x;
    y      = _y;
    symbol = _symbol;
}

void item::place(char g[][SIZEX]) const { g[y][x] = symbol; }

void item::teleport(const int new_y, const int new_x)
{
    x = new_x;
    y = new_y;
}

void item::move(const int dy, const int dx)
{
    x += dx;
    y += dy;
}
