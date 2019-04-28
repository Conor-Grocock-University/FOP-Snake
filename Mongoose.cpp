#include "Mongoose.h"

mongoose::mongoose(int _x, int _y) : item(_x, _y, MONGOOSE) {}

bool mongoose::collide(player& player) const
{
	if (show)
		player.alive = false;
    
    return false;
}

void mongoose::place(char g[][SIZEX]) const
{
    if(show) g[y][x] = symbol;
}

void mongoose::walk(const char g[][SIZEX])
{
    int new_x = random(3) - 2;
    int new_y = random(3) - 2;

    while(g[y + new_y][x + new_x] == WALL) {
        new_x = random(3) - 2;
        new_y = random(3) - 2;
    }

    move(new_x, new_y);
}

void mongoose::update(const char g[][SIZEX], const player& player) {
    if (player.mouseCount >= 3) {
        this->show = true;
        this->walk(g);
     }
}