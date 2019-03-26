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
#include <fstream>
#include <algorithm>
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
const char SCOREBOARD('B'); //to end the game

const int GAMEDELAY(100); // Time to wait between 'frames' in miliseconds
const string SCOREFILE("bestscores.txt");
const string SAVEFILEEXTENSION(".save");

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
	bool inCheatMode, inInvincibleMode, alive = true;

	Player(int _x, int _y)
	{
		x = _x;
		y = _y;
		symbol = SPOT;
		maxSize = 4;
		mouseCount = 0;
		inCheatMode = false;
		inInvincibleMode = false;
        alive = true;
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

struct Score
{
    string name;
    int mice;
};

#pragma endregion

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
	//function declarations (prototypes)
    Score GetHighScore();
	void initialiseGame(char g[][SIZEX], char m[][SIZEX], Player& spot, Mouse& mouse, Pill& pill);
    void getPlayerInformation(string& name);
    void RecordHighScore(string playerName, Player player);
    void renderGame(const char g[][SIZEX], const string& mess, const Player& spot, const Pill& pill);
	void updateGame(char g[][SIZEX], const char m[][SIZEX], Player& s, Mouse& mouse, Pill& pill, int kc, string& mess);
    void saveToFile(const string player_name, const Player player, const Mouse mouse, const Pill pill);
    void displayPlayerInformation(string playerName, Player player, Score highest_score);
    void ShowScoreboard();
	void toggle_cheatmode(Player& spot);
	void endProgram();
    void showGameOver();
    void loadSaveFile(string player_name, Player &player, Mouse &mouse, Pill &pill);
    bool saveFileExists(string playerName);
	bool wantsToQuit(int key);
    bool wantsToCheat(int key);
    bool wantsToSeeScoreboard(int key);
	bool isArrowKey(int k);
    bool askToLoadSave();
	int getKeyPress();

	//local variable declarations 
	char grid[SIZEY][SIZEX];          //grid for display
	char maze[SIZEY][SIZEX];          //structure of the maze
	string message("LET'S START..."); //current message to player
    string playerName;

	Player spot = {0, 0};             //spot's position
	Mouse mouse = {3, 3};             //mouse's position
	Pill pill = {0, 0};               //power up pill's position

    //action...
	seed(); //seed the random number generator
	SetConsoleTitle("FoP 2018-19 - Task 1c - Game Skeleton");
    int key = 0;

	//Store the current key press. Default to up so the snake moves up when the game starts
	do
	{
        clrscr();

        getPlayerInformation(playerName);


        const Score highest_score = GetHighScore();

        spot  = { 0, 0 };             //spot's position
        mouse = { 3, 3 };             //mouse's position
        pill  = { 0, 0 };               //power up pill's position


	    initialiseGame(grid, maze, spot, mouse, pill); //initialise grid (incl. walls and spot)
        
	    if(saveFileExists(playerName))
        {
            if(askToLoadSave())
            {
                loadSaveFile(playerName, spot, mouse, pill);
            }
        }
        clrscr();
	    key = 0;

        while(spot.alive && !wantsToQuit(key)) {
		    renderGame(grid, message, spot, pill); //display game info, modified grid and messages
            displayPlayerInformation(playerName, spot, highest_score);

		    Sleep(GAMEDELAY);
		    if (_kbhit())
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
                else if (wantsToSeeScoreboard(newKey))
                    ShowScoreboard();
                else if (newKey == 's')
                    saveToFile(playerName, spot, mouse, pill);
                else if(newKey == 'z') {
                    spot.inInvincibleMode = true;
                    spot.invincibleCountdown = 50;
                }
			    else
				    message = "INVALID KEY!"; //set 'Invalid key' message
		    }

		    if (isArrowKey(key))
			    updateGame(grid, maze, spot, mouse, pill, key, message);

        }

        showGameOver();
        RecordHighScore(playerName, spot);
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

void updateGame(char grid[][SIZEX], const char maze[][SIZEX], Player& spot, Mouse& mouse, Pill& pill, const int keyCode, string& mess)
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
    void showMessage(WORD backColour, WORD textColour, int x, int y, const string& message);
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
			spot.alive = false;
		}
		break;
	case MOUSE:
		player_collides_with_mouse(g, spot, mouse, pill, dx, dy);
		break;
	case PILL:
        if(!pill.show) break;
		spot.maxSize = 4;
		spot.inInvincibleMode = true;
        spot.invincibleCountdown = 20;
		pill.show = false;
		movePlayer(spot, dy, dx);
		break;
	}
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

void setRandomItemPosition(const char grid[][SIZEX], Item& item)
{
	Position getRandomPosition(const char g[][SIZEX]);

	Position itemPosition = getRandomPosition(grid);
	item.x = itemPosition.x;
	item.y = itemPosition.y;
}

bool validPosition(const char grid[][SIZEX], int x, int y)
{
	return (grid[y][x] == TUNNEL);
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

void showGameOver()
{
    void showMessage(WORD backColour, WORD textColour, int x, int y, const string& message);
    int getKeyPress();

    clrscr();


    showMessage(clBlack, clWhite, 0, 0, "Game over");
    showMessage(clBlack, clWhite, 0, 1, "Press return to continue");

    bool restart = false;
    while (!restart) {
        int newKey = getKeyPress(); //read in  selected key: arrow or letter command
        if (newKey == '\r' || newKey == '\n') restart = true;
    }
}

//---------------------------------------------------------------------------
//----- Player specific code
//---------------------------------------------------------------------------

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
    if (spot.mouseCount % 2 == 0)
    {
        //spot.mouseCount = 0;
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

    if (new_x == 1) new_x = SIZEX - 2;      // Run through each possible of the 4 walls
    else if (new_y == 1) new_y = SIZEY - 2; // and check if the player is 
    else if (new_x == SIZEX - 1) new_x = 1; // colliding with them
    else if (new_y == SIZEY - 1) new_y = 1; // If so move them to the opposite wall

    teleportPlayer(spot, new_y, new_x);
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

bool wantsToSeeScoreboard(const int key)
{
    //check if the user wants to quit (when key is 'Q' or 'q')
    return key == SCOREBOARD || key == tolower(SCOREBOARD);
}

void ShowScoreboard()
{
    vector<Score> LoadScores();
    vector<Score> SortScores(vector<Score> scores);
    void showMessage(WORD backColour, WORD textColour, int x, int y, const string& message);
    string tostring(int x);
    int getKeyPress();

    clrscr();
    vector<Score> scores = LoadScores();
    scores = SortScores(scores);

    showMessage(clDarkBlue, clBlue, 0, 0, "High scores");
    for (int i = 0; i <= 6; i++)
    {
        string score_msg;

        if (scores.size() > i) score_msg = scores[i].name + ": " + tostring(scores[i].mice);
        else score_msg = "anonymous: -1";
        showMessage(clBlack, clWhite, 0, i, score_msg);
    }

    bool restart = false;
    while (!restart) {
        int newKey = getKeyPress(); //read in  selected key: arrow or letter command
        if (newKey == '\r' || newKey == '\n') restart = true;
    }
}

//---------------------------------------------------------------------------
//----- Scoring
//---------------------------------------------------------------------------

vector<Score> LoadScores()
{
    vector<string> split(const string& s, char splitChar);
    bool compareScores(const Score scoreA, const Score scoreB);

    vector<Score> scores;
    string line;
    ifstream score_file(SCOREFILE, std::ifstream::in);
    if (score_file.is_open())
    {
        while (getline(score_file, line))
        {
            vector<string> components = split(line, '-');
            scores.push_back({ components[0], stoi(components[1]) });
        }
        score_file.close();
    }

    return scores;
}

Score GetHighScore()
{
    vector<Score> LoadScores();

    vector<Score> scores = LoadScores();

    Score highest_score = { "annon", -1 };
    for (Score score : scores)
    {
        if (score.mice > highest_score.mice) highest_score = score;
    }

    return highest_score;
}

vector<Score> SortScores(vector<Score> scores)
{
    sort(scores.begin(), scores.end(), [](const Score& l, const Score& r) {
        return l.mice > r.mice;
    });

    return scores;
}

void RecordHighScore(string playerName, Player player)
{
    void showMessage(WORD backColour, WORD textColour, int x, int y, const string& message);
    string tostring(int x);
    if (!player.inCheatMode) {
        ofstream out(SCOREFILE, fstream::app);
        if (out.is_open()) {
            string score_text = playerName + "-" + tostring(player.mouseCount);
            out << score_text;
            out << "\n";
            out.close();
        }
    }
}

//---------------------------------------------------------------------------
//----- Saving and loading game states
//---------------------------------------------------------------------------

bool askToLoadSave()
{
    void showMessage(WORD backColour, WORD textColour, int x, int y, const string& message);
    int getKeyPress();

    clrscr();

    showMessage(clBlack, clWhite, 0, 0, "Would you like to load a previous save?");

    bool selectionMade = false;
    int index = 0;
    while (!selectionMade)
    {
        showMessage(clBlack, clWhite, 0, index, "> ");
        showMessage(clBlack, clWhite, 0, 1 - index, " ");
        showMessage(clBlack, clWhite, 10, 0, "No");
        showMessage(clBlack, clWhite, 10, 1, "Yes");

        const int key = getKeyPress();
        if (key == UP)
        {
            index--;
            if (index < 0) index = 1;
        }
        else if (key == DOWN)
        {
            index++;
            if (index > 1) index = 0;
        }
        else if (key == RIGHT)
        {
            selectionMade = true;
        }

    }
    return index;
}

bool saveFileExists(string playerName)
{
    ifstream f(playerName + SAVEFILEEXTENSION);
    return f.good();
}

void loadSaveFile(string player_name, Player &player, Mouse &mouse, Pill &pill)
{
    vector<string> split(const string& s, char splitChar);

    string objectLine;
    string line;
    ifstream score_file(player_name + SAVEFILEEXTENSION, std::ifstream::in);
    if (score_file.is_open())
    {
        while (getline(score_file, line))
        {
            vector<string> components = split(line, ':');
            if (components[0] == "player")
            {
                if (components[1] == "x")
                    player.x = stoi(components[2]);
                else if (components[1] == "y")
                    player.y = stoi(components[2]);
                else if (components[1] == "mouse_count")
                    player.mouseCount = stoi(components[2]);
                else if (components[1] == "size")
                    player.maxSize = stoi(components[2]);
            }
            else if (components[0] == "mouse")
            {
                if (components[1] == "x")
                    mouse.x = stoi(components[2]);
                else if (components[1] == "y")
                    mouse.y = stoi(components[2]);
            }
            else if (components[0] == "pill")
            {
                if (components[1] == "x")
                    pill.x = stoi(components[2]);
                else if (components[1] == "y")
                    pill.y = stoi(components[2]);
                else if (components[1] == "show")
                    pill.show = stoi(components[2]);
            }
        }
        score_file.close();
    }
}

void saveToFile(const string player_name, const Player player, const Mouse mouse, const Pill pill)
{
    string tostring(int x);

    ofstream out(player_name + SAVEFILEEXTENSION, fstream::trunc);
    if (out.is_open()) {
        out << "player:x:" << tostring(player.x) << "\n";
        out << "player:y:" << tostring(player.y) << "\n";
        out << "player:mouse_count:" << tostring(player.mouseCount) << "\n";
        out << "player:size:" << tostring(player.maxSize) << "\n";

        out << "mouse:x:" << tostring(mouse.x) << "\n";
        out << "mouse:y:" << tostring(mouse.y) << "\n";

        out << "pill:x:" << tostring(pill.x) << "\n";
        out << "pill:y:" << tostring(pill.y) << "\n";
        out << "pill:show:" << tostring(pill.show) << "\n";
        out.close();
    }
}

//---------------------------------------------------------------------------
//----- Player information
//---------------------------------------------------------------------------

void getPlayerInformation(string& name)
{
    gotoxy(0, 1);
    cout << "Please enter your name" << endl;
    cin >> name;
}

void displayPlayerInformation(Player player, Score highest_score)
{
    void showMessage(WORD backColour, WORD textColour, int x, int y, const string& message);
    string tostring(int x);

    showMessage(clWhite, clBlack, 40, 7, "Score: " + tostring(player.mouseCount));
    showMessage(clWhite, clBlack, 40, 8, "High score");
    showMessage(clWhite, clBlack, 40, 9, highest_score.name + ": " + tostring(highest_score.mice));
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
		showMessage(clRed, clYellow, 40, 10, "CHEAT MODE ON");
	else
		showMessage(clBlack, clYellow, 40, 10, "");
	if (spot.inInvincibleMode)
		showMessage(clRed, clYellow, 40, 11, "INVINCIBLE SNAKE");
	else
		showMessage(clBlack, clYellow, 40, 11, "");


	//print auxiliary messages if any
	showMessage(clBlack, clWhite, 40, 10, mess); //display current message

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

vector<string> split(const string& s, char splitChar)
{
    vector<string> components;
    string component;
    istringstream componentStream(s);
    while (getline(componentStream, component, splitChar))
    {
        components.push_back(component);
    }
    return components;
}

void endProgram()
{
	void showMessage(WORD backColour, WORD textColour, int x, int y, const string& message);
	showMessage(clRed, clYellow, 40, 8, "Goodbye!");
	//system("pause");	//hold output screen until a keyboard key is hit
}
