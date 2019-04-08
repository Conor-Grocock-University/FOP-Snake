#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include "Player.h"

#include "ConsoleUtils.h"	//for clrscr, gotoxy, etc.

struct score
{
    std::string                name;
    int                        moves;
    static std::vector <score> load_scores();
    static score               get_high_score();
    static std::vector <score> sort_scores(std::vector <score> scores);
    static void                record_high_score(std::string   playerName, player player);
};
