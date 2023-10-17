#pragma once

#include <chrono>

namespace NHL {

struct Game {
    int id;
    time_t date;
    std::string home_team;
    int home_team_id;
    std::string away_team;
    int away_team_id;
    int score_home = 0;
    int score_away = 0;
    enum Status {
        Scheduled = 1, PreGame = 2, Live = 3, Critical = 4, GameOver = 5, Final = 6, Final_ = 7, TBD = 8, Postponed = 9
    } status;
    std::string venue;
};

}