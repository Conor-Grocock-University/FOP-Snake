#pragma once
#include "Globals.h"

struct item
{
    int  x, y;
    char symbol;

    item(int x, int y, char symbol);

    virtual void place(char g[][SIZEX]) const;

    virtual void teleport(int dy, int dx);
    virtual void move(int     dy, int dx);
};
