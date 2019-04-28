#pragma once
#include <vector>
#include "Item.h"
#include "Tail.h"

const char SPOT('@'); //spot

struct player: item
{
    std::vector <tail> tails;
    // A vector of tail object, allowing tails to be pushed and popped allowing for grid rendering
    int maxSize,               // Store the maximum size of the snake, used to control the growth of the snake
        mouseCount,            // How many mice has the player eaten in the current game
        invincibleCountdown{}, // How many moves the player has of remaining invincible
        numberOfMoves;         // How many moves has the player made since the start of the game
    bool inCheatMode,          // Is the player currently in cheat mode
         inInvincibleMode,     // Is the player currently in invincible mode
         alive = true;         // Has the player hit a wall or its own tail
    std::string causeOfDeath;

         player(int x, int y);
    void place(char g[][SIZEX]) const;

    void player::teleport(int new_y, int new_x) override;
    void player::move(int     dy, int    dx) override;
    void wrap_player(int      dy, int    dx);

    void toggle_invinsibility();
    void toggle_cheatmode();
    void invincibility_countdown();

    void player::update();
};
