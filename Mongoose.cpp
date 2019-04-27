#include "Mongoose.h"

mongoose::mongoose(int _x, int _y) : item(_x, _y, MONGOOSE) {}

bool mongoose::collide(player& player)
{
	if (show)
		player.alive = false;
	show = false;
    
return false;
}

void mongoose::walk()
{
    x += ((std::rand() % 3) - 1);
    y += ((std::rand() % 3) - 1);
}

void mongoose::update(player player) {
    // every three mice caught, a mongoose spawns (check for that)
    if (player.mouseCount >= 3)
    {
        this->walk();
        this->show = true;
    }
    else
        this->show = false;
}