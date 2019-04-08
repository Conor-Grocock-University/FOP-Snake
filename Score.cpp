#include "score.h"

std::vector <score> score::load_scores()
{
    std::vector <std::string> split(const std::string& s, char       splitChar);
    bool                      compareScores(score      scoreA, score scoreB);

    std::vector <score> scores;
    std::string         line;
    std::ifstream       score_file(SCOREFILE, std::ifstream::in);
    if (score_file.is_open())
    {
        while (getline(score_file, line))
        {
            std::vector <std::string> components = split(line, '-');
            scores.push_back({components[0], stoi(components[1])});
        }
        score_file.close();
    }

    return scores;
}

score score::get_high_score()
{
    std::vector <score> scores = load_scores();

    score      highest_score = {"annon", 500};
    for (score score : scores)
    {
        if (score.moves < highest_score.moves)
            highest_score = score;
    }

    return highest_score;
}

std::vector <score> score::sort_scores(std::vector <score> scores)
{
    sort(scores.begin(), scores.end(), [](const score& l, const score& r) { return l.moves > r.moves; });

    return scores;
}

void score::record_high_score(std::string playerName, player player)
{
    void        showMessage(WORD backColour, WORD textColour, int x, int y, const std::string& message);
    std::string tostring(int     x);
    if (!player.inCheatMode)
    {
        std::ofstream out(SCOREFILE, std::fstream::app);
        if (out.is_open())
        {
            std::string score_text = playerName + "-" + tostring(player.mouseCount);
            out << score_text;
            out << "\n";
            out.close();
        }
    }
}
