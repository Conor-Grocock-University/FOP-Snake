#pragma once

#include "Globals.h"
#include "RandomUtils.h"
#include "Item.h"

Position getRandomPosition(const char grid[][SIZEX]);
void setRandomItemPosition(const char grid[][SIZEX], item& item);
bool validPosition(const char grid[][SIZEX], int x, int y);

