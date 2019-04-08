#include "Player.h"
#include <iostream>

player::player(int _x, int _y)
    : item(_x, _y, SPOT)
{
    maxSize          = 4;
    mouseCount       = 0;
    inCheatMode      = false;
    inInvincibleMode = false;
    alive            = true;
    numberOfMoves    = 0;
}

void player::place(char g[][SIZEX]) const
{
    g[y][x] = symbol;
    for each (item tail in tails) { tail.place(g); }
}

void player::teleport(const int new_y, const int new_x)
{
    while (tails.size() > maxSize - 1)
        tails.erase(tails.begin());
    tails.emplace_back(x, y);

    y = new_y; //go in that Y direction
    x = new_x; //go in that X direction
}

void player::toggle_invinsibility()
{
    maxSize             = 4;
    inInvincibleMode    = true;
    invincibleCountdown = 20;
}

void player::toggle_cheatmode()
{
    inCheatMode = !inCheatMode;
    if (inCheatMode)
    {
        std::cout << '\a' << '\a' << '\a';
        maxSize = 4;
        tails.clear();
    }
}

void player::invincibility_countdown()
{
    if (inInvincibleMode)
    {
        if (invincibleCountdown > 0)
            invincibleCountdown--;
        else
            inInvincibleMode = false;
    }
}

void player::update()
{
    numberOfMoves++;

    invincibility_countdown();
}

void player::move(const int dy, const int dx) { teleport(y + dy, x + dx); }

void player::wrap_player(const int dy, const int dx)
{
    int new_x = x + dx; // Store the current x and y values so that the usual method for
    int new_y = y + dy; // moving can be used meaning that the tail will move with the rest

    if (new_x == 1)        // Run through each possible of the 4 walls
        new_x = SIZEX - 2; // Set the new x to be just inside the right wall
    else if (new_y == 1)
        new_y = SIZEY - 2; // Set the new y to be just inside the bottom wall
    else if (new_x == SIZEX - 1)
        new_x = 1; // Set the new x to be just inside the left wall
    else if (new_y == SIZEY - 1)
        new_y = 1; // Set the new y to be just inside the top wall

    teleport(new_y, new_x);
}
