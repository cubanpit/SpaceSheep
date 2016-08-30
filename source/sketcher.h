/*
 * File: sketcher.h
 *
 * This class should draw every object in the game table.
 * It handle also the erase and the animation of object.
*/

#ifndef _SKE_H_
#define _SKE_H_

#include <ncurses.h>
#include <iostream>
#include <string> //std::string, std::to_string
#include <chrono> // std::chrono::system_clock
#include <thread> // std::this_thread::sleep_until
#include "obstacle.h"

class Sketcher 
{
	public:
		Sketcher(unsigned int xDim, unsigned int yDim);
		~Sketcher(){}

		void GameTable(); //print game table limits
		void WelcomeScreen(); //print welcome screen
		bool PauseScreen(); //print pause screen
		bool ExitScreen(unsigned int score); //print exit screen
		void Score(unsigned int score); //print actual score on Game screen

		void Pencil(RectObstacle* bush); //print bushes
		void Pencil(SpaceSheep* sheep); //print sheep

		void Rubber(RectObstacle* bush); //erase bushes
		void Rubber(SpaceSheep* sheep); //erase sheep

		void Animation(RectObstacle* bush); //animate bush movement
		void Animation(SpaceSheep* sheep, bool dir); //animate sheep movement

		unsigned int get_yDim() { return M_yDim; }
		unsigned int get_xDim() { return M_xDim; }

	private:
		unsigned int M_xDim, M_yDim, M_xOffset, M_yOffset;	
};

#endif // _SKE_H_
