//---------------------------------------------------------------------------
//Program: Skeleton for Task 1c – group assignment
//Author: Pascale Vacher
//Last updated: 23 February 2018
//---------------------------------------------------------------------------

//Go to 'View > Task List' menu to open the 'Task List' pane listing the initial amendments needed to this program

//---------------------------------------------------------------------------
//----- include libraries
//---------------------------------------------------------------------------

//include standard libraries
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <cassert>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

//include our own libraries
#include "RandomUtils.h"    //for seed, random
#include "ConsoleUtils.h"	//for clrscr, gotoxy, etc.
#include "TimeUtils.h"		//for getSystemTime, timeToString, etc.
//#include "FoP_Task1c_SKELETON.h"

//---------------------------------------------------------------------------
//----- define constants
//---------------------------------------------------------------------------

//defining the size of the grid
const int SIZEX(15); //horizontal dimension
const int SIZEY(10); //vertical dimension
//defining symbols used for display of the grid and content
const char SPOT('@');   //spot
const char TAIL('+');   //spot
const char TUNNEL(' '); //tunnel
const char WALL('#');   //border
const char MOUSE('M');  //mouse
const char PILL('o');   //power up pill
//defining the command letters to move the spot on the maze
const int UP(72);    //up arrow
const int DOWN(80);  //down arrow
const int RIGHT(77); //right arrow
const int LEFT(75);  //left arrow
//defining the other command letters
const char QUIT('Q');  //to end the game
const char CHEAT('C'); //to end the game

const int GAMEDELAY(200); // Time to wait between 'frames' in miliseconds


#pragma region Structs

struct Item
{
	int x, y;
	char symbol;
};

struct Tail : Item
{
	Tail(int _x, int _y)
	{
		x = _x;
		y = _y;
		symbol = TAIL;
	}
};

struct Player : Item
{
	vector<Tail> tails;
	int maxSize, mouseCount, invincibleCountdown;
	bool inCheatMode, inInvincibleMode;

	Player(int _x, int _y)
	{
		x = _x;
		y = _y;
		symbol = SPOT;
		maxSize = 4;
		mouseCount = 0;
		inCheatMode = false;
		inInvincibleMode = false;
	}
};

struct Mouse : Item
{
	Mouse(int _x, int _y)
	{
		x = _x;
		y = _y;
		symbol = MOUSE;
	}
};

struct Pill : Item
{
	bool show;

	Pill(int _x, int _y)
	{
		x = _x;
		y = _y;
		symbol = PILL;
		show = false;
	}
};

struct Position
{
	int x, y;
};

#pragma endregion

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------


int main()
{
	//function declarations (prototypes)
	void initialiseGame(char g[][SIZEX], char m[][SIZEX], Player& spot, Mouse& mouse, Pill& pill);
	void renderGame(const char g[][SIZEX], const string& mess, const Player& spot, const Pill& pill);
	void updateGame(char g[][SIZEX], const char m[][SIZEX], Player& s, Mouse& mouse, Pill& pill, int kc, string& mess);
	bool wantsToQuit(int key);
	bool wantsToCheat(int key);
	bool isArrowKey(int k);
	void toggle_cheatmode(Player& spot);
	int getKeyPress();
	void endProgram();

	//local variable declarations 
	char grid[SIZEY][SIZEX];          //grid for display
	char maze[SIZEY][SIZEX];          //structure of the maze
	Player spot = {0, 0};             //spot's position
	Mouse mouse = {3, 3};             //mouse's position
	Pill pill = {0, 0};               //power up pill's position
	string message("LET'S START..."); //current message to player

	//action...
	seed(); //seed the random number generator
	SetConsoleTitle("FoP 2018-19 - Task 1c - Game Skeleton");
	initialiseGame(grid, maze, spot, mouse, pill); //initialise grid (incl. walls and spot)
	int key = 0;
	//Store the current key press. Default to up so the snake moves up when the game starts
	do
	{
		renderGame(grid, message, spot, pill); //display game info, modified grid and messages

		Sleep(GAMEDELAY);
		if (kbhit() || key == 0)
		{
			// Detect if any key has been pressed
			// Only update the main key variable if the key is for movement
			// Otherwise run the relevent function. This mean that gameplay is not interupted by 
			// Keys such as quit
			int newKey = getKeyPress(); //read in  selected key: arrow or letter command
			if (isArrowKey(newKey)) key = newKey;

			if (wantsToQuit(newKey)) //if the key is a quit key
				endProgram();        //end the game
			else if (wantsToCheat(newKey))
				toggle_cheatmode(spot);
            else if(newKey == 'z')
                spot.inInvincibleMode = true;
			else
				message = "INVALID KEY!"; //set 'Invalid key' message
		}

		if (isArrowKey(key))
			updateGame(grid, maze, spot, mouse, pill, key, message);
	}
	while (!wantsToQuit(key));             //while user does not want to quit
	renderGame(grid, message, spot, pill); //display game info, modified grid and messages
	return 0;
}


//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Player& spot, Mouse& mouse, Pill& pill)
{
	//initialise grid and place spot in middle
	void setInitialMazeStructure(char maze[][SIZEX]);
	void setRandomItemPosition(const char g[][SIZEX], Item& item);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], const Player& i, const Mouse& n, const Pill& p);

	setInitialMazeStructure(maze); //initialise maze
	setRandomItemPosition(maze, spot);
	setRandomItemPosition(maze, mouse);
	updateGrid(grid, maze, spot, mouse, pill); //prepare grid
}

void toggle_cheatmode(Player& spot)
{
	spot.inCheatMode = !spot.inCheatMode;
	if (spot.inCheatMode)
	{
		cout << '\a' << '\a' << '\a';
		spot.maxSize = 4;
		spot.tails.clear();
	}
}

void setSpotInitialCoordinates(Player& spot, const char maze[][SIZEX])
{
	//set spot coordinates inside the grid at random at beginning of game

	Position getRandomPosition(const char g[][SIZEX]);
	Position playerPosition = getRandomPosition(maze);

	spot.x = playerPosition.x;
	spot.y = playerPosition.y;
}

void setInitialMazeStructure(char maze[][SIZEX])
{
	//set the position of the walls in the maze
	//initialise maze configuration
	char initialMaze[SIZEY][SIZEX] //local array to store the maze structure
		= {
			{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
			{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
			{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
			{'#', ' ', ' ', '#', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', '#'},
			{'#', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#'},
			{'#', ' ', '#', '#', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#'},
			{'#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
			{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
			{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
			{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
		};
	//with '#' for wall, ' ' for tunnel, etc. 
	//copy into maze structure with appropriate symbols
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			switch (initialMaze[row][col])
			{
				//not a direct copy, in case the symbols used are changed
			case '#': maze[row][col] = WALL;
				break;
			case ' ': maze[row][col] = TUNNEL;
				break;
			}
}

//---------------------------------------------------------------------------
//----- Update Game
//---------------------------------------------------------------------------

void updateGame(char grid[][SIZEX], const char maze[][SIZEX], Player& spot, Mouse& mouse, Pill& pill, const int keyCode,
                string& mess)
{
	//update game
	void updateGameData(const char g[][SIZEX], Player& s, Mouse& mo, Pill& p, int kc, string& ms);
	void updateGrid(char g[][SIZEX], const char maze[][SIZEX], const Player& s, const Mouse& mouse, const Pill& pup);
	updateGameData(grid, spot, mouse, pill, keyCode, mess); //move spot in required direction
	updateGrid(grid, maze, spot, mouse, pill);              //update grid information
}

void updateGameData(const char g[][SIZEX], Player& spot, Mouse& mouse, Pill& pill, const int key, string& mess)
{
	//move spot in required direction
	bool isArrowKey(int k);
	void setKeyDirection(int k, int& dx, int& dy);
	void movePlayer(Player& spot, int dy, int dx);
	void setRandomItemPosition(const char g[][SIZEX], Item& item);
	void player_collides_with_wall_in_invincible_mode(Player& spot, const int dx, const int dy);
	void player_collides_with_mouse(const char g[][SIZEX], Player& spot, Mouse& mouse, Pill& pill, const int dx, const int dy);
	assert (isArrowKey(key));

	//reset message to blank
	mess = "";

	//calculate direction of movement for given key
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy);

    if(spot.inInvincibleMode)
    {
        if(spot.invincibleCountdown > 0)
            spot.invincibleCountdown--;
        else spot.inInvincibleMode = false;
    }

	//check new target position in grid and update game data (incl. spot coordinates) if move is possible
	switch (g[spot.y + dy][spot.x + dx])
	{
		//...depending on what's on the target position in grid...
	case TUNNEL: //can move
		movePlayer(spot, dy, dx);
		break;
	case WALL: //hit a wall and stay there
		//cout << '\a';	//beep the alarm
		if (spot.inInvincibleMode)
		{
			player_collides_with_wall_in_invincible_mode(spot, dx, dy);
		}
		else {
			mess = "CANNOT GO THERE!";
		}
		break;
	case MOUSE:
		player_collides_with_mouse(g, spot, mouse, pill, dx, dy);
		break;
	case PILL:
		spot.maxSize = 4;
		spot.mouseCount = 0;
		spot.inInvincibleMode = true;
        spot.invincibleCountdown = 20;
		pill.show = false;
		movePlayer(spot, dy, dx);
		break;
	}
}

void player_collides_with_mouse(const char g[][SIZEX], Player& spot, Mouse& mouse, Pill& pill, const int dx, const int dy)
{
	void setRandomItemPosition(const char g[][SIZEX], Item& item);
	void movePlayer(Player& spot, int dy, int dx);
	if (!spot.inCheatMode)
	{
		spot.maxSize += 2;
		spot.mouseCount++;
	}

	setRandomItemPosition(g, mouse);
	setRandomItemPosition(g, pill);
	movePlayer(spot, dy, dx);

	// every two mice caught, a power up pill spawns (check for that)
	if (spot.mouseCount == 2)
	{
		spot.mouseCount = 0;
		setRandomItemPosition(g, pill);
		pill.show = true;
	}
	else
		pill.show = false;
}

void player_collides_with_wall_in_invincible_mode(Player& spot, const int dx, const int dy)
{
	void teleportPlayer(Player& s, int y, int x);

	int new_x = spot.x + dx; // Store the current x and y values so that the usual method for
	int new_y = spot.y + dy; // moving can be used meaning that the tail will move with the rest

	if (new_x == 0) new_x = SIZEX - 1;      // Run through each possible of the 4 walls
	else if (new_y == 0) new_y = SIZEY - 1; // and check if the player is 
	else if (new_x == SIZEX) new_x = 0; // colliding with them
	else if (new_y == SIZEY) new_y = 0; // If so move them to the opposite wall

	teleportPlayer(spot, new_y, new_x);
}


void setRandomItemPosition(const char grid[][SIZEX], Item& item)
{
	Position getRandomPosition(const char g[][SIZEX]);

	Position itemPosition = getRandomPosition(grid);
	item.x = itemPosition.x;
	item.y = itemPosition.y;
}

Position getRandomPosition(const char grid[][SIZEX])
{
	bool validPosition(const char grid[][SIZEX], int x, int y);

	int x, y;
	bool positionEmpty = false;

	do
	{
		x = random(SIZEX - 2);
		y = random(SIZEY - 2);
		// positionEmpty = validPosition(grid, x, y);
	}
	while (!validPosition(grid, x, y));

	return {x, y};
}

bool validPosition(const char grid[][SIZEX], int x, int y)
{
	return (grid[y][x] == TUNNEL);
}

void movePlayer(Player& spot, int dy, int dx)
{
    void teleportPlayer(Player& spot, int y, int x);
	teleportPlayer(spot, spot.y + dy, spot.x + dx);
}


void teleportPlayer(Player& spot, int y, int x)
{
	while (spot.tails.size() > spot.maxSize - 1)
	{
		spot.tails.erase(spot.tails.begin());
	}
	spot.tails.emplace_back(spot.x, spot.y);
	spot.y = y; //go in that Y direction
	spot.x = x; //go in that X direction
}

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const Player& spot, const Mouse& mouse, const Pill& pill)
{
	//update grid configuration after each move
	void placeMaze(char g[][SIZEX], const char b[][SIZEX]);
	void placeItem(char g[][SIZEX], const Item& item);
	void placePlayer(char g[][SIZEX], const Player& spot);

	placeMaze(grid, maze); //reset the empty maze configuration into grid
	placeItem(grid, mouse);
	placeItem(grid, pill);
	placePlayer(grid, spot); //set spot in grid
}

void placeMaze(char grid[][SIZEX], const char maze[][SIZEX])
{
	//reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

void placePlayer(char g[][SIZEX], const Player& player)
{
	void placeItem(char g[][SIZEX], const Item& item);
	placeItem(g, player);
	for each (Item tail in player.tails)
	{
		placeItem(g, tail);
	}
}

void placeItem(char g[][SIZEX], const Item& item)
{
	//place item at its new position in grid
	g[item.y][item.x] = item.symbol;
}

//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------

void setKeyDirection(const int key, int& dx, int& dy)
{
	//calculate direction indicated by key
	bool isArrowKey(int k);
	assert (isArrowKey(key));
	switch (key) //...depending on the selected key...
	{
	case LEFT:   //when LEFT arrow pressed...
		dx = -1; //decrease the X coordinate
		dy = 0;
		break;
	case RIGHT:  //when RIGHT arrow pressed...
		dx = +1; //increase the X coordinate
		dy = 0;
		break;
	case UP: //when UP arrow pressed...
		dx = 0;
		dy = -1; // decrease the Y coordinate
		break;
	case DOWN: //when DOWN arrow pressed...
		dx = 0;
		dy = +1; // increase the Y coordinate
		break;
	}
}

int getKeyPress()
{
	//get key or command selected by user
	//KEEP THIS FUNCTION AS GIVEN
	int keyPressed;
	keyPressed = _getch();    //read in the selected arrow key or command letter
	while (keyPressed == 224) //ignore symbol following cursor key
		keyPressed = _getch();
	return keyPressed;
}

bool isArrowKey(const int key)
{
	//check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
	return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN);
}

bool wantsToQuit(const int key)
{
	//check if the user wants to quit (when key is 'Q' or 'q')
	return key == QUIT || key == tolower(QUIT);
}

bool wantsToCheat(const int key)
{
	//check if the user wants to quit (when key is 'Q' or 'q')
	return key == CHEAT || key == tolower(CHEAT);
}

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------

string tostring(int x)
{
	//convert an integer to a string
	std::ostringstream os;
	os << x;
	return os.str();
}

string tostring(char x)
{
	//convert a char to a string
	std::ostringstream os;
	os << x;
	return os.str();
}

void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message)
{
	//display a string using specified colour at a given position 
	gotoxy(x, y);
	selectBackColour(backColour);
	selectTextColour(textColour);
	cout << message + string(40 - message.length(), ' ');
}

void renderGame(const char g[][SIZEX], const string& mess, const Player& spot, const Pill& pill)
{
	//display game title, messages, maze, spot and other items on screen
	string tostring(char x);
	string tostring(int x);
	void showMessage(WORD backColour, WORD textColour, int x, int y, const string& message);
	void paintGrid(const char g[][SIZEX], const Player& spot, const Pill& pup);
	//display game title
	showMessage(clBlack, clGreen, 0, 0, "Snake Game");
	showMessage(clWhite, clRed, 40, 0, "FoP Task 1c - " + getTime());
	showMessage(clWhite, clRed, 40, 1, "SE2 - Conor Grocock (b8022088)");
	showMessage(clWhite, clRed, 40, 2, "SE2 - Rae Hewitt (b8014125)");
	//display menu options available
	showMessage(clRed, clYellow, 40, 4, "TO MOVE - USE KEYBOARD ARROWS ");
	showMessage(clRed, clYellow, 40, 5, "TO QUIT - PRESS 'Q'           ");
	if (spot.inCheatMode)
		showMessage(clRed, clYellow, 40, 6, "CHEAT MODE ON");
	else
		showMessage(clRed, clYellow, 40, 6, "");
	if (spot.inInvincibleMode)
		showMessage(clRed, clYellow, 40, 7, "INVINCIBLE SNAKE");
	else
		showMessage(clRed, clYellow, 40, 7, "");

	//print auxiliary messages if any
	showMessage(clBlack, clWhite, 40, 9, mess); //display current message

	//display grid contents
	paintGrid(g, spot, pill);
}

void paintGrid(const char g[][SIZEX], const Player& player, const Pill& pill)
{
	//display grid content on screen
	selectBackColour(clBlack);
	selectTextColour(clWhite);
	gotoxy(0, 2);
	for (int row(0); row < SIZEY; ++row)
	{
		for (int col(0); col < SIZEX; ++col)
		{
			char cell = g[row][col];
			if (cell == SPOT)
			{
				//check for snake 
				if (player.inInvincibleMode)
					selectTextColour(clGreen);   //if rendering the snake make it green
				else selectTextColour(clYellow); //if rendering the snake make it green
			}
			else if (cell == TAIL)
			{
				if (player.inInvincibleMode) selectTextColour(clYellow);
				else selectTextColour(clGreen);
			}
			else if (cell == PILL)
				selectTextColour(clRed);
			else if (cell == WALL)
				selectTextColour(clGrey);
			else
				selectTextColour(clWhite); //any other cell is white

			if (cell == PILL && pill.show == false)
				cout << TUNNEL;
			else
				cout << cell; //output cell content
		}
		cout << endl;
	}
}

void endProgram()
{
	void showMessage(WORD backColour, WORD textColour, int x, int y, const string& message);
	showMessage(clRed, clYellow, 40, 8, "Goodbye!");
	//system("pause");	//hold output screen until a keyboard key is hit
}
