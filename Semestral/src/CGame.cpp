//
// Created by Dominik Alfery on 07.06.2021.
//

#include "CGame.h"

#define DELAY 50000


CGame::CGame(const CInterface &interface, const CMapBuilder &builder) : myInterface(interface),
                                                                        mybuilder(std::move(builder)),
                                                                        myMap(),
                                                                        currLevel(1), maxLevel(3), score(0),
                                                                        characters(),
                                                                        bonus() {
}


bool CGame::Start() {
    if (!mybuilder.CheckMapFiles(maxLevel)) {
        return false;
    }

    myMap = mybuilder.Generate(currLevel);

    WINDOW *gamewin = newwin(myMap->GetHeight() + 1, myMap->GetWidth() + 1, 0, 0);
    WINDOW *infowin = newwin(myMap->GetHeight() + 1, 20, 0, myMap->GetWidth() + 2);

    myInterface.DrawInfoDisplay(infowin);

    myInterface.BeforeGameInfo(gamewin);

    MainPlayer player(100, 10, CCoord(2, 2), EDirection::DOWN, EField::PLAYER);
    characters.push_back(player.Clone());

    EnemySpawner(6);
    int enemykilled = 0;

    while (isRunning) {

        if (currLevel != 1)
            myMap = mybuilder.Generate(currLevel);

        gamewin = newwin(myMap->GetHeight() + 1, myMap->GetWidth() + 1, 0, 0);
        infowin = newwin(myMap->GetHeight() + 1, 20, 0, myMap->GetWidth() + 2);

        keypad(gamewin, TRUE);
        nodelay(gamewin, TRUE);

        myInterface.DrawMap(gamewin, myMap->GetMapData());

        for (auto const &i: characters) {
            myInterface.PrintCharacter(gamewin, i->getCoordinates(), i->getFace());
        }


        while (isRunning) {

            if (characters.front()->getBonus() != nullptr)
                myInterface.UpdateInfoDisplay(infowin, characters.front()->getHealth(),
                                              characters.front()->getAmmo(),
                                              score,
                                              characters.front()->getBonus()->getType().c_str(), currLevel);
            else
                myInterface.UpdateInfoDisplay(infowin, characters.front()->getHealth(),
                                              characters.front()->getAmmo(),
                                              score,
                                              "NONE", currLevel);

            if (characters.front()->IsDead() || characters.front()->getAmmo().second == 0) {
                myInterface.PrintGameOver(gamewin, score);
                isRunning = false;
                nodelay(gamewin, FALSE);
                wgetch(gamewin);
                break;
            }

            if (enemykilled) {
                EnemySpawner(enemykilled * 2);
                enemykilled = 0;
            }

            for (auto it = characters.begin(); it != characters.end(); it++) {
                if (it->get()->IsDead()) {
                    myInterface.PrintCharacter(gamewin, it->get()->getOldCoordinates(), 'X');
                    BonusSpawner(it->get()->getOldCoordinates()); // spawn bonus
                    characters.erase(it--); //spawn new enemy
                    enemykilled++;
                    score += 10 * currLevel;
                    continue;
                }

                if (!it->get()->Move(gamewin, myInterface)) { // moving characters and quit game if ESC key pressed
                    isRunning = false;
                    break;
                }
            }

            //moving bullets
            MoveBullets(gamewin, characters.front()->getBullets());


            auto iter = characters.begin();
            if (bumptoWALL(iter->get()->getCoordinates().m_X, iter->get()->getCoordinates().m_Y)) {
                iter->get()->SetCoordinates(iter->get()->getOldCoordinates());
            }
            iter++;

            while (iter != characters.end()) {
                if (characters.front()->getCoordinates() == iter->get()->getOldCoordinates()) {
                    characters.front()->getHit(iter->get()->getDamage());
                }

                if (bumptoWALL(iter->get()->getCoordinates().m_X, iter->get()->getCoordinates().m_Y)) {
                    iter->get()->SetCoordinates(iter->get()->getOldCoordinates());
                    iter->get()->ChangeDirection();
                }
                iter++;
            }

            for (auto it = bonus.begin(); it != bonus.end(); it++) {
                myInterface.PrintBonus(gamewin, it->get()->getCoordinates());
                if (characters.front()->getCoordinates() == it->get()->getCoordinates()) {
                    characters.front()->setBonus(*it);
                    bonus.erase(it--);
                }

            }

            if (currLevel != maxLevel && score >= 60 * currLevel) {
                currLevel++;
                break;
            }

            wrefresh(infowin);
            wrefresh(gamewin);
            usleep(DELAY - 10000 * (currLevel - 1));
        }
        myInterface.
                eraseWIN(gamewin);
        myInterface.
                eraseWIN(infowin);
        delwin(gamewin);
        delwin(infowin);
    }

    bonus.clear();
    characters.clear();

    return true;
}

bool CGame::bumptoWALL(unsigned int m_X, unsigned int m_Y) const {
    return (m_Y == myMap->GetHeight() ||
            m_X == myMap->GetWidth() - 1 ||
            m_Y == 1 ||
            m_X == 1);
}

void CGame::MoveBullets(WINDOW *gamewin, std::list<CBullet> &bullets) {

    for (auto it = bullets.begin(); it != bullets.end(); it++) {
        bool deleted = false;

        if (!it->firstshot) {
            myInterface.PrintCharacter(gamewin, it->coordinates, ' ');
            for (auto const &character : characters) {
                if (character->getCoordinates() == it->coordinates) {
                    character->getHit(it->damage);
                    deleted = true;
                    break;
                }
            }
        }


        if (deleted) {
            bullets.erase(it--);
            continue;
        }

        it->Move();

        it->firstshot = false;

        for (auto const &character : characters) {
            if (character->getCoordinates() == it->coordinates) {
                character->getHit(it->damage);
                deleted = true;
                break;
            }
        }

        if (deleted) {
            bullets.erase(it--);
            continue;
        }

        if (bumptoWALL(it->coordinates.m_X, it->coordinates.m_Y)) {
            bullets.erase(it--);
            continue;
        }

        myInterface.PrintBullet(gamewin, it->coordinates);
    }
}

void CGame::EnemySpawner(const int cnt) {
    std::random_device r;
    std::default_random_engine gen(r());

    EField enemyface;
    switch (currLevel) {
        default:
            enemyface = EField::ENEMY;
            break;
        case 2:
            enemyface = EField::ENEMYSEC;
            break;
        case 3:
            enemyface = EField::ENEMYTHR;
            break;
    }
    if (currLevel < 3) {
        for (int i = 0; i < cnt; ++i) {
            if (gen() % 2 == 1) {
                Enemy enemy(20 * currLevel, 10 * currLevel,
                            CCoord(2 + gen() % (myMap->GetWidth() - 3), 2 + gen() % (myMap->GetHeight() - 2)),
                            EDirection::LEFT, enemyface);
                characters.push_back(enemy.Clone());
            } else {
                Enemy enemy(20 * currLevel, 10 * currLevel,
                            CCoord(2 + gen() % (myMap->GetWidth() - 3), 2 + gen() % (myMap->GetHeight() - 2)),
                            EDirection::RIGHT, enemyface);
                characters.push_back(enemy.Clone());
            }
        }
    } else {
        for (int i = 0; i < cnt; ++i) {
            if (gen() % 2 == 1) {
                Enemy enemy(20 * currLevel, 10 * currLevel,
                            CCoord(2 + gen() % (myMap->GetWidth() - 3), 2 + gen() % (myMap->GetHeight() - 2)),
                            EDirection::UP, enemyface);
                characters.push_back(enemy.Clone());
            } else {
                Enemy enemy(20 * currLevel, 10 * currLevel,
                            CCoord(2 + gen() % (myMap->GetWidth() - 3), 2 + gen() % (myMap->GetHeight() - 2)),
                            EDirection::DOWN, enemyface);
                characters.push_back(enemy.Clone());
            }
        }
    }
}

int CGame::getScore() const {
    return score;
}

void CGame::BonusSpawner(const CCoord &coord) {
    std::random_device r;
    std::default_random_engine gen(r());

    switch (gen() % 14) {
        default:
            break;
        case 0:
        case 1:
        case 2: {
            CAmmo bon(coord);
            bonus.push_back(bon.Clone());
            break;
        }
        case 3: {
            CMagazine bon(coord);
            bonus.push_back(bon.Clone());
            break;
        }
        case 4: {
            CHealth bon(coord);
            bonus.push_back(bon.Clone());
            break;
        }
        case 5: {
            CReloading bon(coord);
            bonus.push_back(bon.Clone());
            break;
        }
        case 6: {
            CDamage bon(coord);
            bonus.push_back(bon.Clone());
            break;
        }
    }
}

