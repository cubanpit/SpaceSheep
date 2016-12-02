/*
 * SpaceSheep, ncurses game. Code is hosted on GitHub.
 *
 * File: engine.cpp
 *
 * Implementation of classes and functions declared in engine.h
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

#include "engine.h"

Engine::Engine(unsigned int xDim, unsigned int yDim, unsigned int n_fatness,
		unsigned int n_bushes_prod, unsigned int n_bull_prod, unsigned int n_dt_uint_bushes):
	m_artist(xDim,yDim), fatness(n_fatness),
	bushes_prod(n_bushes_prod), bull_prod(n_bull_prod), dt_uint_bushes(n_dt_uint_bushes)
{
}

void Engine::start()
{
	char user_choice;
	user_choice = m_artist.WelcomeScreen();
    if ( user_choice == 'n' ) run_local();
	else if ( user_choice == 'e' ) run_evil();
	else if ( user_choice == 'g' ) run_good();
}

void Engine::run_local()
{
	timeout(0); // getch() does not wait for input [ncurses]
	erase();
	bushes.clear(); //clear vector, if it's not empty

	m_artist.GameTable();
	sheep = new SpaceSheep(m_artist.get_GameW()/2,m_artist.get_GameH()-1-fatness,fatness);
	m_artist.Pencil(sheep);
	char ch; //needed for sheep movement

	// TIME STUFF
	// WARNING: clocks goes on during execution, if you use sleep_until(20:00)
	//  for obstacle and sleep(19:00) for sheep, the second will be ignored,
	//  time has passed!
	// SOLUTION: reassign time_point before using it, if necessary.
	std::chrono::system_clock::time_point t_track_sheep = std::chrono::system_clock::now();
	std::chrono::duration<int,std::milli> dt_sheep(dt_uint_sheep);

	unsigned int count = 0;
	bool dead = false;
	bool new_game = true;

	if ( !check_bushes_parameters() ) {
		throw "Engine::run_local() ERROR: bad parameters for bushes, the game risks a loop.";
	}

	while (!dead) {
		if ( !(count%bushes_prod) ) {
			try {
				add_obstacle_bushes();
			}
			catch (std::bad_alloc& ba)
			{
				std::cerr << "Engine::add_obstacle_bushes() bad_alloc caught: " << ba.what() << std::endl;
				exit (EXIT_FAILURE);
			}
		}
		++count;
		//compute score, different weight for different difficulty level
		score = count*(200/(bushes_prod+(bushes_w_d*10)+(dt_uint_bushes/10)));

		for (auto it = bushes.begin(); it != bushes.end(); it++) {
			m_artist.Pencil(*it);
		}

		for (auto it = bushes.begin(); it != bushes.end(); it++) {
			m_artist.Animation(*it);
		}

		for (auto it = bushes.begin(); it != bushes.end(); it++) {
			if ( ((*(*it)).get_hitbox()).Overlap((*sheep).get_hitbox()) ) {
				dead = true;
				break;
			}
		}

		for (unsigned short int i=0; i<dt_uint_bushes and !dead; i+=dt_uint_sheep) {
			ch = getch();
			if ( ch == left_mov and  (((*sheep).get_ref()).x -
						(int)(*sheep).get_fatness()) > 1 ) {
				m_artist.Animation(sheep,'l');
			}
			else if ( ch == right_mov and (((*sheep).get_ref()).x +
						(int)(*sheep).get_fatness()) < ((int)m_artist.get_GameW() - 1) ) {
				m_artist.Animation(sheep,'r');
			}
			else if ( ch == pause ) {
				dead = m_artist.PauseScreen();
				if ( !dead ) {
					t_track_sheep = std::chrono::system_clock::now();
					m_artist.GameTable();
					for (auto it = bushes.begin(); it != bushes.end(); it++) {
						m_artist.Pencil(*it);
					}
					m_artist.Pencil(sheep);
				}
			}

			for (auto it = bushes.begin(); it != bushes.end(); it++) {
				if ( ((*(*it)).get_hitbox()).Overlap((*sheep).get_hitbox()) ) {
					dead = true;
					break;
				}
			}
			m_artist.Score(score);
			refresh();
			t_track_sheep += dt_sheep;
			std::this_thread::sleep_until(t_track_sheep);
		}
		refresh();
	}
	new_game = m_artist.ExitScreen(score);
	if ( new_game ) start();
}

void Engine::run_good()
{
	timeout(0); // getch() does not wait for input [ncurses]
	erase();
	bushes.clear(); //clear vector, if it's not empty

	UDPSSMcastSender sender("",_UDPMcastSender_h_DEFAULT_TTL,"127.0.0.1", 3263); //open socket
	UDPSSMcastReceiver recv("","127.0.0.1", 3264);

	char ch; //needed for sheep movement
	std::vector<char> message;
	bool paired = false;

	while(!paired) {
		if( m_artist.PairScreen() ) return;
		sender.send_msg("ping");
		if( recv.recv_msg() ){
			message.assign(recv.get_msg(), recv.get_msg()+_UDPSSMcast_h_DEFAULT_MSG_LEN);
			if( message[0] == 'p' and message[1] == 'o' and
				message[2] == 'n' and message[3] == 'g' ) {
				paired = true;
			}
		}
	}
	erase();
	m_artist.GameTable();
	refresh();

	sheep = new SpaceSheep(m_artist.get_GameW()/2,m_artist.get_GameH()-1-fatness,fatness);
	sender.send_msg(compose_msg(sheep));
	m_artist.Pencil(sheep);

	// TIME STUFF
	// WARNING: clocks goes on during execution, if you use sleep_until(20:00)
	//  for obstacle and sleep(19:00) for sheep, the second will be ignored,
	//  time has passed!
	// SOLUTION: reassign time_point before using it, if necessary.
	std::chrono::system_clock::time_point t_track_sheep = std::chrono::system_clock::now();
	std::chrono::duration<int,std::milli> dt_sheep(dt_uint_sheep);

	unsigned int count = 0;
	bool dead = false;
	bool new_game = true;
	bool got_bull = false;

	if ( !check_bushes_parameters() ) {
		throw "Engine::run_local() ERROR: bad parameters for bushes, the game risks a loop.";
	}

	while (!dead) {
		if ( !(count%bushes_prod) ) {
			try {
				add_obstacle_bushes();
			}
			catch (std::bad_alloc& ba)
			{
				std::cerr << "Engine::add_obstacle_bushes() bad_alloc caught: " << ba.what() << std::endl;
				exit (EXIT_FAILURE);
			}
		}
		++count;
		//compute score, different weight for different difficulty level
		score = count*(200/(bushes_prod+(bushes_w_d*10)+(dt_uint_bushes/10)));

		for (auto it = bushes.begin(); it != bushes.end(); it++) {
			m_artist.Animation(*it);
			sender.send_msg(compose_msg(*it));
		}

		for (auto it = bushes.begin(); it != bushes.end(); it++) {
			if ( ((*(*it)).get_hitbox()).Overlap((*sheep).get_hitbox()) ) {
				dead = true;
				break;
			}
		}

		for (unsigned short int i=0; i<dt_uint_bushes and !dead; i+=dt_uint_sheep) {
			ch = getch();
			if ( ch == left_mov and  (((*sheep).get_ref()).x -
						(int)(*sheep).get_fatness()) > 1 ) {
				m_artist.Animation(sheep,'l');
			}
			else if ( ch == right_mov and (((*sheep).get_ref()).x +
						(int)(*sheep).get_fatness()) < ((int)m_artist.get_GameW() - 1) ) {
				m_artist.Animation(sheep,'r');
			}
			else if ( ch == pause ) {
				dead = m_artist.PauseScreen();
				if ( !dead ) {
					t_track_sheep = std::chrono::system_clock::now();
					m_artist.GameTable();
					for (auto it = bushes.begin(); it != bushes.end(); it++) {
						m_artist.Pencil(*it);
					}
					m_artist.Pencil(sheep);
				}
			}

			for (auto it = bushes.begin(); it != bushes.end(); it++) {
				if ( ((*(*it)).get_hitbox()).Overlap((*sheep).get_hitbox()) ) {
					dead = true;
					break;
				}
			}

			if( got_bull and ((*bull).get_hitbox()).Overlap((*sheep).get_hitbox()) ) {
				dead = true;
				break;
			}

			m_artist.Score(score);
			if( recv.recv_msg() ) {
				for (auto it = bushes.begin(); it != bushes.end(); it++) {
					m_artist.Pencil(*it);
				}
				message.assign(recv.get_msg(), recv.get_msg()+_UDPSSMcast_h_DEFAULT_MSG_LEN);
				if( !got_bull ){
					bull = new SpaceBull(message[1] ,message[2], message[3]);
					got_bull = true;
					m_artist.Pencil(bull);
				}
				else {
					m_artist.Rubber(bull);
					delete bull;
					bull = new SpaceBull(message[1] ,message[2], message[3]);
					if( ((int) message[2] - (int) message[3]) > (int)m_artist.get_GameH() ) got_bull = false;
					else m_artist.Pencil(bull);
				}

				if( got_bull and ((*bull).get_hitbox()).Overlap((*sheep).get_hitbox()) ) {
					dead = true;
					break;
				}
			}

			// With this we are sure that the bull is over bushes, and they are
			//  always in sync.
			for (auto it = bushes.begin(); it != bushes.end(); it++) {
				m_artist.Pencil(*it);
			}
			if ( got_bull ) m_artist.Pencil(bull);
			refresh();

			sender.send_msg(compose_msg(sheep));
			t_track_sheep += dt_sheep;
			std::this_thread::sleep_until(t_track_sheep);
		}
		refresh();
	}
	new_game = m_artist.ExitScreen(score);
	if ( new_game ) start();
}

void Engine::run_evil()
{
	timeout(0);
	UDPSSMcastReceiver recv("","127.0.0.1", 3263);
	UDPSSMcastSender sender("",_UDPMcastSender_h_DEFAULT_TTL,"127.0.0.1", 3264); //open socket

	bool paired = false;
	std::vector<char> message;
	bushes.clear(); //clear vector, if it's not empty

	// This loop waits endlessly till the good guy is online
	while(!paired) {
		if( m_artist.PairScreen() ) return;
		if( recv.recv_msg() ){
			message.assign(recv.get_msg(), recv.get_msg()+_UDPSSMcast_h_DEFAULT_MSG_LEN);
			if( message[0] == 'p' and message[1] == 'i' and
				message[2] == 'n' and message[3] == 'g' ) {
				sender.send_msg("pong");
				paired = true;
			}
		}
	}
	// We flush any garbage on the socket, there shouldn't be anything
	recv.flush_socket();
	erase();
	m_artist.GameTable();
	refresh();

	bool got_sheep = false;
	sheep = new SpaceSheep(m_artist.get_GameW()/2,m_artist.get_GameH()-1-fatness,fatness);

	while( !got_sheep ){
		recv.recv_msg();
		message.assign(recv.get_msg(), recv.get_msg()+_UDPSSMcast_h_DEFAULT_MSG_LEN);

		if( message[0] == 'c' ) {
			position tmp_ref;
			tmp_ref.x = message[1];
			tmp_ref.y = m_artist.get_GameH()-1-message[3];

			sheep = new SpaceSheep(tmp_ref,(unsigned int)message[3]);
			m_artist.Pencil(sheep);
			got_sheep = true;
		}
	}
	recv.flush_socket();

	bool got_bull = false;
	bool online = true;
	bool received = false;
	bool new_game = true;

	unsigned short int count = 0;
	char ch = '0';

	while( online ){
		try {
			received = recv.recv_msg();
		}
		catch ( const char* msg ) {
			endwin();
			std::cerr << msg << std::endl;
		}
		if( received ) {
			message.assign(recv.get_msg(), recv.get_msg()+_UDPSSMcast_h_DEFAULT_MSG_LEN);
			if( message[0] == 'c' ){
				if( !got_sheep ){
					if( bushes.size() > 0 ){
						for (auto it = bushes.begin(); it != bushes.end(); it++) {
							m_artist.Pencil(*it);
						}
					}
					got_sheep = true;
				}
				m_artist.Animation(sheep, (unsigned int) message[1]);
				if ( got_bull ) m_artist.Pencil(bull);
				refresh();
			}
			else if( message[0] == 'r' ){
				if( got_sheep ){
					if( bushes.size() > 0 ){
						for (auto it = bushes.begin(); it != bushes.end(); it++) {
							m_artist.Rubber(*it);
							delete *it;
						}
						bushes.clear();
					}
					got_sheep = false;
				}
				RectObstacle * tmp_bush = new RectObstacle(message[1], message[2], message[3], message[4]);
				bushes.push_back(tmp_bush);
			}
		}
		if ( count%bull_prod == 0 ) {
			if( !got_bull ) {
				ch = '0';
				int i = 0;
				char tmp_char = '0';
				// This loop eats the queue on stdin, remains only 1 char.
				while ( true ) {
					tmp_char = getch();
					if ( tmp_char == EOF ) break;
					else ch = tmp_char;
					++i;
				}
				if ( i == 0 ) continue;
				else if ( ch == 'a'){
					bull = new SpaceBull(10, -3, 2);
					m_artist.Pencil(bull);
					got_bull = true;
					sender.send_msg(compose_msg(bull));
				}
				else if ( ch == 'E' ) std::cerr << "cosa a caso " << std::endl;
			}
			else if ( ((int)(bull->get_ref()).y - (int)(bull->get_fatness()) - 1) > (int)m_artist.get_GameH() ) {
				delete bull;
				got_bull = false;
			}
			else {
				m_artist.Animation(bull);
				sender.send_msg(compose_msg(bull));
			}

			// With this we are sure that the bull is over bushes, and they are
			//  always in sync.
			for (auto it = bushes.begin(); it != bushes.end(); it++) {
				m_artist.Pencil(*it);
			}
			if ( got_bull ) m_artist.Pencil(bull);
			refresh();
		}
		++count;
	}
	new_game = m_artist.ExitScreen(0);
	if ( new_game ) start();
}

void Engine :: add_obstacle_bushes ()
{
	/*
	 * In the next 'while(!ctrl)' cycle we check different parameters to
	 *  have a good game environment.
	 * RectObstacle:
	 * - size limits for x,y,h,w at creation time
	 * - limits on (x+w) to have every obstacle inside Game Table
	 * - minimum horizontal distance between obstacle, to be sure
	 *    the SpaceSheep can pass (m_dist)
	 * - minimum tot with of obstacle on same row, so it's always
	 *    engaging (tot_w)
	 * - check obstacle overlapping
	 *
	 * WARNING:
	 * It's not so simple to respect all these rules.
	 * Minimum width (m_w) and maximum width (M_w) values modify the
	 *  solution to the problem.
	 *
	 * GameLimit           center              GameLimit
	 * |                   |                   |
	 * |                |obst |free|  obst     |
	 * |                ,-----,----,-----------|
	 * |                m_w   |    X           |
	 *                        m_dist
	 *
	 * In this situation X must be m_w<=X<=M_w and w_min+X=tot_w
	 * Idem with M_w instead of m_w in the scheme.
	 */

	// Random stuff
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<unsigned int> distribution(0,m_artist.get_GameW()-1);

	unsigned int x = 0, w = 1, h = 1;
	int y = 0;
	bool ctrl = false; //to check obstacle spawn

	for ( unsigned int i=0; i<2; ++i) {
		RectObstacle* tmp_bush = new RectObstacle(x,y,w,h);
		while (!ctrl) {
			w = (distribution(generator) % bushes_w_r) + bushes_w_m; //(dist%range)+min
			x = distribution(generator);
			if ( (x + w) <= m_artist.get_GameW() ) ctrl = true;
			if ( i > 0 ) {
				if ( ctrl ) {
					if ( (((int)x >= (((*(bushes.back())).get_ref()).x
								+ (int)((*(bushes.back())).get_rec()).width)) and
							((int)x - (((*(bushes.back())).get_ref()).x +
								(int)((*(bushes.back())).get_rec()).width)) <
								((int)(*sheep).get_fatness()*2+1+(int)bushes_w_d)) ) ctrl = false;
					if ( ((((int)x+(int)w) <= ((*(bushes.back())).get_ref()).x) and
							(((*(bushes.back())).get_ref()).x - ((int)x + (int)w)) <
								((int)(*sheep).get_fatness()*2+1+(int)bushes_w_d)) ) ctrl = false;
				}
				if ( ctrl ) {
					if ( ((*(bushes.back())).get_rec()).width+w < bushes_w_tot ) {
						ctrl = false;
					}
				}
			}

			h = (distribution(generator) % bushes_h_r) + bushes_h_m ;
			y = -h ;
			RectObstacle* t_b = new RectObstacle(x,y,w,h);
			delete tmp_bush;
			tmp_bush = t_b;

			if ( ctrl ) {
				for (auto it = bushes.begin(); it != bushes.end(); it++) {
					if ( ((*(*it)).get_hitbox()).Overlap((*tmp_bush).get_hitbox()) ) {
						ctrl = false;
						break;
					}
				}
			}
		}
		ctrl = false; //reset control bool
		bushes.push_back(tmp_bush); //only good obstacle are stored

		// Old obstacles need to be erased from the vector TMP
		for (auto it = bushes.begin(); it != bushes.end(); it++) {
			if ( ((*(*it)).get_ref()).y > (int)m_artist.get_GameH() ) {
				delete *it;
				bushes.erase(it);
			}
		}
	}
}

bool Engine :: check_bushes_parameters ()
{
	if ( !( (((int)bushes_w_m/2) + ((int)m_artist.get_GameW()/2) -
				((int)(*sheep).get_fatness()*2+1+(int)bushes_w_d)) >= (int)bushes_w_tot
			and (((int)m_artist.get_GameW()/2) - (((int)bushes_w_m+(int)bushes_w_r)/2) -
				((int)(*sheep).get_fatness()*2+1+(int)bushes_w_d)) >= (int)bushes_w_m ) ) {
		return false;
	}
	else return true;
}

void Engine :: set_bushes_properties (unsigned int n_bushes_w_d,
		unsigned int n_bushes_w_tot, unsigned int n_bushes_w_m,
		unsigned int n_bushes_w_r, unsigned int n_bushes_h_m,
		unsigned int n_bushes_h_r)
{
	bushes_w_d = n_bushes_w_d;
	bushes_w_tot = n_bushes_w_tot;
	bushes_w_m = n_bushes_w_m;
	bushes_w_r = n_bushes_w_r;
	bushes_h_m = n_bushes_h_m;
	bushes_h_r = n_bushes_h_r;
	if ( !check_bushes_parameters() ) {
		throw "Engine::set_bushes_properties() ERROR: bad parameters for bushes, the game risks a loop.";
	}
}

void Engine :: set_movement_properties (unsigned int n_dt_uint_sheep,
		char n_left_mov, char n_right_mov)
{
	dt_uint_sheep = n_dt_uint_sheep;
	left_mov = n_left_mov;
	right_mov = n_right_mov;
}
