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

#include "Player.h"
#include "Mouse.h"
#include "Pill.h"
#include "Score.h"
#include "Globals.h"

#include "RandomUtils.h"    //for seed, random
#include "ConsoleUtils.h"	//for clrscr, gotoxy, etc.
#include "TimeUtils.h"		//for getSystemTime, timeToString, etc.

using namespace std;

//---------------------------------------------------------------------------
//----- define constants
//---------------------------------------------------------------------------

#pragma region Structs

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
    void initialiseGame(char             g[][SIZEX], char m[][SIZEX], player& spot, mouse& mouse, pill& pill);
    void getPlayerInformation(string&    name);
    void displayPlayerInformation(player player, score             highest_score);
    void renderGame(const char           g[][SIZEX], const string& mess, const player& spot, const pill& pill);
    void updateGame(char                 g[][SIZEX],
                    const char           m[][SIZEX],
                    player&              s,
                    mouse&               mouse,
                    pill&                pill,
                    int                  kc,
                    string&              mess);
    void showScoreboard();
    void endProgram();
    void showGameOver();
    void saveToFile(string        player_name, player  player, mouse  mouse, pill  pill);
    void loadSaveFile(string      player_name, player& player, mouse& mouse, pill& pill);
    bool saveFileExists(string    playerName);
    bool wantsToQuit(int          key);
    bool wantsToCheat(int         key);
    bool wantsToSeeScoreboard(int key);
    bool wantsToSave(int          key);
    bool isArrowKey(int           key);
    bool askToloadSave();
    int  getKeyPress();

    //local variable declarations 
    char   grid[SIZEY][SIZEX];        //grid for display
    char   maze[SIZEY][SIZEX];        //structure of the maze
    string message("LET'S START..."); //current message to player
    string playerName;                // Players chosen name

    player spot  = {0, 0}; //spot's position
    mouse  mouse = {3, 3}; //mouse's position
    pill   pill  = {0, 0}; //power up pill's position

    //action...
    seed(); //seed the random number generator
    SetConsoleTitle("FoP 2018-19 - Task 1c - Game Skeleton");
    int key = 0; // Store the keycode (Used for automatic movement)

    //Store the current key press. Default to up so the snake moves up when the game starts
    do
    {
        clrscr(); // Clear the screen after each game

        getPlayerInformation(playerName);
        // Show a screen to get the player name, return it into the variable playerName

        const score highest_score = score::get_high_score(); // Get the current highest score from a file, 

        spot  = {0, 0}; //spot's position
        mouse = {3, 3}; //mouse's position
        pill  = {0, 0}; //power up pill's position

        initialiseGame(grid, maze, spot, mouse, pill); //initialise grid (incl. walls and spot)

        if (saveFileExists(playerName)) // Run a check to see if a save file exists
        {
            if (askToloadSave()) // Ask if the player wants to load the save file
            {
                loadSaveFile(playerName, spot, mouse, pill);
                // Run the function to load the data from the save file into the 3 'actors'
            }
        }
        clrscr(); // Clear the screen before the game begins
        key = 0;  // Reset the key to 0 before the game

        while (spot.alive && spot.mouseCount < MAXSCORE && !wantsToQuit(key))
        {
            renderGame(grid, message, spot, pill);         //display game info, modified grid and messages
            displayPlayerInformation(spot, highest_score); // Show the score information on the side of the screen

            Sleep(GAMEDELAY);
            if (_kbhit())
            {
                // Detect if any key has been pressed
                // Only update the main key variable if the key is for movement
                // Otherwise run the relevent function. This mean that game play 
                // is not interrupted by keys such as quit
                int newKey = getKeyPress(); //read in  selected key: arrow or letter command

                if (isArrowKey(newKey))
                    // If the key is an arrow key, set the key variable, so it can be used to direct movement
                    key = newKey;
                else if (wantsToQuit(newKey))                  //if the key is a quit key
                    endProgram();                              //end the game
                else if (wantsToCheat(newKey))                 // If the key is the cheat key
                    spot.toggle_cheatmode();                   // Enable cheat mode
                else if (wantsToSeeScoreboard(newKey))         // If the key is the scoreboard key
                    showScoreboard();                          // Clear the screen and show the scoreboard
                else if (wantsToSave(newKey))                  // IF the key is the save key
                    saveToFile(playerName, spot, mouse, pill); // Save the current game to a file
                else
                    message = "INVALID KEY!"; //set 'Invalid key' message
            }

            if (isArrowKey(key)) // Update the game if the current key is an arrow key, if not, re run the loop
                updateGame(grid, maze, spot, mouse, pill, key, message);
        }

        showGameOver();
        score::record_high_score(playerName, spot);
    }
    while (!wantsToQuit(key));             //while user does not want to quit
    renderGame(grid, message, spot, pill); //display game info, modified grid and messages
    return 0;
}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], player& spot, mouse& mouse, pill& pill)
{
    //initialise grid and place spot in middle
    void setInitialMazeStructure(char     maze[][SIZEX]);
    void setRandomItemPosition(const char g[][SIZEX], item& item);
    void updateGrid(char                  g[][SIZEX],
                    const char            m[][SIZEX],
                    const player&         i,
                    const ::mouse&        n,
                    const ::pill&         p);

    setInitialMazeStructure(maze);             //initialise maze
    setRandomItemPosition(maze, spot);         // Place the player in a random position on the grid
    setRandomItemPosition(maze, mouse);        // Place the mouse in a random position
    setRandomItemPosition(maze, pill);         // Place the pill in a random position
    updateGrid(grid, maze, spot, mouse, pill); //prepare grid
}

void setSpotInitialCoordinates(player& spot, const char maze[][SIZEX])
{
    //set spot coordinates inside the grid at random at beginning of game

    Position       getRandomPosition(const char g[][SIZEX]);
    const Position playerPosition = getRandomPosition(maze);

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
    for (int     row(0); row < SIZEY; ++row)
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

void updateGame(char       grid[][SIZEX],
                const char maze[][SIZEX],
                player&    spot,
                mouse&     mouse,
                pill&      pill,
                const int  keyCode,
                string&    mess)
{
    //update game
    void updateGameData(const char g[][SIZEX],
                        player&    s,
                        ::mouse&   mo,
                        ::pill&    p,
                        int        kc,
                        string&    ms);
    void updateGrid(char           g[][SIZEX],
                    const char     maze[][SIZEX],
                    const player&  s,
                    const ::mouse& mouse,
                    const ::pill&  pup);
    updateGameData(grid, spot, mouse, pill, keyCode, mess); //move spot in required direction
    updateGrid(grid, maze, spot, mouse, pill);              //update grid information
}

void updateGameData(const char g[][SIZEX], player& spot, mouse& mouse, pill& pill, const int key, string& mess)
{
    //move spot in required direction
    bool isArrowKey(int                   k);
    void setKeyDirection(int              k, int&           dx, int& dy);
    void setRandomItemPosition(const char g[][SIZEX], item& item);
    void showMessage(WORD                 backColour,
                     WORD                 textColour,
                     int                  x,
                     int                  y,
                     const string&        message);
    assert (isArrowKey(key));

    //reset message to blank
    mess = "";

    //calculate direction of movement for given key
    int dx(0), dy(0);
    setKeyDirection(key, dx, dy);

    spot.update();

    //check new target position in grid and update game data (incl. spot coordinates) if move is possible
    switch (g[spot.y + dy][spot.x + dx])
    {
        //...depending on what's on the target position in grid...
    case TAIL: spot.alive = false;
        break;
    case TUNNEL: //can move
        spot.move(dy, dx);
        break;
    case WALL: //hit a wall and stay there
        //cout << '\a';	//beep the alarm
        if (spot.inInvincibleMode) { spot.wrap_player(dy, dx); }
        else { spot.alive = false; }
        break;
    case MOUSE: mouse.collide(g, spot, mouse, pill, dx, dy);
        break;
    case PILL: pill.collide(spot);
        spot.move(dy, dx);
        break;
    }
}

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const player& spot, const mouse& mouse, const pill& pill)
{
    //update grid configuration after each move
    void placeMaze(char g[][SIZEX], const char b[][SIZEX]);

    placeMaze(grid, maze); //reset the empty maze configuration into grid

    mouse.place(grid);
    pill.place(grid);
    spot.place(grid);
}

Position getRandomPosition(const char grid[][SIZEX])
{
    bool validPosition(const char grid[][SIZEX], int x, int y);

    int  x, y;
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

void setRandomItemPosition(const char grid[][SIZEX], item& item)
{
    Position getRandomPosition(const char g[][SIZEX]);

    const Position itemPosition = getRandomPosition(grid);
    item.x                      = itemPosition.x;
    item.y                      = itemPosition.y;
}

bool validPosition(const char grid[][SIZEX], int x, int y) { return grid[y][x] == TUNNEL; }

void placeMaze(char grid[][SIZEX], const char maze[][SIZEX])
{
    //reset the empty/fixed maze configuration into grid
    for (int     row(0); row < SIZEY; ++row)
        for (int col(0); col < SIZEX; ++col)
            grid[row][col] = maze[row][col];
}

void showGameOver()
{
    void showMessage(WORD backColour, WORD textColour, int x, int y, const string& message);
    int  getKeyPress();

    clrscr();

    showMessage(clBlack, clWhite, 0, 0, "Game over");
    showMessage(clBlack, clWhite, 0, 1, "Press return to continue");

    bool restart = false;
    while (!restart)
    {
        int newKey = getKeyPress(); //read in  selected key: arrow or letter command
        if (newKey == '\r' || newKey == '\n')
            restart = true;
    }
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

bool wantsToSeeScoreboard(const int key)
{
    //check if the user wants to quit (when key is 'Q' or 'q')
    return key == SCOREBOARD || key == tolower(SCOREBOARD);
}

void showScoreboard()
{
    void   showMessage(WORD backColour, WORD textColour, int x, int y, const string& message);
    string tostring(int     x);
    int    getKeyPress();

    clrscr();
    vector <score> scores = score::load_scores();
    scores                = score::sort_scores(scores);

    showMessage(clDarkBlue, clBlue, 0, 0, "High scores");
    for (int i = 0; i <= 6; i++)
    {
        string score_msg;

        if (scores.size() > i)
            score_msg = scores[i].name + ": " + tostring(scores[i].moves);
        else
            score_msg = "anonymous: -1";
        showMessage(clBlack, clWhite, 0, i, score_msg);
    }

    int newKey;
    do
    {
        newKey = getKeyPress(); //read in  selected key: arrow or letter command
    }
    while (!(newKey == '\n' || newKey == '\r'));
}

bool askToloadSave()
{
    void showMessage(WORD backColour, WORD textColour, int x, int y, const string& message);
    int  getKeyPress();

    clrscr();

    showMessage(clBlack, clWhite, 0, 0, "Would you like to load a previous save?");

    bool selectionMade = false;
    int  index         = 1;
    while (!selectionMade)
    {
        showMessage(clBlack, clWhite, 0, index + 1, "> ");
        showMessage(clBlack, clWhite, 0, 2 - index, " ");
        showMessage(clBlack, clWhite, 10, 1, "No");
        showMessage(clBlack, clWhite, 10, 2, "Yes");

        const int key = getKeyPress();
        if (key == UP)
        {
            index--;
            if (index < 0)
                index = 1;
        }
        else if (key == DOWN)
        {
            index++;
            if (index > 1)
                index = 0;
        }
        else
            if (key == RIGHT) { selectionMade = true; }
    }
    return index;
}

bool wantsToSave(const int key) { return key == SAVE || key == tolower(SAVE); }

//---------------------------------------------------------------------------
//----- Saving and loading game states
//---------------------------------------------------------------------------

bool saveFileExists(string playerName)
{
    ifstream f(playerName + SAVEFILEEXTENSION);
    return f.good();
}

void loadSaveFile(string player_name, player& player, mouse& mouse, pill& pill)
{
    vector <string> split(const string& s, char splitChar);

    string   objectLine;
    string   line;
    ifstream score_file(player_name + SAVEFILEEXTENSION, std::ifstream::in);
    if (score_file.is_open())
    {
        while (getline(score_file, line))
        {
            vector <string> components = split(line, ':');
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
                else if (components[1] == "moves")
                    player.numberOfMoves = stoi(components[2]);
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

void saveToFile(const string player_name, const player player, const mouse mouse, const pill pill)
{
    string tostring(int x);

    ofstream out(player_name + SAVEFILEEXTENSION, fstream::trunc);
    if (out.is_open())
    {
        out << "player:x:" << tostring(player.x) << "\n";
        out << "player:y:" << tostring(player.y) << "\n";
        out << "player:mouse_count:" << tostring(player.mouseCount) << "\n";
        out << "player:moves:" << tostring(player.numberOfMoves) << "\n";
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

void displayPlayerInformation(player player, score highest_score)
{
    void   showMessage(WORD backColour, WORD textColour, int x, int y, const string& message);
    string tostring(int     x);

    showMessage(clWhite, clBlack, 40, 7, "Score: " + tostring(player.numberOfMoves));
    showMessage(clWhite, clBlack, 40, 8, tostring(player.mouseCount) + " out of " + tostring(MAXSCORE));
    showMessage(clWhite, clBlack, 40, 9, "High score");
    showMessage(clWhite, clBlack, 40, 10, highest_score.name + ": " + tostring(highest_score.moves));
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

void renderGame(const char g[][SIZEX], const string& mess, const player& spot, const pill& pill)
{
    //display game title, messages, maze, spot and other items on screen
    string tostring(char        x);
    string tostring(int         x);
    void   showMessage(WORD     backColour, WORD          textColour, int     x, int y, const string& message);
    void   paintGrid(const char g[][SIZEX], const player& spot, const ::pill& pup);

    //display game title
    showMessage(clBlack, clGreen, 0, 0, "Snake Game");

    showMessage(clWhite, clRed, 40, 0, "FoP Task 1c - " + getTime());
    showMessage(clWhite, clRed, 40, 1, "SE2 - Conor Grocock (b8022088)");
    showMessage(clWhite, clRed, 40, 2, "SE2 - Rae Hewitt (b8014125)");
    //display menu options available
    showMessage(clRed, clYellow, 40, 4, "TO MOVE - USE KEYBOARD ARROWS ");
    showMessage(clRed, clYellow, 40, 5, "TO QUIT - PRESS 'Q'           ");

    if (spot.inCheatMode)
        showMessage(clRed, clYellow, 40, 14, "CHEAT MODE ON");
    else
        showMessage(clBlack, clYellow, 40, 14, "");
    if (spot.inInvincibleMode)
        showMessage(clRed, clYellow, 40, 15, "INVINCIBLE SNAKE");
    else
        showMessage(clBlack, clYellow, 40, 15, "");

    //print auxiliary messages if any
    showMessage(clBlack, clWhite, 40, 11, mess); //display current message

    //display grid contents
    paintGrid(g, spot, pill);
}

void paintGrid(const char g[][SIZEX], const player& player, const pill& pill)
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
                    selectTextColour(clGreen); //if rendering the snake make it green
                else
                    selectTextColour(clYellow); //if rendering the snake make it green
            }
            else if (cell == TAIL)
            {
                if (player.inInvincibleMode)
                    selectTextColour(clYellow);
                else
                    selectTextColour(clGreen);
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

vector <string> split(const string& s, char splitChar)
{
    vector <string> components;
    string          component;
    istringstream   componentStream(s);
    while (getline(componentStream, component, splitChar)) { components.push_back(component); }
    return components;
}

void endProgram()
{
    void showMessage(WORD backColour, WORD textColour, int x, int y, const string& message);
    showMessage(clRed, clYellow, 40, 8, "Goodbye!");
    //system("pause");	//hold output screen until a keyboard key is hit
}
