#pragma once
#include "Item.h"
#include "Player.h"

const char MONGOOSE('M');
struct mongoose : item
{
	bool show;
	mongoose(int x, int y);

    void update(player player);

	bool mongoose::collide(player& player);
	void mongoose::walk();
};