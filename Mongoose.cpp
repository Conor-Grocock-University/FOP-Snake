#include "Mongoose.h"

mongoose::mongoose(int _x, int _y) : item(_x, _y, MONGOOSE) {}

bool mongoose::collide(player& player)
{
	if (show)
		player.alive = false;
	show = false;
}

void mongoose::walk(const char g[][SIZEX])
{
		
}