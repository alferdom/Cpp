//
// Created by Dominik Alfery on 07.06.2021.
//

#pragma once

#include <vector>
#include <memory>
#include <map>
#include <list>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>

#include "CMap.h"
#include "CCharacter.h"
#include "CInterface.h"
#include "CMapBuilder.h"
#include "CBonus.h"

/** class representing main game */
class CGame {
private:
    /** @var reference to Interface */
    const CInterface &myInterface;
    /** @var builder to generate map from file */
    const CMapBuilder mybuilder;
    /** @var pointer to stored map */
    std::unique_ptr<CMap> myMap;
    /** @var integers to store level info and score */
    int currLevel, maxLevel, score;
    /** @var list of pointers to created characters */
    std::list<std::unique_ptr<CCharacter>> characters;
    /** bool if game is running */
    bool isRunning = true;
    /** vector of stored pointers to spawned bonuses */
    std::vector<std::shared_ptr<CBonus>> bonus;

public:
    /**
     * constructor
     * @param interface what interface to use
     * @param builder map builder and generator from file
     */
    explicit CGame(const CInterface &interface, const CMapBuilder &builder);

    /**
     * method that starts the game and do main game loop
     * @return true if game has started and ended else if has not started
     */
    bool Start();

    /**
     * spawner of enemies
     * @param cnt number of spawned enemies
     */
    void EnemySpawner(const int cnt);

    /**
    * spawner of bonuses
    * @param coord where to spawn bonus
    */
    void BonusSpawner(const CCoord &coord);

    /**
     * check if character is at the wall
     * @param m_X x coordinates
     * @param m_Y y coordinates
     * @return true if it is else false if it does not
     */
    bool bumptoWALL(unsigned m_X, unsigned m_Y) const;

    /**
     * method for moving bullets in game
     * @param gamewin where to printe bullet
     * @param bullets list of what bullets
     */
    void MoveBullets(WINDOW *gamewin, std::list<CBullet> &bullets);

    /**
     * getter for game score
     * @return int score
     */
    int getScore() const;

};