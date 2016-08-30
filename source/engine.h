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
		void add_obstacle_bushes();
		bool check_bushes_parameters();
	
	private:
		Sketcher m_artist;
		SpaceSheep* sheep;
		std::vector<RectObstacle*> bushes;

		unsigned int fatness = 2;
		unsigned int production = 14; //set vertical frequency of obstacle
		unsigned int dt_uint_obstacle = 150;
		unsigned int w_d = 4; //margin between bushes distance and sheep width
		unsigned int w_tot = 50; //minimal total width of bushes on the same row
		unsigned int w_m = 20; //minimal width of bushes
		unsigned int w_r = 22; //range of width, w_m+(w_r-1)=maximum width
		unsigned int h_m = 3; //minimal height of bushes
		unsigned int h_r = 4; //range of height	
		
		unsigned int dt_uint_sheep = 10; 
		unsigned int score = 0;
		char left_mov = 'j', right_mov = 'l';
		char pause = 'p';
};

#endif // _ENGINE_H_
