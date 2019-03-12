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
const int  SIZEX(15);    	//horizontal dimension
const int  SIZEY(10);		//vertical dimension
//defining symbols used for display of the grid and content
const char SPOT('@');   	//spot
const char TAIL('+');   	//spot
const char TUNNEL(' ');    	//tunnel
const char WALL('#');    	//border
const char MOUSE('M');    	//mouse
//defining the command letters to move the spot on the maze
const int  UP(72);			//up arrow
const int  DOWN(80); 		//down arrow
const int  RIGHT(77);		//right arrow
const int  LEFT(75);		//left arrow
//defining the other command letters
const char QUIT('Q');		//to end the game
const char QUITLOWER('q'); //to end the game with lowercase q

struct Item {
	int x, y;
	char symbol;
};

struct Tail : Item {
	Tail(int _x, int _y) {
		x = _x;
		y = _y;
		symbol = TAIL;
	}
};

struct Player : Item {
	vector<Tail> tails;
	int maxSize;

	Player(int _x, int _y) {
		x = _x;
		y = _y;
		symbol = SPOT;
		maxSize = 4;
	}
};

struct Mouse : Item {
	Mouse(int _x, int _y) {
		x = _x;
		y = _y;
		symbol = MOUSE;
	}
};

struct Position {
	int x, y;
};

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
	//function declarations (prototypes)
	void initialiseGame(char g[][SIZEX], char m[][SIZEX], Player& spot, Mouse& mouse);
	void renderGame(const char g[][SIZEX], const string& mess);
	void updateGame(char g[][SIZEX], const char m[][SIZEX], Player& s, Mouse& mouse, const int kc, string& mess);
	bool wantsToQuit(const int key);
	bool isArrowKey(const int k);
	int  getKeyPress();
	void endProgram();

	//local variable declarations 
	char grid[SIZEY][SIZEX];			//grid for display
	char maze[SIZEY][SIZEX];			//structure of the maze
	Player spot = { 0, 0 };			 	//spot's position and symbol
	Mouse mouse = { 3, 3 };
	string message("LET'S START...");	//current message to player

	//action...
	seed();								//seed the random number generator
	SetConsoleTitle("FoP 2018-19 - Task 1c - Game Skeleton");
	initialiseGame(grid, maze, spot, mouse);	//initialise grid (incl. walls and spot)
	int key;							//current key selected by player
	do {
		renderGame(grid, message);			//display game info, modified grid and messages
		//TODO: Ensure command letters are not  case sensitive  // Q or q is only one??
		key = getKeyPress(); 	//read in  selected key: arrow or letter command
		if (isArrowKey(key))
			updateGame(grid, maze, spot, mouse, key, message);

		else
			message = "INVALID KEY!";  //set 'Invalid key' message
	} while (!wantsToQuit(key));		//while user does not want to quit
	renderGame(grid, message);			//display game info, modified grid and messages
	endProgram();						//display final message
	return 0;
}


//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Player& spot, Mouse& mouse)
{ //initialise grid and place spot in middle
	void setInitialMazeStructure(char maze[][SIZEX]);
	void setRandomItemPosition(const char g[][SIZEX], Item& item);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], const Player& i, const Mouse& n);

	setInitialMazeStructure(maze);		//initialise maze
	setRandomItemPosition(maze, spot);
	setRandomItemPosition(maze, mouse);
	updateGrid(grid, maze, spot, mouse);		//prepare grid
}

void setSpotInitialCoordinates(Player& spot,const char maze[][SIZEX])
{ //set spot coordinates inside the grid at random at beginning of game

	Position getRandomPosition(const char g[][SIZEX]);

	Position playerPosition = getRandomPosition(maze);
	spot.x = playerPosition.x;
	spot.y = playerPosition.y;

} 

void setInitialMazeStructure(char maze[][SIZEX])
{ //set the position of the walls in the maze
  //initialise maze configuration
	char initialMaze[SIZEY][SIZEX] 	//local array to store the maze structure
		= { { '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#','#', '#','#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', ' ', '#', '#', ' ', ' ',' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ',' ', ' ', ' ', '#' },
		{ '#', ' ', '#', '#', '#', ' ', ' ', ' ', '#', ' ', ' ',' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', '#' },
		{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' } };
	//with '#' for wall, ' ' for tunnel, etc. 
	//copy into maze structure with appropriate symbols
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			switch (initialMaze[row][col])
			{
			//not a direct copy, in case the symbols used are changed
			case '#': maze[row][col] = WALL; break;
			case ' ': maze[row][col] = TUNNEL; break;
			}
}

//---------------------------------------------------------------------------
//----- Update Game
//---------------------------------------------------------------------------

void updateGame(char grid[][SIZEX], const char maze[][SIZEX], Player& spot, Mouse& mouse, const int keyCode, string& mess)
{ //update game
	void updateGameData(const char g[][SIZEX], Player& s, Mouse& mo, const int kc, string& ms);
	void updateGrid(char g[][SIZEX], const char maze[][SIZEX], const Player& s, const Mouse& mouse);
	updateGameData(grid, spot, mouse, keyCode, mess);		//move spot in required direction
	updateGrid(grid, maze, spot, mouse);					//update grid information
}

void updateGameData(const char g[][SIZEX], Player& spot, Mouse& mouse, const int key, string& mess)
{ //move spot in required direction
	bool isArrowKey(const int k);
	void setKeyDirection(int k, int& dx, int& dy);
	void movePlayer(Player& spot, int dy, int dx);
	void setRandomItemPosition(const char g[][SIZEX], Item& item);
	assert (isArrowKey(key));

	//reset message to blank
	mess = "";

	//calculate direction of movement for given key
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy);

	//check new target position in grid and update game data (incl. spot coordinates) if move is possible
	switch (g[spot.y + dy][spot.x + dx])
	{			//...depending on what's on the target position in grid...
	case TUNNEL:		//can move
		movePlayer(spot, dy, dx);
		break;
	case WALL:  		//hit a wall and stay there
		//cout << '\a';	//beep the alarm
		mess = "CANNOT GO THERE!";
		break;
	case MOUSE:
		spot.maxSize += 2;
		setRandomItemPosition(g, mouse);
		break;
	}
}

void setRandomItemPosition(const char grid[][SIZEX], Item& item) {
	Position getRandomPosition(const char g[][SIZEX]);

	Position itemPosition = getRandomPosition(grid);
	item.x = itemPosition.x;
	item.y = itemPosition.y;
}

Position getRandomPosition(const char grid[][SIZEX]) {
	bool validPosition(const char grid[][SIZEX], int x, int y);

	int x, y;
	bool positionEmpty = false;

	while (!positionEmpty) {
		x = random(SIZEX - 1);
		y = random(SIZEX - 1);
		positionEmpty = validPosition(grid, x, y);
	}

	return { x, y };
}

bool validPosition(const char grid[][SIZEX], int x, int y) {

	bool validPosition = false;
	if (grid[y][x] == TUNNEL)
		validPosition = true;
	return validPosition;
}

void movePlayer(Player & spot, int dy, int dx)
{
	while (spot.tails.size() > spot.maxSize - 1) {
		spot.tails.erase(spot.tails.begin());
	}
	spot.tails.push_back({ spot.x, spot.y });
	spot.y += dy;	//go in that Y direction
	spot.x += dx;	//go in that X direction
}


void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const Player& spot, const Mouse& mouse)
{ //update grid configuration after each move
	void placeMaze(char g[][SIZEX], const char b[][SIZEX]);
	void placeItem(char g[][SIZEX], const Item& item);
	void placePlayer(char g[][SIZEX], const Player& spot);

	placeMaze(grid, maze);	//reset the empty maze configuration into grid
	placeItem(grid, mouse);
	placePlayer(grid, spot);	//set spot in grid
}

void placeMaze(char grid[][SIZEX], const char maze[][SIZEX])
{ //reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

void placePlayer(char g[][SIZEX], const Player& player) {
	void placeItem(char g[][SIZEX], const Item& item);
	placeItem(g, player);
	for each (Item tail in player.tails)
	{
		placeItem(g, tail);
	}
}

void placeItem(char g[][SIZEX], const Item& player)
{ //place item at its new position in grid
	g[player.y][player.x] = player.symbol;
}
//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------
void setKeyDirection(const int key, int& dx, int& dy)
{ //calculate direction indicated by key
	bool isArrowKey(const int k);
	assert (isArrowKey(key));
	switch (key)	//...depending on the selected key...
	{
	case LEFT:  	//when LEFT arrow pressed...
		dx = -1;	//decrease the X coordinate
		dy = 0;
		break;
	case RIGHT: 	//when RIGHT arrow pressed...
		dx = +1;	//increase the X coordinate
		dy = 0;
		break;
	case UP:  		//when UP arrow pressed...
		dx =  0;
		dy = -1;	// decrease the Y coordinate
		break;
	case DOWN: 		//when DOWN arrow pressed...
		dx =  0;
		dy = +1;	// increase the Y coordinate
		break;
	}
}

int getKeyPress()
{ //get key or command selected by user
  //KEEP THIS FUNCTION AS GIVEN
	int keyPressed;
	keyPressed = _getch();			//read in the selected arrow key or command letter
	while (keyPressed == 224) 		//ignore symbol following cursor key
		keyPressed = _getch();
	return keyPressed;				 
}

bool isArrowKey(const int key)
{	//check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
	return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN);
}
bool wantsToQuit(const int key)
{	//check if the user wants to quit (when key is 'Q' or 'q')
	return key == QUIT || key==QUITLOWER;
}

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------

string tostring(int x)
{	//convert an integer to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
string tostring(char x) 
{	//convert a char to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message)
{	//display a string using specified colour at a given position 
	gotoxy(x, y);
	selectBackColour(backColour);
	selectTextColour(textColour);
	cout << message + string(40 - message.length(), ' ');
}
void renderGame(const char g[][SIZEX], const string& mess)
{ //display game title, messages, maze, spot and other items on screen
	string tostring(char x);
	string tostring(int x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	void paintGrid(const char g[][SIZEX]);
	//display game title
	showMessage(clBlack, clGreen, 0, 0, "Snake Game");
	time_t now = time(0);
	string dt = ctime(&now);
	showMessage(clWhite, clRed, 40, 0, "FoP Task 1c - "+dt);
	showMessage(clWhite, clRed, 40, 1, "SE2 - Conor Grocock (b8022088)");
	showMessage(clWhite, clRed, 40, 2, "SE2 - Rae Hewitt (b8014125)");
	//display menu options available
	showMessage(clRed, clYellow, 40, 4, "TO MOVE - USE KEYBOARD ARROWS ");
	showMessage(clRed, clYellow, 40, 5, "TO QUIT - PRESS 'Q'           ");

	//print auxiliary messages if any
	showMessage(clBlack, clWhite, 40, 9, mess);	//display current message

	//display grid contents
	paintGrid(g);
}

void paintGrid(const char g[][SIZEX])
{ //display grid content on screen
	selectBackColour(clBlack);
	selectTextColour(clWhite);
	gotoxy(0, 2);
	for (int row(0); row < SIZEY; ++row)
	{
		for (int col(0); col < SIZEX; ++col) {
			char cell = g[row][col];
			if (cell == '@') //check for snake
				selectTextColour(clGreen); //if rendering the snake make it green
			else
				selectTextColour(clWhite); //any other cell is white
			cout << cell; //output cell content
		}
		cout << endl;
	}
}

void endProgram()
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	showMessage(clRed, clYellow, 40, 8, "Goodbye!");	
	//system("pause");	//hold output screen until a keyboard key is hit
}
