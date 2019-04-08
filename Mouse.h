#pragma once
#include "Item.h"
#include "Pill.h"
#include "Player.h"

const char MOUSE('M'); //mouse
struct mouse: item
{
    mouse(int x, int y);

    bool mouse::collide(const char g[][SIZEX],
                        player&    spot,
                        mouse&     mouse,
                        pill&      pill,
                        int        dx,
                        int        dy);
};
