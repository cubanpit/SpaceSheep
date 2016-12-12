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

Engine::Engine(unsigned int xDim, unsigned int yDim, unsigned int fatness,
		unsigned int bushes_prod, unsigned int dt_uint_bull, unsigned int dt_uint_bushes):
	m_artist(xDim,yDim), m_fatness(fatness),
	m_bushes_prod(bushes_prod), m_dt_uint_bull(dt_uint_bull), m_dt_uint_bushes(dt_uint_bushes)
{
	if ( fatness > _Engine_h_MAX_FATNESS ) {
		throw "Engine::Engine() ERROR: Sheep is too fat!";
	}
	m_sheep = nullptr;
	m_bull = nullptr;
	m_bushes.clear();
	m_sender = nullptr;
	m_recver = nullptr;
	m_my_ip_addr = "";
	m_opp_ip_addr = "";
}

Engine::~Engine()
{
	//the game is closing, let's clean everything
	if ( m_bull != nullptr ) delete m_bull;
	if ( m_sheep != nullptr ) delete m_sheep;
	for (auto it = m_bushes.begin(); it != m_bushes.end(); it++) {
		delete *it;
	}
	m_bushes.clear();
	if ( m_sender != nullptr ) delete m_sender;
	if ( m_recver != nullptr ) delete m_recver;
}

void Engine::start()
{
	curs_set(0); // hide cursor position [ncurses]
	noecho(); // to hide input obtained with getch() [ncurses]

	bool gimme_more = true;
	while ( gimme_more ) {
		//the game is starting, let's clean everything from any previous game
		if ( m_bull != nullptr ) delete m_bull;
		m_bull = nullptr;
		if ( m_sheep != nullptr ) delete m_sheep;
		m_sheep = nullptr;
		for (auto it = m_bushes.begin(); it != m_bushes.end(); it++) {
			delete *it;
		}
		m_bushes.clear();
		if ( m_sender != nullptr ) delete m_sender;
		m_sender = nullptr;
		if ( m_recver != nullptr ) delete m_recver;
		m_recver = nullptr;

		char user_choice;
		user_choice = m_artist.welcome_screen();
	    if ( user_choice == 'n' ) gimme_more = run_local();
		else if ( user_choice == 'e' ) gimme_more = run_evil();
		else if ( user_choice == 'g' ) gimme_more = run_good();
		else if ( user_choice == 'q' ) gimme_more = false;
	}
}

bool Engine::run_local()
{
	timeout(0); // getch() does not wait for input [ncurses]
	erase();
	m_bushes.clear(); //clear vector, if it's not empty

	m_artist.game_table();
	if ( m_sheep != nullptr ) delete m_sheep;
	m_sheep = new SpaceSheep(m_artist.get_gameW()/2,
							m_artist.get_gameH()-1-m_fatness,m_fatness);
	m_artist.pencil(m_sheep);

	// TIME STUFF
	// WARNING: clocks goes on during execution, including screens
	// SOLUTION: reassign time_point before using it, if necessary.
	std::chrono::system_clock::time_point t_track_sheep = std::chrono::system_clock::now();
	std::chrono::duration<int,std::milli> dt_sheep(m_dt_uint_sheep);

	//temporary time interval we modify during the game
	unsigned int tmp_dt_uint_bushes = m_dt_uint_bushes;
	unsigned int count = 0;
	bool dead = false;
	char ch; //needed for sheep movement

	if ( !check_bushes_parameters() ) {
		throw "Engine::run_local() ERROR: bad parameters for bushes, "
				"the game risks a loop.";
	}

	while (!dead) {
		if ( !(count%m_bushes_prod) ) {
			try {
				add_obstacle_bushes();
			}
			catch (std::bad_alloc& ba)
			{
				endwin();
				std::cerr << "Engine::add_obstacle_bushes() bad_alloc caught: "
					<< ba.what() << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		++count;
		//compute score, different weight for different difficulty level
		m_score = (float)count*(200/((float)m_bushes_prod+
					((float)m_bushes_w_d*10)+((float)tmp_dt_uint_bushes/5)));
		//increase difficult level with time
		if ( !(count%10) ) --tmp_dt_uint_bushes;

		for (auto it = m_bushes.begin(); it != m_bushes.end(); it++) {
			m_artist.animation(*it);
		}
		for (auto it = m_bushes.begin(); it != m_bushes.end(); it++) {
			if ( ((*(*it)).get_hitbox()).overlap((*m_sheep).get_hitbox()) ) {
				dead = true;
				break;
			}
		}

		for (unsigned short int i=0; i<tmp_dt_uint_bushes and !dead; i+=m_dt_uint_sheep) {
			ch = getch();
			if ( ch == m_left_mov and  (((*m_sheep).get_ref()).x -
						(int)(*m_sheep).get_radius()) > 1 ) {
				m_artist.animation(m_sheep,'l');
			}
			else if ( ch == m_right_mov and (((*m_sheep).get_ref()).x +
						(int)(*m_sheep).get_radius()) <
						((int)m_artist.get_gameW() - 1) ) {
				m_artist.animation(m_sheep,'r');
			}
			else if ( ch == m_pause ) {
				dead = m_artist.pause_screen();
				if ( !dead ) {
					t_track_sheep = std::chrono::system_clock::now();
					m_artist.game_table();
					for (auto it = m_bushes.begin(); it != m_bushes.end(); it++) {
						m_artist.pencil(*it);
					}
					m_artist.pencil(m_sheep);
				}
			}

			for (auto it = m_bushes.begin(); it != m_bushes.end(); it++) {
				if ( ((*(*it)).get_hitbox()).overlap((*m_sheep).get_hitbox()) ) {
					dead = true;
					break;
				}
			}
			m_artist.update_score(m_score);
			refresh();
			t_track_sheep += dt_sheep;
			std::this_thread::sleep_until(t_track_sheep);
		}
		refresh();
	}
	return m_artist.exit_local_screen(m_score);
}

bool Engine::run_good()
{
	timeout(0); // getch() does not wait for input [ncurses]
	erase();
	m_bushes.clear(); //clear vector, if it's not empty

	// continue to ask the IP address till is a valid one
	std::string error_string = "";
	while ( m_sender == nullptr ) {
		if ( error_string.length() > 0 or m_opp_ip_addr.length() == 0 ) {
			m_opp_ip_addr = m_artist.addr_input_screen("your opponent's",
								_UDPMcastSender_h_DEFAULT_PORT,	error_string);
		}
		m_sender = new UDPSSMcastSender("",_UDPMcastSender_h_DEFAULT_TTL,
										m_opp_ip_addr,3264); //open socket
		if ( !m_sender->good() ) {
			error_string = m_sender->get_error();
			delete m_sender;
			m_sender = nullptr;
		}
		else error_string = "";
	}
	while ( m_recver == nullptr ) {
		if ( error_string.length() > 0 or m_my_ip_addr.length() == 0 ) {
			m_my_ip_addr = m_artist.addr_input_screen("your own",
								_UDPMcastSender_h_DEFAULT_PORT,	error_string);
		}
		m_recver = new UDPSSMcastReceiver("",m_my_ip_addr,3263);
		if ( !m_recver->good() ) {
			error_string = m_recver->get_error();
			delete m_recver;
			m_recver = nullptr;
		}
		else error_string = "";
	}

	bool exit_to_menu = false;
	exit_to_menu = !(pair_with_evil());
	m_artist.game_table();
	refresh();

	if ( m_sheep != nullptr ) delete m_sheep;
	m_sheep = new SpaceSheep(m_artist.get_gameW()/2,
						m_artist.get_gameH()-1-m_fatness,m_fatness);
	m_sender->send_msg(compose_msg(m_sheep));
	m_artist.pencil(m_sheep);

	// TIME STUFF
	// WARNING: clocks goes on during execution, including screens
	// SOLUTION: reassign time_point before using it, if necessary.
	std::chrono::system_clock::time_point t_track_sheep = std::chrono::system_clock::now();
	std::chrono::duration<int,std::milli> dt_sheep(m_dt_uint_sheep);

	unsigned int count = 0;
	bool dead = false;
	bool got_bull = false;
	bool received = false;
	char ch; //needed for sheep movement
	std::vector<char> message;

	if ( !check_bushes_parameters() ) {
		throw "Engine::run_local() ERROR: bad parameters for bushes, the game "
			"risks a loop.";
	}

	while ( !dead and !exit_to_menu ) {
		if ( !(count%m_bushes_prod) ) {
			try {
				add_obstacle_bushes();
			}
			catch (std::bad_alloc& ba)
			{
				endwin();
				std::cerr << "Engine::add_obstacle_bushes() bad_alloc caught: "
							<< ba.what() << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		++count;
		//compute score, different weight for different difficulty level
		m_score = (float)count*(200/((float)m_bushes_prod+
					((float)m_bushes_w_d*10)+((float)m_dt_uint_bushes/10)));

		for (auto it = m_bushes.begin(); it != m_bushes.end(); it++) {
			m_artist.animation(*it);
			m_sender->send_msg(compose_msg(*it));
		}

		for (auto it = m_bushes.begin(); it != m_bushes.end(); it++) {
			if ( ((*(*it)).get_hitbox()).overlap((*m_sheep).get_hitbox()) ) {
				dead = true;
				break;
			}
		}

		for (unsigned short int i=0; i<m_dt_uint_bushes and !dead; i+=m_dt_uint_sheep) {
			ch = getch();
			if ( ch == m_left_mov and  (((*m_sheep).get_ref()).x -
						(int)(*m_sheep).get_radius()) > 1 ) {
				m_artist.animation(m_sheep,'l');
			}
			else if ( ch == m_right_mov and (((*m_sheep).get_ref()).x +
						(int)(*m_sheep).get_radius()) <
						((int)m_artist.get_gameW() - 1) ) {
				m_artist.animation(m_sheep,'r');
			}

			for (auto it = m_bushes.begin(); it != m_bushes.end(); it++) {
				if ( ((*(*it)).get_hitbox()).overlap((*m_sheep).get_hitbox()) ) {
					dead = true;
					break;
				}
			}
			if( got_bull and ((*m_bull).get_hitbox()).overlap((*m_sheep).get_hitbox()) ) {
				dead = true;
				break;
			}

			try {
				received = m_recver->recv_msg();
			}
			catch ( const char* msg ) {
				endwin();
				std::cerr << msg << std::endl;
			}
			if( received ) {
				message.assign(m_recver->get_msg(),
						m_recver->get_msg()+_UDPSSMcast_h_DEFAULT_MSG_LEN);
				got_bull = create_bull(message);
				if ( got_bull ) m_score += 1;

				if( got_bull and ((*m_bull).get_hitbox()).overlap((*m_sheep).get_hitbox()) ) {
					dead = true;
					break;
				}
			}

			// With this we are sure that the bull is over bushes, and they are
			//  always in sync.
			for (auto it = m_bushes.begin(); it != m_bushes.end(); it++) {
				m_artist.pencil(*it);
			}
			if ( got_bull ) m_artist.pencil(m_bull);
			m_artist.update_score(m_score);
			refresh();

			m_sender->send_msg(compose_msg(m_sheep));
			t_track_sheep += dt_sheep;
			std::this_thread::sleep_until(t_track_sheep);
		}
		refresh();
	}
	m_sender->send_msg("dead");
	if ( !exit_to_menu ) return m_artist.exit_good_screen(m_score);
	else return true;
}

bool Engine::run_evil()
{
	timeout(0); // getch() does not wait for input [ncurses]
	erase();
	m_bushes.clear(); //clear vector, if it's not empty

	// continue to ask the IP address till is a valid one
	std::string error_string = "";
	while ( m_sender == nullptr ) {
		if ( error_string.length() > 0 or m_opp_ip_addr.length() == 0 ) {
			m_opp_ip_addr = m_artist.addr_input_screen("your opponent's",
								_UDPMcastSender_h_DEFAULT_PORT,error_string);
		}
		m_sender = new UDPSSMcastSender("",_UDPMcastSender_h_DEFAULT_TTL,
											m_opp_ip_addr,3263); //open socket
		if ( !m_sender->good() ) {
			error_string = m_sender->get_error();
			delete m_sender;
			m_sender = nullptr;
		}
		else error_string = "";
	}
	while ( m_recver == nullptr ) {
		if ( error_string.length() > 0 or m_my_ip_addr.length() == 0 ) {
			m_my_ip_addr = m_artist.addr_input_screen("your own",
								_UDPMcastSender_h_DEFAULT_PORT,error_string);
		}
		m_recver = new UDPSSMcastReceiver("",m_my_ip_addr,3264);
		if ( !m_recver->good() ) {
			error_string = m_recver->get_error();
			delete m_recver;
			m_recver = nullptr;
		}
		else error_string = "";
	}

	bool exit_to_menu = false;
	exit_to_menu = !(pair_with_good());
	m_artist.game_table();
	m_artist.creator_choice();

	std::vector<char> message;
	bool got_sheep = false;
	while( !got_sheep and !exit_to_menu ){
		m_recver->recv_msg();
		message.assign(m_recver->get_msg(), m_recver->get_msg()+_UDPSSMcast_h_DEFAULT_MSG_LEN);

		if( message[0] == 'c' ) {
			position tmp_ref;
			tmp_ref.x = message[1];
			tmp_ref.y = m_artist.get_gameH()-1-message[3];

			if ( m_sheep != nullptr ) delete m_sheep;
			m_sheep = new SpaceSheep(tmp_ref,(unsigned int)message[3]);
			m_artist.pencil(m_sheep);
			got_sheep = true;
		}
	}
	m_recver->flush_socket();

	bool got_bull = false;
	bool victory = false;
	bool received = false;

	unsigned int count = 0;
	while ( !victory and !exit_to_menu ){
		try {
			received = m_recver->recv_msg();
		}
		catch ( const char* msg ) {
			endwin();
			std::cerr << msg << std::endl;
		}
		if ( received ) {
			message.assign(m_recver->get_msg(),
					m_recver->get_msg()+_UDPSSMcast_h_DEFAULT_MSG_LEN);
			if ( message[0] == 'c' ){
				if ( !got_sheep ){
					got_sheep = true;
				}
				m_artist.animation(m_sheep, (unsigned int) message[1]);
			}
			else if ( message[0] == 'r' ){
				if( got_sheep ){
					if( m_bushes.size() > 0 ){
						for (auto it = m_bushes.begin(); it != m_bushes.end(); it++) {
							m_artist.rubber(*it);
							delete *it;
						}
						m_bushes.clear();
					}
					got_sheep = false;
				}
				RectObstacle * tmp_bush = new RectObstacle(message[1],
											message[2], message[3], message[4]);
				m_bushes.push_back(tmp_bush);
			}
			else if ( message[0] == 'd' and message[1] == 'e' and
						message[2] == 'a' and message[3] == 'd') {
				victory = true;
			}
		}
		if ( count%(m_dt_uint_bull/m_dt_uint_sheep) == 0 and !victory ) {
			if( !got_bull ) {
				got_bull = bull_creator_choice();
				if ( got_bull ) {
					m_artist.pencil(m_bull);
					m_sender->send_msg(compose_msg(m_bull));
				}
			}
			else if ( ((int)(m_bull->get_ref()).y - (int)(m_bull->get_radius()) - 1)
												> (int)m_artist.get_gameH() ) {
				delete m_bull;
				m_bull = nullptr;
				got_bull = false;
			}
			else {
				m_artist.animation(m_bull);
				m_sender->send_msg(compose_msg(m_bull));
			}
		}
		// With this we are sure that the bull is over bushes, and they are
		//  always in sync.
		for (auto it = m_bushes.begin(); it != m_bushes.end(); it++) {
			m_artist.pencil(*it);
		}
		if ( got_bull ) m_artist.pencil(m_bull);
		refresh();
		++count;
	}
	if ( !exit_to_menu ) return m_artist.exit_evil_screen();
	else return true;
}

void Engine :: add_obstacle_bushes ()
{
	/*
	 * In the next 'while(!ctrl)' cycle we check different parameters to
	 *  have a good game environment.
	 * - size limits for x,y,h,w at creation time
	 * - limits on (x+w) to have every obstacle inside Game Table
	 * - minimum horizontal distance between obstacle, to be sure
	 *    the SpaceSheep can pass (m_dist)
	 * - minimum tot width of obstacle on same row, so it's always
	 *    engaging (tot_w)
	 * - check obstacle overlapping
	 */

	// Random stuff
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<unsigned int> distribution(0,
														m_artist.get_gameW()-1);

	unsigned int x = 0, w = 1, h = 1;
	int y = 0;
	bool ctrl = false; //to check obstacle spawn

	for ( unsigned int i=0; i<2; ++i) {
		RectObstacle* tmp_bush = new RectObstacle(x,y,w,h);
		while (!ctrl) {
			//(dist%range)+min
			w = (distribution(generator) % m_bushes_w_r) + m_bushes_w_m;
			x = distribution(generator);
			if ( (x + w) <= m_artist.get_gameW() ) ctrl = true;
			if ( i > 0 ) {
				if ( ctrl ) {
					if ( (((int)x >= (((*(m_bushes.back())).get_v()).x
								+ (int)((*(m_bushes.back())).get_rec()).width))
							and	((int)x - (((*(m_bushes.back())).get_v()).x +
								(int)((*(m_bushes.back())).get_rec()).width)) <
								((int)(*m_sheep).get_radius()*2+1+(int)m_bushes_w_d)) ) {
						ctrl = false;
					}
					if ( ((((int)x+(int)w) <= ((*(m_bushes.back())).get_v()).x)
							and	(((*(m_bushes.back())).get_v()).x -
								((int)x + (int)w)) <
								((int)(*m_sheep).get_radius()*2+1+
								(int)m_bushes_w_d)) ) {
						ctrl = false;
					}
				}
				if ( ctrl ) {
					if ( ((*(m_bushes.back())).get_rec()).width+w < m_bushes_w_tot ) {
						ctrl = false;
					}
				}
			}

			h = (distribution(generator) % m_bushes_h_r) + m_bushes_h_m ;
			y = -h ;
			RectObstacle* t_b = new RectObstacle(x,y,w,h);
			delete tmp_bush;
			tmp_bush = t_b;

			if ( ctrl ) {
				for (auto it = m_bushes.begin(); it != m_bushes.end(); it++) {
					if ( ((*(*it)).get_hitbox()).overlap((*tmp_bush).get_hitbox()) ) {
						ctrl = false;
						break;
					}
				}
			}
		}
		ctrl = false; //reset control bool
		m_bushes.push_back(tmp_bush); //only good obstacle are stored

		// Old obstacles need to be erased from the vector TMP
		for (auto it = m_bushes.begin(); it != m_bushes.end(); it++) {
			if ( ((*(*it)).get_v()).y > (int)m_artist.get_gameH() ) {
				delete *it;
				m_bushes.erase(it);
			}
		}
	}
}

bool Engine :: check_bushes_parameters () const
{
	/*
	 * As explained in add_obstacle_bushes(), there are many parameters used
	 *  to check we have a friendly game, and not an impossible one.
	 * This parameters need a control, otherwise they can generate an infinite
	 *  when trying to generate obstacle fitting them.
	 *
	 * WARNING:
	 * It's not so simple.
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

	if ( !( (((int)m_bushes_w_m/2) + ((int)m_artist.get_gameW()/2) -
				((int)(*m_sheep).get_radius()*2+1+(int)m_bushes_w_d))
				>= (int)m_bushes_w_tot
			and (((int)m_artist.get_gameW()/2) -
				(((int)m_bushes_w_m+(int)m_bushes_w_r)/2) -
				((int)(*m_sheep).get_radius()*2+1+(int)m_bushes_w_d))
				>= (int)m_bushes_w_m ) ) {
		return false;
	}
	else if ( (m_bushes_h_m+m_bushes_h_r) > m_bushes_prod ) return false;
	else return true;
}

bool Engine :: bull_creator_choice()
{
	char user_choice = '0';
	char tmp_char = '0';
	unsigned short int count_input = 0;
	// This loop eats the queue on stdin, remains only 1 char.
	while ( true ) {
		tmp_char = getch();
		if ( tmp_char == EOF ) break;
		else user_choice = tmp_char;
		++count_input;
	}
	if ( count_input == 0 ) return false;
	else if ( user_choice == 'q'){
		m_bull = new SpaceBull(m_artist.get_gameW()/11, -2, 2);
		return true;
	}
	else if ( user_choice == 'w'){
		m_bull = new SpaceBull(2*m_artist.get_gameW()/11, -2, 2);
		return true;
	}
	else if ( user_choice == 'e'){
		m_bull = new SpaceBull(3*m_artist.get_gameW()/11, -2, 2);
		return true;
	}
	else if ( user_choice == 'r'){
		m_bull = new SpaceBull(4*m_artist.get_gameW()/11, -2, 2);
		return true;
	}
	else if ( user_choice == 't'){
		m_bull = new SpaceBull(5*m_artist.get_gameW()/11, -2, 2);
		return true;
	}
	else if ( user_choice == 'y'){
		m_bull = new SpaceBull(6*m_artist.get_gameW()/11, -2, 2);
		return true;
	}
	else if ( user_choice == 'u'){
		m_bull = new SpaceBull(7*m_artist.get_gameW()/11, -2, 2);
		return true;
	}
	else if ( user_choice == 'i'){
		m_bull = new SpaceBull(8*m_artist.get_gameW()/11, -2, 2);
		return true;
	}
	else if ( user_choice == 'o'){
		m_bull = new SpaceBull(9*m_artist.get_gameW()/11, -2, 2);
		return true;
	}
	else if ( user_choice == 'p'){
		m_bull = new SpaceBull(10*m_artist.get_gameW()/11, -2, 2);
		return true;
	}
	else return false;
}

// pair opponents, both should be able to send and receive packages
bool Engine::pair_with_good() const
{
	std::vector<char> message;
	bool paired = false;
	bool stop_pair = false;
	//send, check the opponent answers
	while( !paired and !stop_pair ) {
		stop_pair = m_artist.pair_screen();
		m_sender->send_msg("ping");
		if( m_recver->recv_msg() and !stop_pair ){
			message.assign(m_recver->get_msg(),
				m_recver->get_msg()+_UDPSSMcast_h_DEFAULT_MSG_LEN);
			if( message[0] == 'p' and message[1] == 'o'
				and	message[2] == 'n' and message[3] == 'g' ) {
				m_sender->send_msg("pong");
				paired = true;
			}
		}
	}

	m_recver->flush_socket();
	erase();
	return paired;
}

// pair opponents, both should be able to send and receive packages
bool Engine::pair_with_evil() const
{
	std::vector<char> message;
	bool paired = false;
	bool stop_pair = false;
	//checks he can receive, answers, then checks the opponent answered
	while( !paired and !stop_pair ) {
		stop_pair = m_artist.pair_screen();
		if( m_recver->recv_msg() and !stop_pair ){
			message.assign(m_recver->get_msg(),
				m_recver->get_msg()+_UDPSSMcast_h_DEFAULT_MSG_LEN);
			if( message[0] == 'p' and message[1] == 'i'
				and	message[2] == 'n' and message[3] == 'g' ) {
				m_sender->send_msg("pong");
				for(int i=0; i<100 and !paired and !stop_pair; ++i) {
					stop_pair = m_artist.pair_screen();
					if( m_recver->recv_msg() and !stop_pair ){
						message.assign(m_recver->get_msg(),
							m_recver->get_msg()+_UDPSSMcast_h_DEFAULT_MSG_LEN);
						if( message[0] == 'p' and message[1] == 'o'
							and	message[2] == 'n' and message[3] == 'g' ) {
							paired = true;
						}
					}
				}
			}
		}
	}
	m_recver->flush_socket();
	erase();
	return paired;
}

bool Engine::create_bull(std::vector<char>& message)
{
	// message[1] = X position
	// message[2] = Y position
	// message[3] = fatness
	if( m_bull == nullptr ){
		if( ((int)message[2] - (int)message[3])
				> (int)m_artist.get_gameH()
			or (int)message[3] > (int)_Engine_h_MAX_FATNESS
			or (int)message[1] > (int)m_artist.get_gameW() ) {
			return false;
		}
		else {
			m_bull = new SpaceBull(message[1] ,message[2],
												message[3]);
			return true;
		}
	}
	else {
		m_artist.rubber(m_bull);
		delete m_bull;
		m_bull = nullptr;
		if( ((int)message[2] - (int)message[3])
				> (int)m_artist.get_gameH()
			or (int)message[3] > (int)_Engine_h_MAX_FATNESS
			or (int)message[1] > (int)m_artist.get_gameW() ) {
			return false;
		}
		else {
			m_bull = new SpaceBull(message[1] ,message[2],
												message[3]);
			return true;
		}
	}
}

void Engine :: set_bushes_properties (unsigned int bushes_w_d,
		unsigned int bushes_w_tot, unsigned int bushes_w_m,
		unsigned int bushes_w_r, unsigned int bushes_h_m,
		unsigned int bushes_h_r)
{
	m_bushes_w_d = bushes_w_d;
	m_bushes_w_tot = bushes_w_tot;
	m_bushes_w_m = bushes_w_m;
	m_bushes_w_r = bushes_w_r;
	m_bushes_h_m = bushes_h_m;
	m_bushes_h_r = bushes_h_r;
	if ( !check_bushes_parameters() ) {
		throw "Engine::set_bushes_properties() ERROR: bad parameters for "
				"bushes, the game risks a loop.";
	}
}

void Engine :: set_movement_properties (unsigned int dt_uint_sheep,
		char left_mov, char right_mov)
{
	m_dt_uint_sheep = dt_uint_sheep;
	m_left_mov = left_mov;
	m_right_mov = right_mov;
}
