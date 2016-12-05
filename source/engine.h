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
		Engine(	unsigned int xDim = 100,
				unsigned int yDim = 30,
				unsigned int fatness = 2,
				unsigned int bushes_prod = 14,
				unsigned int dt_uint_bull = 200,
				unsigned int dt_uint_bushes = 350);
		~Engine();

		void start(); //start the game!
		void run_local(); //play local mode
		void run_good(); //play as good sheep versus the evil bull creator
		void run_evil(); //play as evil bull creator versus the good sheep

		void add_obstacle_bushes(); //add a bush to existent bushes vector
		bool check_bushes_parameters(); //check if bushes param. are legal
		bool bull_creator_choice(); //receive input from creator to spawn a bull
		void pair_with_opponent(); //pair opponents through network

		//change bushes size or density limit
		void set_bushes_properties(	unsigned int bushes_w_d = 4,
									unsigned int bushes_w_tot = 50,
									unsigned int bushes_w_m = 20,
									unsigned int bushes_w_r = 20,
									unsigned int bushes_h_m = 3,
									unsigned int bushes_h_r = 4);
		//change movement keys or speed
		void set_movement_properties(	unsigned int dt_uint_sheep = 10,
										char left_mov = 'j',
										char right_mov = 'l');


	private:
		Sketcher m_artist;
		SpaceSheep* m_sheep;
		SpaceBull* m_bull;
		std::vector<RectObstacle*> m_bushes;
		UDPSSMcastSender* m_sender;
		UDPSSMcastReceiver* m_recver;

		unsigned int m_fatness; //sheep fatness
		unsigned int m_bushes_prod; //vertical space frequency (production) of obstacle
		unsigned int m_dt_uint_bull; //time interval of bull movement (ms)
		unsigned int m_dt_uint_bushes; //time interval of bushes movement (ms)

		unsigned int m_bushes_w_d = 4; //margin between bushes distance and sheep width
		unsigned int m_bushes_w_tot = 50; //minimal total width of bushes on the same row
		unsigned int m_bushes_w_m = 20; //minimal width of bushes
		unsigned int m_bushes_w_r = 20; //range of width, w_m+(w_r-1)=maximum width
		unsigned int m_bushes_h_m = 3; //minimal height of bushes
		unsigned int m_bushes_h_r = 4; //range of height

		unsigned int m_dt_uint_sheep = 10; //time interval of sheep movement (ms)
		unsigned int m_score = 0;
		char m_left_mov = 'j', m_right_mov = 'l';
		char m_pause = 'p';
};

#endif // _ENGINE_H_
