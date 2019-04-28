#pragma once
#include "Item.h"
#include "Player.h"
#include "RandomUtils.h"
#include "Mouse.h"
#include "Utility.h"

const char MONGOOSE('M');
struct mongoose : item
{
	bool show = false;

	mongoose(int x, int y);

    void walk(const char g[][SIZEX]);
    void update(const char[][SIZEX], const player& player);
	bool collide(player& player) const;

    virtual void place(char g[][SIZEX]) const override;
};