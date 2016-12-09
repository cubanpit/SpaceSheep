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
 *	Martina Crippa 				<martina.crippa2@studenti.unimi.it>
 *	Pietro Francesco Fontana 	<pietrofrancesco.fontana@studenti.unimi.it>
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

#include <ncurses.h>
#include <string> // std::string, std::to_string
#include <chrono> // std::chrono::system_clock
#include <thread> // std::this_thread::sleep_until
#include "obstacle.h"
#include "prizegive.h"

class Sketcher
{
	public:
		Sketcher(unsigned int xDim, unsigned int yDim);
		~Sketcher(){}

		void game_table() const; //print game table limits
		char welcome_screen() const; //print welcome screen and return user choice
		bool pause_screen() const; //print pause screen
		//Receive owner's IP addr from user input, tell user which is the
		// right port to open and print error if present
		std::string addr_input_screen(	std::string owner,
										unsigned int default_port,
										std::string error = "") const;
		bool pair_screen() const; //print pair screen
		bool exit_local_screen(unsigned int score) const;//exit screen with score
		bool exit_good_screen(unsigned int score) const;//exit screen with score
		bool exit_evil_screen() const; //print exit screen
		void score(unsigned int score) const;//print actual score on screen
		void creator_choice() const; //print bull creation keybinds

		// print obstacles
		void pencil(RectObstacle* bush) const;
		void pencil(SpaceSheep* sheep) const;
		void pencil(SpaceBull* bull) const;

		// erase obstacles
		void rubber(RectObstacle* bush) const;
		void rubber(CircleObstacle* circle) const;

		// function to animate obstacles in different ways
		void animation(RectObstacle* bush) const; //drop bush
		void animation(SpaceSheep* sheep, char dir) const; //move 'r' or 'l'
		void animation(SpaceSheep* sheep, unsigned int x) const; //move to 'x'
		void animation(SpaceBull* bull) const; //drop bull

		unsigned int get_gameW() const { return m_gameW; }
		unsigned int get_gameH() const { return m_gameH; }

	private:
		unsigned int m_xDim, m_yDim; // size of the entire game table
		unsigned int m_xOffset, m_yOffset;
		unsigned int m_gameW, m_gameH; // size of internal playground
};

#endif // _SKETCHER_H_
