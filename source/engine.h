/*
 * File: engine.h
 * 
 * This header declares classes needed to handle game run.
 */

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <random> // std::default_random_engine , std::uniform_int_distribution
#include <vector> //std::vector
#include "sketcher.h"

class Engine
{
	public:
		Engine(unsigned int xDim = 100, unsigned int yDim = 30);
		~Engine(){}

		void run();
	
	private:
		Sketcher m_artist;


};

#endif // _ENGINE_H_
