#include "Mouse.h"

mouse::mouse(int _x, int _y): item(_x, _y, MOUSE) {}

bool mouse::collide(const char g[][SIZEX],
                    player&    spot,
                    mouse&     mouse,
                    pill&      pill,
					mongoose&  mongoose,
                    int        dx,
                    int        dy)
{
    void setRandomItemPosition(const char g[][SIZEX], item& item);

    if (!spot.inCheatMode) // If the player is in cheat mode it stays at a fixed size, and score doesn't increase
    {
        spot.maxSize += 2; // Otherwise, increase the max size
        spot.mouseCount++; // And increase the score
    }

    setRandomItemPosition(g, mouse); // Move the mouse to a new position
    setRandomItemPosition(g, pill);  // Move the pill at the same time
    spot.move(dy, dx);               // Allow the player to teleport into the space

    // every two mice caught, a power up pill spawns (check for that)
    if (spot.mouseCount % 2 == 0)
    {
        setRandomItemPosition(g, pill);
        pill.show = true;
    }
    else
        pill.show = false;

	// every three mice caught, a mongoose spawns (check for that)
	if (spot.mouseCount >= 3)
	{
		mongoose.walk();
		mongoose.show = true;
	}
	else
		mongoose.show = false;

    return false;
}
