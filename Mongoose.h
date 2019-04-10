#pragma once
#include "Item.h"
#include "Player.h"

const char MONGOOSE('#');
struct mongoose : item
{
	bool show;
	mongoose(int x, int y);

	bool mongoose::collide(player& player);
	void mongoose::walk(const char g[][SIZEX]);
};