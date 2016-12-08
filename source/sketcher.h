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

class Sketcher
{
	public:
		Sketcher(unsigned int xDim, unsigned int yDim);
		~Sketcher(){}

		void game_table(); //print game table limits
		char welcome_screen(); //print welcome screen and return user choice
		bool pause_screen(); //print pause screen
		//Receive owner's IP addr from user input, tell user which is the
		// right port to open and print error if present
		std::string addr_input_screen(	std::string owner,
										unsigned int default_port,
										std::string error = "");
		bool pair_screen(); //print pair screen
		bool exit_good_screen(unsigned int score); //print exit screen with score
		bool exit_evil_screen(); //print exit screen
		void score(unsigned int score); //print actual score on Game screen
		void creator_choice(); //print bull creation keybinds

		void pencil(RectObstacle* bush); //print bushes
		void pencil(SpaceSheep* sheep); //print sheep
		void pencil(SpaceBull* bull); //print bull

		void rubber(RectObstacle* bush); //erase bushes
		void rubber(CircleObstacle* circle); //erase circle

		void animation(RectObstacle* bush); //animate bush movement
		void animation(SpaceSheep* sheep, char dir); //animate sheep movement
		void animation(SpaceSheep* sheep, unsigned int x); //animate sheep movement
		void animation(SpaceBull* bull); //animate bull movement

		unsigned int get_gameW() { return m_gameW; }
		unsigned int get_gameH() { return m_gameH; }

	private:
		unsigned int m_xDim, m_yDim; // size of the entire game table
		unsigned int m_xOffset, m_yOffset;
		unsigned int m_gameW, m_gameH; // size of internal playground
};

#endif // _SKETCHER_H_
