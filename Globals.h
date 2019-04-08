#pragma once
#include <string>

//defining the size of the grid
const int SIZEX(15); //horizontal dimension
const int SIZEY(10); //vertical dimension
//defining symbols used for display of the grid and content
const char TUNNEL(' '); //tunnel
const char WALL('#');   //border

//defining the command letters to move the spot on the maze
const int UP(72);    //up arrow
const int DOWN(80);  //down arrow
const int RIGHT(77); //right arrow
const int LEFT(75);  //left arrow
//defining the other command letters

//defining key binds
const char QUIT('Q');       //to end the game
const char CHEAT('C');      //to end the game
const char SCOREBOARD('B'); //to end the game
const char SAVE('S');       //to end the game

const int         GAMEDELAY(150); // Time to wait between 'frames' in miliseconds
const std::string SCOREFILE("bestscores.txt");
const std::string SAVEFILEEXTENSION(".save");
const int         MAXSCORE(10);
