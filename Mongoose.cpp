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
    int new_x = random(2) - 1;
    int new_y = random(2) - 1;

    while(!validPosition(g, x + new_x, y + new_y)) {
        new_x = random(2) - 1;
        new_y = random(2) - 1;
    }

    x += new_x;
    y += new_y;
}

void mongoose::update(const char g[][SIZEX], const player& player) {
    this->walk(g);
    if (player.mouseCount >= 3)
        this->show = true;
}