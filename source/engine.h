/*
 * SpaceSheep, ncurses game. Code is hosted on GitHub.
 *
 * File: engine.h
 *
 * This header declares classes needed to handle game run.
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

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <iostream> //std::cerr
#include <random> // std::default_random_engine , std::uniform_int_distribution
#include <vector> //std::vector
#include "sketcher.h"
#include "UDPSSMcast.h"

class Engine
{
	public:
		Engine(unsigned int xDim = 100,
			   unsigned int yDim = 30,
		       unsigned int n_fatness = 2,
	    	   unsigned int n_bushes_prod = 14,
			   unsigned int n_bull_prod = 10,
			   unsigned int n_dt_uint_bushes = 1500);
		~Engine(){}

		void run_local(); //play local mode
		void run_good(); //play as good sheep versus the evil bull creator
		void run_evil(); //play as evil bull creator versus the good sheep

		void add_obstacle_bushes();
		bool check_bushes_parameters();

		void set_bushes_properties(	unsigned int n_bushes_w_d = 4,
									unsigned int n_bushes_w_tot = 50,
									unsigned int n_bushes_w_m = 20,
									unsigned int n_bushes_w_r = 20,
									unsigned int n_bushes_h_m = 3,
									unsigned int n_bushes_h_r = 4);
		void set_movement_properties(	unsigned int n_dt_uint_sheep = 10,
										char n_left_mov = 'j',
										char n_right_mov = 'l');


	private:
		SpaceSheep* sheep;
		SpaceBull* bull;
		Sketcher m_artist;
		
		std::vector<RectObstacle*> bushes;

		unsigned int fatness; //set sheep fatness
		unsigned int bushes_prod; //set vertical frequency (production) of obstacle
		unsigned int bull_prod; //set bull speed
		unsigned int dt_uint_bushes; //set time frequency of bushes movement

		unsigned int bushes_w_d = 4; //margin between bushes distance and sheep width
		unsigned int bushes_w_tot = 50; //minimal total width of bushes on the same row
		unsigned int bushes_w_m = 20; //minimal width of bushes
		unsigned int bushes_w_r = 20; //range of width, w_m+(w_r-1)=maximum width
		unsigned int bushes_h_m = 3; //minimal height of bushes
		unsigned int bushes_h_r = 4; //range of height

		unsigned int dt_uint_sheep = 10; //maybe milliseconds (tmp)
		unsigned int score = 0;
		char left_mov = 'j', right_mov = 'l';
		char pause = 'p';
};

#endif // _ENGINE_H_
