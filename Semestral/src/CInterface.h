//
// Created by Dominik Alfery on 09.05.2021.
//

#pragma once

#include <string>
#include <map>
#include <set>

#include <ncurses.h>

#include "EDirection.h"
#include "CCoord.h"
#include "CScoreboard.h"

/**
 * class representing an Interface
 */
class CInterface {
private:
    /** @var main window */
    WINDOW *mainwin;
    /** @var main window maximal height */
    int maxHeight;
    /** @var main window maximal width */
    int maxWidth;
    /** @var represents scoreboard */
    CScoreboard myScoreboard;

public:

    /**
     * Constructor that starts ncurses mode
     */
    CInterface();

    /**
     * display welcoming window
     */
    void StartDisplay() const;

    /**
     * draw Menu and describing text
     * @param stage capture last chosen entry
     */
    void DrawMenu(int stage) const;

    /**
     * draw loaded game map
     * @param gamewin where to display map
     * @param mapDATA what to display
     */
    void DrawMap(WINDOW *gamewin, const std::map<unsigned, std::string> &mapDATA) const;

    /**
    * draw infor bonus window and text
    */
    void DrawBonusInfo() const;

    /**
     * manage menu movement and selection
     * @return
     */
    int MakeMenu() const;

    /**
     * erase and refresh window
     * @param win what window to erase
     */
    void eraseWIN(WINDOW *win) const;

    /**
     *  draw text for info game panel
     * @param infowin info panel window
     */
    void DrawInfoDisplay(WINDOW *infowin) const;

    /**
     * draw before game start info
     * @param gamewin in which window to draw
     */
    void BeforeGameInfo(WINDOW *gamewin) const;

    /**
     * draw and display character's information
     * @param infowin where to draw
     * @param HP health number
     * @param Ammo ammo number
     * @param Score gained score
     * @param Bonus which bonus is at disposal
     * @param Level current Level
     */
    void
    UpdateInfoDisplay(WINDOW *infowin, int HP, std::pair<unsigned, unsigned> Ammo, int Score, const std::string &Bonus,
                      int Level) const;

    /**
     * print one character at defined position
     * @param gamewin in which window to draw
     * @param coord draw coordinates
     * @param ch    what char to draw
     */
    void PrintCharacter(WINDOW *gamewin, const CCoord &coord, const char &ch) const;

    /**
     * print bullet special character at defined position
     * @param gamewin in which window to draw
     * @param coord draw coordinates
     */
    void PrintBullet(WINDOW *gamewin, const CCoord &coord) const;

    /**
     * print bonus special character at defined position
     * @param gamewin in which window to draw
     * @param coord draw coordinates
     */
    void PrintBonus(WINDOW *gamewin, const CCoord &coord) const;

    /**
     * method which manages during game inputs
     * @param gamewin for which window to control input
     * @return EDirection enum based on clicked key
     */
    EDirection getInput(WINDOW *gamewin) const;

    /**
     * print Game Over window and text wiht achieved score
     * @param gamewin in which window to draw
     * @param score achieved score
     */
    void PrintGameOver(WINDOW *gamewin, const unsigned score) const;

    /**
     * print scoreboard window with TOP 10 achieved score
     * @param scoreset STL set container containing ordered TOP 10 score
     */
    void PrintScoreboard(const std::set<int, std::greater<>> &scoreset) const;

};