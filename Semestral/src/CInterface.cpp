//
// Created by Dominik Alfery on 05.06.2021.
//
#include <cstring>
#include <set>

#include "CInterface.h"
#include "CGame.h"

CInterface::CInterface() : maxHeight(15), maxWidth(60), myScoreboard() {
    initscr();/* Start curses mode */
    refresh();
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    cbreak();
    curs_set(0);
    mainwin = newwin(15, 60, 0, 0);
}

void CInterface::StartDisplay() const {
    wborder(mainwin, 0, 0, 0, 0, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

    mvwaddstr(mainwin, (maxHeight / 2) - 1, maxWidth / 2 - 13, "Welcome to Wolf and Stein!");
    mvwaddstr(mainwin, (maxHeight / 2) + 1, maxWidth / 2 - 10, "Click E to enter");
    wrefresh(mainwin);
    char enter;
    while ((enter = getch())) {
        if (enter == 'e' || enter == 'E') {
            eraseWIN(mainwin);
            break;
        }
    }
}

void CInterface::DrawMenu(int stage) const {
    wborder(mainwin, 0, 0, 0, 0, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    mvwaddstr(mainwin, (maxHeight / 2) - 4, maxWidth / 2 - 4, "New Game");
    mvwaddstr(mainwin, (maxHeight / 2) - 2, maxWidth / 2 - 5, "Scoreboard");
    mvwaddstr(mainwin, (maxHeight / 2), maxWidth / 2 - 5, "Bonus info");
    mvwaddstr(mainwin, (maxHeight / 2) + 2, maxWidth / 2 - 2, "Quit");
    switch (stage) {
        default:
            wattron(mainwin, A_STANDOUT);
            mvwaddstr(mainwin, (maxHeight / 2) - 4, maxWidth / 2 - 4, "New Game");
            break;
        case 1:
            wattron(mainwin, A_STANDOUT);
            mvwaddstr(mainwin, (maxHeight / 2) - 2, maxWidth / 2 - 5, "Scoreboard");
            break;
        case 2:
            wattron(mainwin, A_STANDOUT);
            mvwaddstr(mainwin, (maxHeight / 2), maxWidth / 2 - 5, "Bonus info");
            break;
        case 3:
            wattron(mainwin, A_STANDOUT);
            mvwaddstr(mainwin, (maxHeight / 2) + 2, maxWidth / 2 - 2, "Quit");
            break;
    }
    wattroff(mainwin, A_STANDOUT);
    mvwaddstr(mainwin, 3, 1, "SELECT with E");
    mvwaddstr(mainwin, 5, 1, "Move UP with W or ");
    waddch(mainwin, ACS_UARROW);
    mvwaddstr(mainwin, 7, 1, "Move DOWN with S or ");
    waddch(mainwin, ACS_DARROW);
    wrefresh(mainwin);
}

void moveStandout(WINDOW *mainwin, int lines, int cols, const char *str1, const char *str2, int dir) {
    if (dir == -1) {
        mvwaddstr(mainwin, lines, cols - strlen(str1) / 2, str1);
        wattron(mainwin, A_STANDOUT);
        mvwaddstr(mainwin, lines + 2, cols - strlen(str2) / 2, str2);
    } else if (dir == 1) {
        mvwaddstr(mainwin, lines, cols - strlen(str1) / 2, str1);
        wattron(mainwin, A_STANDOUT);
        mvwaddstr(mainwin, lines - 2, cols - strlen(str2) / 2, str2);
    } else {
        if (dir == 2) {
            mvwaddstr(mainwin, lines - 6, cols - strlen(str1) / 2, str1);
        } else {
            mvwaddstr(mainwin, lines + 6, cols - strlen(str1) / 2, str1);
        }
        wattron(mainwin, A_STANDOUT);
        mvwaddstr(mainwin, lines, cols - strlen(str2) / 2, str2);
    }
    wattroff(mainwin, A_STANDOUT);
    wrefresh(mainwin);
}


int CInterface::MakeMenu() const {
    int choose, stage = 0;
    DrawMenu(stage);
    std::set<int, std::greater<>> scoreset = myScoreboard.readScore();

    while ((choose = getch())) {

        switch (choose) {
            default:
                break;
            case KEY_DOWN:
            case 's':
                switch (stage) {
                    default:
                        moveStandout(mainwin, (maxHeight / 2) - 4, maxWidth / 2, "New Game", "Scoreboard", -1);
                        stage++;
                        break;
                    case 1:
                        moveStandout(mainwin, (maxHeight / 2) - 2, maxWidth / 2, "Scoreboard", "Bonus info", -1);
                        stage++;
                        break;
                    case 2:
                        moveStandout(mainwin, (maxHeight / 2), maxWidth / 2, "Bonus info", "Quit", -1);
                        stage++;
                        break;
                    case 3:
                        moveStandout(mainwin, (maxHeight / 2) - 4, maxWidth / 2, "Quit", "New Game", 0);
                        stage = 0;
                        break;
                }
                break;
            case KEY_UP:
            case 'w':
                switch (stage) {
                    default:
                        moveStandout(mainwin, (maxHeight / 2) + 2, maxWidth / 2, "New Game", "Quit", 2);
                        stage = 3;
                        break;
                    case 1:
                        moveStandout(mainwin, (maxHeight / 2) - 2, maxWidth / 2, "Scoreboard", "New Game", 1);
                        stage--;
                        break;
                    case 2:
                        moveStandout(mainwin, (maxHeight / 2), maxWidth / 2, "Bonus info", "Scoreboard", 1);
                        stage--;
                        break;
                    case 3:
                        moveStandout(mainwin, (maxHeight / 2) + 2, maxWidth / 2, "Quit", "Bonus info", 1);
                        stage--;
                        break;
                }
                break;
            case 'e':
            case 'E':
                switch (stage) {
                    default: {
                        eraseWIN(mainwin);
                        CGame myGame(*this, CMapBuilder());
                        if (!myGame.Start()) {
                            mvwaddstr(mainwin, 5, 5, "ERROR MapFile is missing");
                            mvwaddstr(mainwin, 7, 5, "Press KEY to exit");
                            wrefresh(mainwin);
                            getch();
                            delwin(mainwin);
                            endwin();
                            return -1;
                        }
                        scoreset = myScoreboard.writeScore(myGame.getScore());
                        touchwin(mainwin);
                        DrawMenu(stage);
                        break;
                    }
                    case 1:
                        eraseWIN(mainwin);
                        PrintScoreboard(scoreset);
                        touchwin(mainwin);
                        DrawMenu(stage);
                        break;
                    case 2:
                        eraseWIN(mainwin);
                        DrawBonusInfo();
                        touchwin(mainwin);
                        DrawMenu(stage);
                        break;
                    case 3:
                        delwin(mainwin);
                        endwin();
                        return 1;
                        break;
                }
        }
    }
    return 0;
}

void CInterface::DrawInfoDisplay(WINDOW *infowin) const {
    box(infowin, ACS_VLINE, ACS_HLINE);
    mvwaddstr(infowin, 2, 2, "HEALTH:");
    mvwaddstr(infowin, 4, 2, "AMMO:");
    mvwaddstr(infowin, 6, 2, "SCORE:");
    mvwaddstr(infowin, 8, 2, "BONUS:");
    mvwaddstr(infowin, 10, 2, "LEVEL:");
    wrefresh(infowin);
}

void CInterface::DrawMap(WINDOW *gamewin, const std::map<unsigned, std::string> &mapDATA) const {
    for (auto const &i: mapDATA) {
        mvwaddstr(gamewin, i.first + 1, 1, i.second.c_str());
    }
    wrefresh(gamewin);
}

void CInterface::DrawBonusInfo() const {
    WINDOW *bonuswin = newwin(14, 40, 1, 2);
    wborder(bonuswin, '&', '&', '#', '#', 0, 0, 0, 0);
    mvwaddstr(bonuswin, 2, 2, "Ammo: add 20 bullets");
    mvwaddstr(bonuswin, 4, 2, "Magazine: add 5 bullets to mag");
    mvwaddstr(bonuswin, 6, 2, "Health: 25 of HP");
    mvwaddstr(bonuswin, 8, 2, "Reloading: decrease reloading speed");
    mvwaddstr(bonuswin, 10, 2, "Damage: deal more damage");
    wrefresh(bonuswin);
    getch();
    eraseWIN(bonuswin);
    delwin(bonuswin);
}

void CInterface::eraseWIN(WINDOW *win) const {
    werase(win);
    wrefresh(win);
}

void CInterface::PrintCharacter(WINDOW *gamewin, const CCoord &coord, const char &ch) const {
    mvwaddch(gamewin, coord.m_Y, coord.m_X, ch);
}

void CInterface::PrintBullet(WINDOW *gamewin, const CCoord &coord) const {
    mvwaddch(gamewin, coord.m_Y, coord.m_X, ACS_BULLET);
}

void CInterface::PrintBonus(WINDOW *gamewin, const CCoord &coord) const {
    mvwaddch(gamewin, coord.m_Y, coord.m_X, ACS_DIAMOND);
}

void
CInterface::UpdateInfoDisplay(WINDOW *infowin, const int HP, const std::pair<unsigned, unsigned> Ammo, const int Score,
                              const std::string &Bonus, const int Level) const {
    werase(infowin);
    box(infowin, ACS_VLINE, ACS_HLINE);
    mvwprintw(infowin, 2, 2, "HEALTH: %d", HP);
    mvwprintw(infowin, 4, 2, "AMMO: %u / %u", Ammo.first, Ammo.second);
    mvwprintw(infowin, 6, 2, "SCORE: %d", Score);
    mvwprintw(infowin, 8, 2, "BONUS: %s", Bonus.c_str());
    mvwprintw(infowin, 10, 2, "LEVEL: %d", Level);
    if (!Ammo.first) {
        mvwaddstr(infowin, 14, 2, "RELOADING");
    } else {
        mvwaddstr(infowin, 14, 2, "         ");
    }
    wrefresh(infowin);
}

EDirection CInterface::getInput(WINDOW *gamewin) const {
    int key;
    while ((key = wgetch(gamewin))) {
        switch (key) {
            default:
                return EDirection::NONE;
            case 'w':
                return EDirection::UP;
            case 's':
                return EDirection::DOWN;
            case 'a':
                return EDirection::LEFT;
            case 'd':
                return EDirection::RIGHT;
            case 'f':
                return EDirection::SHOOT;
            case 'r':
                return EDirection::USE;
            case 27:
                return EDirection::QUIT;
        }
    }
    return EDirection::NONE;
}

void CInterface::PrintGameOver(WINDOW *gamewin, const unsigned score) const {
    eraseWIN(gamewin);
    int width, height;
    getmaxyx(gamewin, height, width);
    wattron(gamewin, A_UNDERLINE);
    mvwaddstr(gamewin, height / 2 - 2, width / 2 - 5, "GAME OVER");
    mvwprintw(gamewin, height / 2, width / 2 - 5, "SCORE: %u", score);
    wattroff(gamewin, A_UNDERLINE);
    wrefresh(gamewin);
}

void CInterface::PrintScoreboard(const std::set<int, std::greater<>> &scoreset) const {
    WINDOW *scorewin = newwin(25, 25, 1, 2);
    wborder(scorewin, '$', '$', '$', '$', '*', '*', '*', '*');
    wattron(scorewin, A_UNDERLINE);
    mvwaddstr(scorewin, 2, 6, "TOP 10 SCORE:");
    wattroff(scorewin, A_UNDERLINE);
    unsigned pos = 1;

    for (const auto &it : scoreset) {
        mvwprintw(scorewin, pos + 3 + pos - 1, 6, "%u. %d", pos, it);
        pos++;
    }
    wrefresh(scorewin);
    getch();
    eraseWIN(scorewin);
    delwin(scorewin);
}

void CInterface::BeforeGameInfo(WINDOW *gamewin) const {
    int x, y;
    getmaxyx(gamewin, y, x);
    wattron(gamewin, A_BLINK | A_UNDERLINE);
    mvwaddstr(gamewin, 8, x / 2 - 9, "PRESS KEY TO START");
    wattroff(gamewin, A_BLINK | A_UNDERLINE);
    mvwaddstr(gamewin, y / 2, x / 2 - 6, "WASD TO MOVE");
    mvwaddstr(gamewin, y / 2 + 1, x / 2 - 5, "F TO SHOOT");
    mvwaddstr(gamewin, y / 2 + 2, x / 2 - 7, "R TO USE BONUS");
    mvwaddstr(gamewin, y / 2 + 3, x / 2 - 5, "ESC TO QUIT");
    wrefresh(gamewin);

    getch();
    eraseWIN(gamewin);
}

