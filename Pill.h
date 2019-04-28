#pragma once
#include "Item.h"
#include "Player.h"

const char PILL('o'); //power up pill
struct pill: item
{
    bool show;
    pill(int x, int y);

    void collide(player& player);
};
