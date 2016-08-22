/*
 * File: sketcher.h
 *
 * This class should draw every object in the game table.
 * It handle also the erase and the animation of object.
 *
 * TODO:
 * - clean GameTable and outside automatically;
*/

#ifndef _SKE_H_
#define _SKE_H_

#include <ncurses.h>
#include <iostream>
#include "obstacle.h"

class Sketcher 
{
	public:
		Sketcher(unsigned int xDim, unsigned int yDim);
		~Sketcher(){};

		void GameTable(); //print game table limits

		void Artist(RectObstacle* bush); //print bushes
		void Artist(SpaceSheep* sheep); //print sheep

		void Rubber(RectObstacle* bush); //erase bushes
		void Rubber(SpaceSheep* sheep); //erase sheep

		void Animation(RectObstacle* bush); //animate bush movement
		void Animation(SpaceSheep* sheep, bool dir); //animate sheep movement

	private:
		unsigned int M_xDim, M_yDim, M_xOffset, M_yOffset;	
};

#endif // _SKE_H_
