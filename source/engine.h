/*
 * File: engine.h
 * 
 * This header declares classes needed to handle game run.
 */

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <iostream> //std::cerr
#include <random> // std::default_random_engine , std::uniform_int_distribution
#include <vector> //std::vector
#include "sketcher.h"

class Engine
{
	public:
		Engine(unsigned int xDim = 100, 
			   unsigned int yDim = 30,
		       unsigned int n_fatness = 2,
	    	   unsigned int n_bushes_prod = 14,
       		   unsigned int n_dt_uint_bushes = 150);
		~Engine(){}

		void run();
		void add_obstacle_bushes();
		bool check_bushes_parameters();
	
		void set_bushes_properties(	unsigned int n_bushes_w_d = 4,
									unsigned int n_bushes_w_tot = 50,
									unsigned int n_bushes_w_m = 20,
									unsigned int n_bushes_w_r = 22,
									unsigned int n_bushes_h_m = 3,
									unsigned int n_bushes_h_r = 4);
		void set_movement_properties(	unsigned int n_dt_uint_sheep = 10,
										char n_left_mov = 'j',
										char n_right_mov = 'l');

	private:
		Sketcher m_artist;
		SpaceSheep* sheep;
		std::vector<RectObstacle*> bushes;

		unsigned int fatness; //set sheep fatness
		unsigned int bushes_prod; //set vertical frequency (production) of obstacle
		unsigned int dt_uint_bushes; //set time frequency of bushes movement

		unsigned int bushes_w_d = 4; //margin between bushes distance and sheep width
		unsigned int bushes_w_tot = 50; //minimal total width of bushes on the same row
		unsigned int bushes_w_m = 20; //minimal width of bushes
		unsigned int bushes_w_r = 22; //range of width, w_m+(w_r-1)=maximum width
		unsigned int bushes_h_m = 3; //minimal height of bushes
		unsigned int bushes_h_r = 4; //range of height	
		
		unsigned int dt_uint_sheep = 10; 
		unsigned int score = 0;
		char left_mov = 'j', right_mov = 'l';
		char pause = 'p';
};

#endif // _ENGINE_H_
