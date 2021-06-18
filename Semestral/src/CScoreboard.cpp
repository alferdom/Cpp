//
// Created by Dominik Alfery on 13.06.2021.
//

#include "CScoreboard.h"


CScoreboard::CScoreboard() : savepath("./examples/scoreboard.txt") {
}


bool is_digits(const std::string &str) {
    return str.find_first_not_of("0123456789") == std::string::npos;
}

std::string deleteWS(std::string &str) {
    str.erase(std::remove_if(str.begin(), str.end(), [](unsigned char x) { return std::isspace(x); }), str.end());
    return str;
}

std::set<int, std::greater<>> CScoreboard::writeScore(const int score) const {
    std::set<int, std::greater<>> topscore;

    std::ifstream myFile(savepath);
    std::string readline;

    if (!myFile) {
        std::ofstream newFile(savepath); //create new file
        newFile << score << '\n';
        topscore.insert(score); //first score input
        newFile.close();
        return topscore;
    }

    while (std::getline(myFile, readline)) {
        if (readline.empty()) {
            continue;
        }
        deleteWS(readline);
        if (is_digits(readline)) {
            topscore.insert(std::stoi(readline));
        }
    }
    myFile.close();

    topscore.insert(score);

    if (topscore.size() > 10) {
        auto it = topscore.begin();
        std::advance(it, 10);
        topscore.erase(it, topscore.end());
    }

    std::ofstream output(savepath);

    for (auto const &sc : topscore) {
        output << sc << '\n';
    }
    output.close();

    return topscore;
}

std::set<int, std::greater<>> CScoreboard::readScore() const {
    std::set<int, std::greater<>> topscore;

    std::ifstream myFile(savepath);
    std::string readline;

    if (!myFile) {
        std::ofstream newFile(savepath); //create new file
        newFile.close();
        return topscore;
    }

    while (std::getline(myFile, readline)) {
        if (readline.empty()) {
            continue;
        }
        deleteWS(readline);
        if (is_digits(readline)) {
            topscore.insert(std::stoi(readline));
        }
    }
    myFile.close();

    return topscore;
}



