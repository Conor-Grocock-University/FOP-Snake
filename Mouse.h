#pragma once
#include "Item.h"
#include "Pill.h"
#include "Player.h"
#include "Mongoose.h"

const char MOUSE('*'); //mouse
struct mouse: item
{
    mouse(int x, int y);

    bool collide(const char g[][SIZEX], player & spot, mouse & mouse, pill & pill, int dx, int dy);
};
