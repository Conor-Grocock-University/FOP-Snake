#include "Pill.h"

pill::pill(int _x, int _y) : item(_x, _y, PILL) { show = false; }

void pill::collide(player& player)
{
    if (show)
        player.toggle_invinsibility();
    show = false;
}
