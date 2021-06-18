//
// Created by Dominik Alfery on 13.06.2021.
//

#pragma once

#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <algorithm>

/** class representing scoreboard */
class CScoreboard {
private:
    /** @var where scoreboard is stored */
    std::string savepath = "../examples/scoreboard.txt";
public:
    /**
     * constructor
     */
    CScoreboard();

    /**
     * write new score to file
     * @param score what score to write
     * @return std::set<int, std::greater<>> of top10 score
     */
    std::set<int, std::greater<>> writeScore(int score) const;

    /**
     * read score from file
     * @return std::set<int, std::greater<>> of top10 score
     */
    std::set<int, std::greater<>> readScore() const;
};

