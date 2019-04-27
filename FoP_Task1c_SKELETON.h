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
#include "Mongoose.h"
#include "Pill.h"
#include "Score.h"
#include "Globals.h"

#include "RandomUtils.h"    //for seed, random
#include "ConsoleUtils.h"	//for clrscr, gotoxy, etc.
#include "TimeUtils.h"		//for getSystemTime, timeToString, etc.

using namespace std;

struct Position
{
    int x, y;
};

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], player& spot, mouse& mouse, pill& pill, mongoose& mongoose);
void setSpotInitialCoordinates(player& spot, const char maze[][SIZEX]);
void setInitialMazeStructure(char maze[][SIZEX]);
void updateGame(char       grid[][SIZEX],
    const char maze[][SIZEX],
    player&    spot,
    mouse&     mouse,
    pill&      pill,
    mongoose   mongoose,
    const int  keyCode,
    string&    mess);
void updateGameData(const char g[][SIZEX], player& spot, mouse& mouse, pill& pill, mongoose& mongoose, const int key,
    string&    mess);
void updateGrid(char           grid[][SIZEX], const char maze[][SIZEX], const player spot, const mouse mouse,
    const pill     pill,
    const mongoose mongoose);
void handle_input(string& message, string playerName, player spot, mouse mouse, pill pill, int& key);
Position getRandomPosition(const char grid[][SIZEX]);
void setRandomItemPosition(const char grid[][SIZEX], item& item);
bool validPosition(const char grid[][SIZEX], int x, int y);
void placeMaze(char grid[][SIZEX], const char maze[][SIZEX]);
void showScoreboard();
void showGameOver();
void setKeyDirection(const int key, int& dx, int& dy);
int getKeyPress();
bool isArrowKey(const int key);
bool wantsToQuit(const int key);
bool wantsToCheat(const int key);
bool wantsToSeeScoreboard(const int key);
bool askToloadSave();
bool wantsToSave(const int key);
bool saveFileExists(string playerName);
void loadSaveFile(string player_name, player& player, mouse& mouse, pill& pill);
void saveToFile(const string player_name, const player player, const mouse mouse, const pill pill);
void getPlayerInformation(string& name);
void displayPlayerInformation(player player, score highest_score);
string tostring(int x);
string tostring(char x);
void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
void renderGame(const char g[][SIZEX], const string& mess, const player& spot, const pill& pill);
void paintGrid(const char g[][SIZEX], const player& player, const pill& pill);
vector <string> split(const string& s, char splitChar);
void endProgram();