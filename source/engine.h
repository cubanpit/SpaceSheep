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

//max time the online game waits for a message from the opponent in ms
const unsigned short int _Engine_h_MAX_WAIT = 1000;
//max radius/fatness for bull and sheep
const unsigned short int _Engine_h_MAX_FATNESS = 5;

class Engine
{
	public:
		Engine(	unsigned int xDim = 110,
				unsigned int yDim = 34,
				unsigned int fatness = 2,
				unsigned int bushes_prod = 15,
				unsigned int dt_uint_bull = 50,
				unsigned int dt_uint_bushes = 175);
		~Engine();

		void start(); //start the game!
		bool run_local(); //play offline
		bool run_good(); //play online as good
		bool run_evil(); //play online as evil

		//Add bushes to m_bushes vector checking position and size
		//If bushes paramteres are not checked, it can fall in a loop
		void add_obstacle_bushes();
		//check if bushes parameters can live together without troubles (loops)
		bool check_bushes_parameters() const;
		//receive input from user to create a bull in a given position
		bool bull_creator_choice();
		//pair opponents though network, return false if user wants to quit
		bool pair_with_good() const;
		bool pair_with_evil() const;
		//create a bull reading a message:[-,posX,posY,radius,-]
		bool create_bull(std::vector<char>& msg);

		//change bushes parameters: size or density limit
		void set_bushes_properties(	unsigned int bushes_w_d = 4,
									unsigned int bushes_w_tot = 55,
									unsigned int bushes_w_m = 20,
									unsigned int bushes_w_r = 20,
									unsigned int bushes_h_m = 3,
									unsigned int bushes_h_r = 4);
		//Change movement keys or speed
		void set_movement_properties(	unsigned int dt_uint_sheep = 5,
										char left_mov = 'j',
										char right_mov = 'l');


	private:
		Sketcher m_artist;
		SpaceSheep* m_sheep;
		SpaceBull* m_bull;
		std::vector<RectObstacle*> m_bushes;

		UDPSSMcastSender* m_sender;
		UDPSSMcastReceiver* m_recver;
		std::string m_my_ip_addr;
		std::string m_opp_ip_addr;

		//sheep fatness (aka radius)
		unsigned int m_fatness = 2;
		//vertical space frequency (production) of obstacle
		unsigned int m_bushes_prod = 15;
		//time interval of bull movement (ms)
		unsigned int m_dt_uint_bull = 100;
		//time interval of bushes movement (ms)
		unsigned int m_dt_uint_bushes = 175;

		//margin between bushes distance and sheep width
		unsigned int m_bushes_w_d = 4;
		//minimal total width of bushes on the same row
		unsigned int m_bushes_w_tot = 55;
		//minimal width of bushes
		unsigned int m_bushes_w_m = 20;
		//range of width, w_m+(w_r-1)=maximum width
		unsigned int m_bushes_w_r = 20;
		//minimal height of bushes
		unsigned int m_bushes_h_m = 3;
		//range of height, h_m+(h_r-1)=maximum height
		unsigned int m_bushes_h_r = 4;

		//time interval of sheep movement (ms)
		unsigned int m_dt_uint_sheep = 5;
		unsigned int m_score = 0;
		char m_left_mov = 'j', m_right_mov = 'l';
		char m_pause = 'p';
};

#endif // _ENGINE_H_
