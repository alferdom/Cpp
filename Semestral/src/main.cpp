//
// Created by Dominik Alfery on 05.06.2021.
//

#include "CApplication.h"
#include <csignal>
#include <ncurses.h>

void ctrc_handler(int signum) {
    erase();
    int x, y;
    getmaxyx(stdscr, y, x);
    mvaddstr(y / 2, x / 2 - 12, "CTR + C has been used!");
    refresh();
    getch();
    delwin(stdscr);
    endwin();
    exit(signum);
}

int main() {
    signal(SIGINT, ctrc_handler);
    CInterface myInterface;
    CApplication myApp(myInterface);

    myApp.Run();

    return 0;
}