/*
 * SpaceSheep, ncurses game. Code is hosted on GitHub.
 *
 * File: sketcher.h
 *
 * This class should draw every object in the game table.
 * It handle also the erase and the animation of object, the welcome screen and
 *  the other screens of the game.
 *
 * Authors:
 *  Martina Crippa             <martina.crippa2@studenti.unimi.it>
 *  Pietro Francesco Fontana   <pietrofrancesco.fontana@studenti.unimi.it>
 *
 *******************************************************************************
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *******************************************************************************
 */

#ifndef _SKETCHER_H_
#define _SKETCHER_H_

// neeeded for compilation on old system (tolab)
#ifndef _GLIBCXX_USE_NANOSLEEP
#define _GLIBCXX_USE_NANOSLEEP
#endif

#include <ncurses.h> // initsrc, endwin, curs_set, echo/noecho, timeout,
// mvprintw, erase, getch, getstr, attron/attroff, init_pair
#include <string> // std::string, std::to_string
#include <chrono> // std::chrono::system_clock
#include <thread> // std::this_thread::sleep_until

#include "obstacle.h"
#include "prizegive.h"

// Under these values some screens are ugly
const unsigned short int _Sketcher_h_MIN_HEIGHT_ = 28;
const unsigned short int _Sketcher_h_MIN_WIDTH_ = 80;

// Over this value some variables overflow
const unsigned short int _Sketcher_h_MAX_HEIGHT_ = 127;
const unsigned short int _Sketcher_h_MAX_WIDTH_ = 123;

class Sketcher
{
  public:
    Sketcher(unsigned int xDim, unsigned int yDim);
    ~Sketcher(){}

    //draw game table borders
    void game_table() const;

    //user can choice, welcome_screen() returns:
    // 'q' to exit the game, 'n' to play offline
    // 'g' to play online as good, 'e' to play online as evil
    char welcome_screen() const;
    //return 'false' if user wants to resume the game
    bool pause_screen() const;

    //Receive owner's IP addr from user input, tell user which is the
    // right port to open and print error if present
    std::string addr_input_screen(
        std::string owner, unsigned int default_port,
        std::string error = "") const;
    //print simple pair screen, return true if user choose to cancel
    bool pair_screen() const;

    //Print exit screens for different game style, return 'false' if user
    // wants to exit the game.
    bool exit_local_screen(unsigned int score) const;
    bool exit_good_screen(unsigned int score) const;
    bool exit_evil_screen() const;
    bool exit_lost_connection() const;

    //update actual score on game table border
    void update_score(unsigned int score) const;
    //print bull creation keybindings on game table border
    void creator_choice() const;

    // print obstacles
    void pencil(RectObstacle* bush) const;
    void pencil(SpaceSheep* sheep) const;
    void pencil(SpaceBull* bull) const;

    // erase obstacles
    void rubber(RectObstacle* bush) const;
    void rubber(CircleObstacle* circle) const;

    // animate obstacles in different ways
    void animation(RectObstacle* bush) const; //drop bush
    void animation(SpaceSheep* sheep, char dir) const; //move 'r' or 'l'
    void animation(SpaceSheep* sheep, unsigned int x) const; //move to 'x'
    void animation(SpaceBull* bull) const; //drop bull

    unsigned int get_gameW() const { return m_gameW; }
    unsigned int get_gameH() const { return m_gameH; }

  private:
    unsigned int m_xDim, m_yDim; // size of the entire game table
    unsigned int m_xOffset, m_yOffset; // offset inside terminal window
    unsigned int m_gameW, m_gameH; // size of internal playground
};

#endif // _SKETCHER_H_

// vim: set expandtab ts=2 sw=2:
