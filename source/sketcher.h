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

#ifndef _SKE_H_
#define _SKE_H_

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

		void GameTable(); //print game table limits
		char WelcomeScreen(); //print welcome screen and return user choice
		bool PauseScreen(); //print pause screen
		bool PairScreen(); //print pair screen
		bool ExitScreen(unsigned int score); //print exit screen
		void Score(unsigned int score); //print actual score on Game screen

		void Pencil(RectObstacle* bush); //print bushes
		void Pencil(SpaceSheep* sheep); //print sheep
		void Pencil(SpaceBull* bull); //print bull

		void Rubber(RectObstacle* bush); //erase bushes
		void Rubber(CircleObstacle* circle); //erase circle

		void Animation(RectObstacle* bush); //animate bush movement
		void Animation(SpaceSheep* sheep, char dir); //animate sheep movement
		void Animation(SpaceSheep* sheep, unsigned int x); //animate sheep movement
		void Animation(SpaceBull* bull); //animate bull movement

		unsigned int get_GameW() { return M_GameW; }
		unsigned int get_GameH() { return M_GameH; }

	private:
		unsigned int M_xDim, M_yDim; // size of the entire game table
		unsigned int M_xOffset, M_yOffset;
		unsigned int M_GameW, M_GameH; // size of internal playground
};

#endif // _SKE_H_
