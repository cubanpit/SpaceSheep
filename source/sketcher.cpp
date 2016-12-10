/*
 * SpaceSheep, ncurses game. Code is hosted on GitHub.
 *
 * File: sketcher.cpp
 *
 * Implementation of sketcher.h classes.
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

#include "sketcher.h"

Sketcher :: Sketcher (unsigned int xDim, unsigned int yDim)
{
	m_xDim = xDim;
	m_yDim = yDim;
	m_gameW = xDim - 2;
	m_gameH = yDim - 2;
	if (COLS >= (int)m_xDim and LINES >= (int)m_yDim) {
		// (Offset+1) because we want all body drawn without overlapping
		//  the game table
		m_xOffset = ((COLS - m_xDim) / 2) + 1 ;
		m_yOffset = ((LINES - m_yDim) / 2) + 1 ;
	}
	else {
		std::string cols_str = std::to_string(COLS);
		std::string lines_str = std::to_string(LINES);
		std::string xDim_str = std::to_string(m_xDim);
		std::string yDim_str = std::to_string(m_yDim);
		throw "Sketcher::Sketcher() ERROR: terminal window is too small! "
			"\nGame table size: "+xDim_str+"x"+yDim_str+"\nCurrent terminal "
			"size: "+cols_str+"x"+lines_str;
	}

	// define some color we use in draw functions
	init_pair(1,COLOR_BLUE,COLOR_BLUE);
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	init_pair(3,COLOR_MAGENTA,COLOR_BLACK);
	init_pair(4,COLOR_GREEN,COLOR_BLACK);
	init_pair(5,COLOR_RED,COLOR_BLACK);
}

void Sketcher :: game_table () const
{
	attron(COLOR_PAIR(1));
	// (Offset-1) beacuse we have put a +1 in the offset, see previous comment
	for (unsigned short int i=1; i<(m_yDim-1); ++i) {
		mvprintw(m_yOffset-1+i, m_xOffset-1, "|");
		mvprintw(m_yOffset-1+i, m_xOffset-1+m_xDim-1, "|");
	}
	for (unsigned short int i=1; i<(m_xDim-1); ++i) {
		mvprintw(m_yOffset-1, m_xOffset-1+i, "-");
		mvprintw(m_yOffset-1+m_yDim-1, m_xOffset-1+i, "-");
	}
	mvprintw(m_yOffset-1, m_xOffset-1, "/");
	mvprintw(m_yOffset-1+m_yDim-1, m_xOffset-1, "\\");
	mvprintw(m_yOffset-1, m_xOffset-1+m_xDim-1, "\\");
	mvprintw(m_yOffset-1+m_yDim-1, m_xOffset-1+m_xDim-1, "/");
	attroff(COLOR_PAIR(1));
}

char Sketcher :: welcome_screen () const
{
	timeout(0); // getch() waits no time for input [ncurses]
	erase();

	game_table();
	attron(COLOR_PAIR(4));
	mvprintw(m_yOffset+1,m_xOffset+(m_gameW/2)-28,"________________________________________________________");
	attroff(COLOR_PAIR(4));
	attron(COLOR_PAIR(3));
	mvprintw(m_yOffset+2,m_xOffset+(m_gameW/2)-28,"   _____                      _____ __                  ");
	mvprintw(m_yOffset+3,m_xOffset+(m_gameW/2)-28,"  / ___/____  ____  ________ / ___// /_  ___  ___  ____ ");
	mvprintw(m_yOffset+4,m_xOffset+(m_gameW/2)-28,"  \\__ \\/ __ \\/ __ `/ ___/ _ \\\\__ \\/ __ \\/ _ \\/ _ \\/ __ \\");
	mvprintw(m_yOffset+5,m_xOffset+(m_gameW/2)-28," ___/ / /_/ / /_/ / /__/  __/__/ / / / /  __/  __/ /_/ /");
	mvprintw(m_yOffset+6,m_xOffset+(m_gameW/2)-28,"/____/ .___/\\__,_/\\___/\\___/____/_/ /_/\\___/\\___/ .___/ ");
	mvprintw(m_yOffset+7,m_xOffset+(m_gameW/2)-28,"    /_/                                        /_/      ");
	attroff(COLOR_PAIR(3));
	attron(COLOR_PAIR(4));
	mvprintw(m_yOffset+8,m_xOffset+(m_gameW/2)-28,"________________________________________________________");
	attroff(COLOR_PAIR(4));
	mvprintw(m_yOffset+11,m_xOffset+(m_gameW/2)-28,
			"Welcome in the Sheeps Galaxy!");
	mvprintw(m_yOffset+13,m_xOffset+(m_gameW/2)-28,
			"Help SpaceSheep to avoid space bushes using 'j' and 'l' key.");
	mvprintw(m_yOffset+15,m_xOffset+(m_gameW/2)-28,"Press:");
	mvprintw(m_yOffset+16,m_xOffset+(m_gameW/2)-28,
			" 'p' during the game to take a break.");
	mvprintw(m_yOffset+17,m_xOffset+(m_gameW/2)-28,
			" 'n' to start a local game. You against your computer!");
	mvprintw(m_yOffset+18,m_xOffset+(m_gameW/2)-28,
			" 'g' to start a 1v1 game, moving the sheep. Fear the bull!");
	mvprintw(m_yOffset+19,m_xOffset+(m_gameW/2)-28,
			" 'e' to start a 1v1 game, creating evil bulls. Hit the sheep!");
	mvprintw(m_yOffset+20,m_xOffset+(m_gameW/2)-28,
			" 'q' to quit the game now.");

	std::chrono::system_clock::time_point t_tmp_sheep = std::chrono::system_clock::now();
	std::chrono::duration<int,std::milli> dt_tmp_sheep(300);

	SpaceSheep *tmp = new SpaceSheep((m_gameW/2)-16,m_gameH-3,2);
	pencil(tmp);
	bool ctrl = false;
	char return_ch, tmp_char;
	unsigned int tmp_count = 0;
	while (!ctrl) {
		refresh();
		t_tmp_sheep += dt_tmp_sheep;
		std::this_thread::sleep_until(t_tmp_sheep);

		// This loop eats the queue on stdin, remains only 1 char.
		while ( true ) {
			tmp_char = getch();
			if ( tmp_char == EOF ) break;
			else return_ch = tmp_char;
		}
		if ( tmp_count%32 < 16 ) {
			animation(tmp,'r');
		}
		else if ( tmp_count%32 >= 16 ) {
			animation(tmp,'l');
		}
		++tmp_count;
		if ( return_ch == 'n' ) ctrl = true;
		else if ( return_ch == 'e' ) ctrl = true;
		else if ( return_ch == 'g' ) ctrl = true;
		else if ( return_ch == 'q' ) ctrl = true;
	}
	delete tmp;
	erase();
	return return_ch;
}

bool Sketcher :: exit_local_screen (unsigned int score) const
{
	timeout(-1); // getch() waits endlessly for input [ncurses]
	curs_set(1); // show cursor position [ncurses]
	echo(); // show user input [ncurses]
	bool saved_score = false;
	while ( !saved_score ) {
		erase();
		game_table();
		attron(COLOR_PAIR(5));
		mvprintw(m_yOffset+4,m_xOffset+(m_gameW/2)-19,"______________________________________");
		attroff(COLOR_PAIR(5));
		mvprintw(m_yOffset+5,m_xOffset+(m_gameW/2)-19,"__   __            _              _  ");
		mvprintw(m_yOffset+6,m_xOffset+(m_gameW/2)-19,"\\ \\ / /__  _   _  | |    ___  ___| |_ ");
		mvprintw(m_yOffset+7,m_xOffset+(m_gameW/2)-19," \\ V / _ \\| | | | | |   / _ \\/ __| __|");
		mvprintw(m_yOffset+8,m_xOffset+(m_gameW/2)-19,"  | | (_) | |_| | | |__| (_) \\__ \\ |_ ");
		mvprintw(m_yOffset+9,m_xOffset+(m_gameW/2)-19,"  |_|\\___/ \\__,_| |_____\\___/|___/\\__|");
		attron(COLOR_PAIR(5));
		mvprintw(m_yOffset+10,m_xOffset+(m_gameW/2)-19,"______________________________________");
		attroff(COLOR_PAIR(5));

		std::string msg = "Your score is "+std::to_string(score);
		mvprintw(m_yOffset+14,m_xOffset+(m_gameW/2)-19,msg.c_str());
		mvprintw(m_yOffset+17,m_xOffset+(m_gameW/2)-19,
				"[Do not use '_' and '\\']");
		mvprintw(m_yOffset+16,m_xOffset+(m_gameW/2)-19,"Enter your name: ");

		refresh();
		char input[20];
		getstr(input);
		std::string str_input(input);
		saved_score = add_score(score,str_input);
	}
	curs_set(0); // show cursor position [ncurses]
	noecho(); // show user input [ncurses]
	erase();
	game_table();

	std::vector<std::string> score_list = get_score(10);
	std::vector<std::string> score_value;
	std::vector<std::string> score_name;
	for (unsigned short int i=0; i<score_list.size(); ++i) {
		std::size_t pos = score_list[i].find("_");
		score_value.push_back(score_list[i].substr(0,pos));
		score_name.push_back(score_list[i].substr(pos+1,std::string::npos));
	}

	mvprintw(m_yOffset+2,m_xOffset+(m_gameW/2)-19,"Score history:");
	for (unsigned short int i=0; i<score_name.size(); ++i) {
		mvprintw(m_yOffset+4+i,m_xOffset+(m_gameW/2)-13-score_value[i].length(),
				score_value[i].c_str());
		mvprintw(m_yOffset+4+i,m_xOffset+(m_gameW/2)-11,
				score_name[i].c_str());
	}
	mvprintw(m_yOffset+18,m_xOffset+(m_gameW/2)-19,
			"Press 'n' to start a new game, 'q' to exit.");

	refresh();
	char tmp_ch = '0';
	while ( !(tmp_ch == 'q') and !(tmp_ch == 'n') ) {
		tmp_ch = getch();
	}
	erase();
	timeout(0);
	if ( tmp_ch == 'n' ) return true;
	else return false;
}

bool Sketcher :: exit_good_screen (unsigned int score) const
{
	timeout(-1); // getch() waits endlessly for input [ncurses]
	erase();
	game_table();
	attron(COLOR_PAIR(5));
	mvprintw(m_yOffset+4,m_xOffset+(m_gameW/2)-19,"______________________________________");
	attroff(COLOR_PAIR(5));
	mvprintw(m_yOffset+5,m_xOffset+(m_gameW/2)-19,"__   __            _              _  ");
	mvprintw(m_yOffset+6,m_xOffset+(m_gameW/2)-19,"\\ \\ / /__  _   _  | |    ___  ___| |_ ");
	mvprintw(m_yOffset+7,m_xOffset+(m_gameW/2)-19," \\ V / _ \\| | | | | |   / _ \\/ __| __|");
	mvprintw(m_yOffset+8,m_xOffset+(m_gameW/2)-19,"  | | (_) | |_| | | |__| (_) \\__ \\ |_ ");
	mvprintw(m_yOffset+9,m_xOffset+(m_gameW/2)-19,"  |_|\\___/ \\__,_| |_____\\___/|___/\\__|");
	attron(COLOR_PAIR(5));
	mvprintw(m_yOffset+10,m_xOffset+(m_gameW/2)-19,"______________________________________");
	attroff(COLOR_PAIR(5));

	std::string msg = "Your score is "+std::to_string(score);
	mvprintw(m_yOffset+16,m_xOffset+(m_gameW/2)-19,msg.c_str());
	mvprintw(m_yOffset+18,m_xOffset+(m_gameW/2)-19,
			"Press 'n' to start a new game, 'q' to exit.");

	refresh();
	char tmp_ch = '0';
	while ( !(tmp_ch == 'q') and !(tmp_ch == 'n') ) {
		tmp_ch = getch();
	}
	erase();
	timeout(0);
	if ( tmp_ch == 'n' ) return true;
	else return false;
}

bool Sketcher :: exit_evil_screen () const
{
	timeout(-1); // getch() waits endlessly for input [ncurses]
	erase();
	game_table();

	attron(COLOR_PAIR(2));
	mvprintw(m_yOffset+4,m_xOffset+(m_gameW/2)-19,"______________________________________");
	attroff(COLOR_PAIR(2));
	mvprintw(m_yOffset+5,m_xOffset+(m_gameW/2)-19,"__   __           __        ___       ");
	mvprintw(m_yOffset+6,m_xOffset+(m_gameW/2)-19,"\\ \\ / /__  _   _  \\ \\      / (_)_ __  ");
	mvprintw(m_yOffset+7,m_xOffset+(m_gameW/2)-19," \\ V / _ \\| | | |  \\ \\ /\\ / /| | '_ \\ ");
	mvprintw(m_yOffset+8,m_xOffset+(m_gameW/2)-19,"  | | (_) | |_| |   \\ V  V / | | | | |");
	mvprintw(m_yOffset+9,m_xOffset+(m_gameW/2)-19,"  |_|\\___/ \\__,_|    \\_/\\_/  |_|_| |_|");
	attron(COLOR_PAIR(2));
	mvprintw(m_yOffset+10,m_xOffset+(m_gameW/2)-19,"______________________________________");
	attroff(COLOR_PAIR(2));

	mvprintw(m_yOffset+16,m_xOffset+(m_gameW/2)-19,
			"Press 'n' to start a new game, 'q' to exit.");

	refresh();
	char tmp_ch = '0';
	while ( !(tmp_ch == 'q') and !(tmp_ch == 'n') ) {
		tmp_ch = getch();
	}
	erase();
	timeout(0);
	if ( tmp_ch == 'n' ) return true;
	else return false;
}

bool Sketcher :: pause_screen () const
{
	timeout(-1); // getch() waits endlessly for input [ncurses]
	erase();
	game_table();
	mvprintw(m_yOffset+15,m_xOffset+(m_gameW/2)-28,"The game is paused. "
										"Meantime SpaceSheep is getting gas.");
	mvprintw(m_yOffset+17,m_xOffset+(m_gameW/2)-28,"Press 'p' to continue, "
													"'q' to exit the game.");
	refresh();
	char tmp_ch = '0';
	while ( !(tmp_ch == 'p' or tmp_ch == 'q') ) {
		tmp_ch = getch();
	}
	erase();
	timeout(0);
	if ( tmp_ch == 'q' ) return true;
	else return false;
}

std::string Sketcher :: addr_input_screen (std::string owner,
											unsigned int default_port,
											std::string error) const
{
	timeout(-1); // getch() waits endlessly for input [ncurses]
	curs_set(1); // show cursor position [ncurses]
	echo(); // show user input [ncurses]
	erase();
	game_table();
	if ( error.length() > 0 ) {
		attron(COLOR_PAIR(5)); // enable red color
		mvprintw(m_yOffset+10,m_xOffset+1,error.c_str());
		attroff(COLOR_PAIR(5)); // disable red color
		mvprintw(m_yOffset+11,m_xOffset+1,"Please double check the IP address "
				"and retry.");
	}
	mvprintw(m_yOffset+15,m_xOffset+(m_gameW/2)-28,
			"You've chosen to play against an opponent through network.");
	std::string port_str = std::to_string(default_port);
	mvprintw(m_yOffset+16,m_xOffset+(m_gameW/2)-28,
			"The default port used to connect, that should be open, is UDP/");
	printw(port_str.c_str());
	unsigned short int owner_space = owner.length();
	mvprintw(m_yOffset+17,m_xOffset+(m_gameW/2)-28,"Give me ");
	printw(owner.c_str());
	mvprintw(m_yOffset+17,m_xOffset+(m_gameW/2)-20+owner_space,
			" IPv4 address: ");
	refresh();
	char input[20];
	getstr(input);
	std::string str_input(input);
	erase();
	timeout(0); // getch() doesn't wait for input
	curs_set(0); // doesn't show cursor
	noecho(); // hide user input
	return str_input;
}

bool Sketcher :: pair_screen () const
{
	timeout(0);
	erase();
	game_table();
	mvprintw(m_yOffset+15,m_xOffset+(m_gameW/2)-28,"Waiting for connection...");
	mvprintw(m_yOffset+17,m_xOffset+(m_gameW/2)-28,"Press 'q' to cancel.");
	refresh();
	char tmp_ch = '0';
	tmp_ch = getch();
	if ( tmp_ch == 'q' ) return true;
	else return false;
}

void Sketcher :: update_score (unsigned int score) const
{
	unsigned short int score_length = 1;
	unsigned int score_tmp = score;
	while ( score_tmp /= 10 ) ++score_length;

	//fill with '0' the first part, to have always 6 total digits
	std::string score_str(6-score_length,'0');
	score_str += std::to_string(score);
	mvprintw(m_yOffset-1, m_xOffset+m_xDim-4-5, score_str.c_str());
}

void Sketcher :: creator_choice() const
{
	mvprintw(m_yOffset-1, m_xOffset+m_gameW/11, "q");
	mvprintw(m_yOffset-1, m_xOffset+2*m_gameW/11, "w");
	mvprintw(m_yOffset-1, m_xOffset+3*m_gameW/11, "e");
	mvprintw(m_yOffset-1, m_xOffset+4*m_gameW/11, "r");
	mvprintw(m_yOffset-1, m_xOffset+5*m_gameW/11, "t");
	mvprintw(m_yOffset-1, m_xOffset+6*m_gameW/11, "y");
	mvprintw(m_yOffset-1, m_xOffset+7*m_gameW/11, "u");
	mvprintw(m_yOffset-1, m_xOffset+8*m_gameW/11, "i");
	mvprintw(m_yOffset-1, m_xOffset+9*m_gameW/11, "o");
	mvprintw(m_yOffset-1, m_xOffset+10*m_gameW/11, "p");
}

void Sketcher :: pencil (RectObstacle* bush) const
{
	// (width-1) and (height-1) because we have a side from x to x+5 we
	//  have a width of 6

	attron(COLOR_PAIR(2)); // enable bush color pair
	if ( (unsigned int)abs((bush->get_v()).y) < (m_yDim-2) ) {
		unsigned int i_stop_top = 1;
		unsigned int i_stop_bottom = ((bush->get_rec()).height-1);
		if ( ((bush->get_v()).y+(bush->get_rec()).height) >= (m_yDim-1) ) {
			if ( ((int)m_yDim-1-(bush->get_v()).y-2) < 0 ) i_stop_bottom = 0;
			else i_stop_bottom = m_yDim - 1 - (bush->get_v()).y - 2;
		}
		if ( (bush->get_v()).y < 0 ) {
			i_stop_top = -(bush->get_v()).y;
		} else {
			for (unsigned int i=1; i < ((bush->get_rec()).width-1); ++i) {
				mvprintw(m_yOffset+(bush->get_v()).y,
						m_xOffset+(bush->get_v()).x+i, "-");
			}
			mvprintw(m_yOffset+(bush->get_v()).y,
					m_xOffset+(bush->get_v()).x, "*");
			mvprintw(m_yOffset+(bush->get_v()).y,
					m_xOffset+(bush->get_v()).x+((bush->get_rec()).width-1), "*");
		}

		for (unsigned int i=i_stop_bottom; i>=i_stop_top; --i) {
			mvprintw(m_yOffset+(bush->get_v()).y+i,
					m_xOffset+(bush->get_v()).x, "|");
			mvprintw(m_yOffset+(bush->get_v()).y+i,
					m_xOffset+(bush->get_v()).x+((bush->get_rec()).width-1), "|");
		}

		if ( (bush->get_v()).y+(bush->get_rec()).height > 0
				and (bush->get_v()).y+(bush->get_rec()).height < (m_yDim-2) ) {
			for (unsigned int i=1; i < ((bush->get_rec()).width-1); ++i) {
				mvprintw(m_yOffset+(bush->get_v()).y+((bush->get_rec()).height-1),
						m_xOffset+(bush->get_v()).x+i, "-");
			}
			mvprintw(m_yOffset+(bush->get_v()).y+((bush->get_rec()).height-1),
					m_xOffset+(bush->get_v()).x, "*");
			mvprintw(m_yOffset+(bush->get_v()).y+((bush->get_rec()).height-1),
					m_xOffset+(bush->get_v()).x+((bush->get_rec()).width-1), "*");
		}
	}
	attroff(COLOR_PAIR(2)); // disable bush color pair
}

void Sketcher :: pencil (SpaceSheep* sheep) const
{
	mvprintw(m_yOffset+(sheep->get_ref()).y-sheep->get_radius(),
			m_xOffset+(sheep->get_ref()).x, "O");
	mvprintw(m_yOffset+(sheep->get_ref()).y-sheep->get_radius()+1,
			m_xOffset+(sheep->get_ref()).x-1, "*@*");
	for (unsigned int i=2; i < (sheep->get_radius()+1); ++i) {
		for (unsigned int j=0; j < (2*i)+1; ++j) {
			mvprintw(m_yOffset+(sheep->get_ref()).y-sheep->get_radius()+i,
					m_xOffset+(sheep->get_ref()).x-i+j, "@");
		}
	}
	for (unsigned int i=sheep->get_radius(); i > 0 ; --i) {
		for (unsigned int j=0; j < (2*i)-1; ++j) {
			mvprintw(m_yOffset+(sheep->get_ref()).y+(sheep->get_radius()-i+1),
					m_xOffset+(sheep->get_ref()).x-i+1+j, "@");
		}
	}
}

void Sketcher :: pencil (SpaceBull* bull) const
{
	attron(COLOR_PAIR(5)); // enable bull color pair
	if ( (unsigned int)abs((bull->get_ref()).y-bull->get_radius()) < (m_yDim-2) ) {
		unsigned int i_top_up = 0;
		unsigned int i_top_down = bull->get_radius() + 1;
		unsigned int i_bottom_up = bull->get_radius();
		unsigned int i_bottom_down = 0;
		unsigned int i_bottom_limit = 2;
		if ( (bull->get_ref()).y-(int)bull->get_radius() < 0 ) {
			i_top_up = abs((bull->get_ref()).y-(int)bull->get_radius());
		}
		if ( (bull->get_ref()).y > ((int)m_yDim-3) ) {
			i_top_down = bull->get_radius() + 1 - abs((bull->get_ref()).y
					-((int)m_yDim-3));
		}
		if ( (bull->get_ref()).y < 1 ) {
			if ( ((int)bull->get_radius()+(bull->get_ref()).y) > 0 ) {
				i_bottom_up = (int)bull->get_radius() + 1 + (bull->get_ref()).y;
			} else i_bottom_up = 0;
		}
		if ( ((bull->get_ref()).y+(int)bull->get_radius()) > ((int)m_yDim-3) ) {
			i_bottom_down = (bull->get_ref()).y + (int)bull->get_radius()
				- (int)m_yDim + 3;
		}
		for (unsigned int i=i_top_up; i < i_top_down; ++i) {
			for (unsigned int j=0; j < (2*i)+1; ++j) {
				mvprintw(m_yOffset+(bull->get_ref()).y-bull->get_radius()+i,
						m_xOffset+(bull->get_ref()).x-i+j, "@");
			}
		}
		if ( i_bottom_down > 2 ) i_bottom_limit = i_bottom_down;
		for (unsigned int i=i_bottom_up; i > i_bottom_limit; --i) {
			for (unsigned int j=0; j < (2*i)-1; ++j) {
				mvprintw(m_yOffset+(bull->get_ref()).y+(bull->get_radius()-i+1),
						m_xOffset+(bull->get_ref()).x-i+1+j, "@");
			}
		}
		if ( i_bottom_down < 2
				and ((bull->get_ref()).y+(int)bull->get_radius()-1) >= 0 ) {
			mvprintw(m_yOffset+(bull->get_ref()).y+bull->get_radius()-1,
					m_xOffset+(bull->get_ref()).x-1, "O@O");
		}
		if ( i_bottom_down < 1
				and ((bull->get_ref()).y+(int)bull->get_radius()) >= 0 ) {
			mvprintw(m_yOffset+(bull->get_ref()).y+bull->get_radius(),
					m_xOffset+(bull->get_ref()).x, "W");
		}
	}
	attroff(COLOR_PAIR(5)); // disable bull color pair
}

void Sketcher :: rubber (RectObstacle* bush) const
{
	for (unsigned int i=0; i < (bush->get_rec()).height; ++i) {
		if ( (bush->get_v()).y+(int)i != -1 and
				(bush->get_v()).y+(int)i != (int)(m_yDim-2) ) {
			mvprintw(m_yOffset+(bush->get_v()).y+i, m_xOffset+(bush->get_v()).x, " ");
			mvprintw(m_yOffset+(bush->get_v()).y+i,
					m_xOffset+(bush->get_v()).x+((bush->get_rec()).width-1), " ");
		}
	}

	if ( (bush->get_v()).y != -1 and (bush->get_v()).y != (int)(m_yDim-2) ) {
		for (unsigned int i=0; i < (bush->get_rec()).width; ++i) {
			mvprintw(m_yOffset+(bush->get_v()).y,
					m_xOffset+(bush->get_v()).x+i, " ");
		}
	}
	if ( (bush->get_v()).y+((int)(bush->get_rec()).height-1) != -1 and
			(bush->get_v()).y+((int)(bush->get_rec()).height-1) != (int)(m_yDim-2) ) {
		for (unsigned int i=0; i < (bush->get_rec()).width; ++i) {
			mvprintw(m_yOffset+(bush->get_v()).y+((bush->get_rec()).height-1),
					m_xOffset+(bush->get_v()).x+i, " ");
		}
	}
}

void Sketcher :: rubber (CircleObstacle* circle) const
{
	if ( abs((circle->get_ref()).y-(int)circle->get_radius()) < (int)(m_yDim-2) ) {
		unsigned int i_top_up = 0;
		unsigned int i_top_down = circle->get_radius() + 1;
		unsigned int i_bottom_up = circle->get_radius();
		unsigned int i_bottom_down = 0;
		if ( (circle->get_ref()).y-(int)circle->get_radius() < 0 ) {
			i_top_up = abs((circle->get_ref()).y-(int)circle->get_radius());
		}
		if ( (circle->get_ref()).y > ((int)m_yDim-3) ) {
			i_top_down = circle->get_radius() + 1 - abs((circle->get_ref()).y
					-((int)m_yDim-3));
		}
		if ( (circle->get_ref()).y < 1 ) {
			if ( ((int)circle->get_radius()+(circle->get_ref()).y) > 0 ) {
				i_bottom_up = (int)circle->get_radius() + 1 + (circle->get_ref()).y;
			} else i_bottom_up = 0;
		}
		if ( ((circle->get_ref()).y+(int)circle->get_radius()) > ((int)m_yDim-3) ) {
			i_bottom_down = (circle->get_ref()).y + (int)circle->get_radius()
				- (int)m_yDim + 3;
		}
		for (unsigned int i=i_top_up; i < i_top_down; ++i) {
			for (unsigned int j=0; j < (2*i)+1; ++j) {
				mvprintw(m_yOffset+(circle->get_ref()).y-circle->get_radius()+i,
						m_xOffset+(circle->get_ref()).x-i+j, " ");
			}
		}
		for (unsigned int i=i_bottom_up; i > i_bottom_down; --i) {
			for (unsigned int j=0; j < (2*i)-1; ++j) {
				mvprintw(m_yOffset+(circle->get_ref()).y+(circle->get_radius()-i+1),
						m_xOffset+(circle->get_ref()).x-i+1+j, " ");
			}
		}
	}
}

void Sketcher :: animation (RectObstacle* bush) const
{
	rubber(bush);
	bush->drop();
	pencil(bush);
}

void Sketcher :: animation (SpaceSheep* sheep, char dir) const
{
	rubber(sheep);
	sheep->move(dir);
	pencil(sheep);
}

void Sketcher :: animation (SpaceSheep* sheep, unsigned int x) const
{
	rubber(sheep);
	sheep->move_to(x);
	pencil(sheep);
}

void Sketcher :: animation (SpaceBull* bull) const
{
	rubber(bull);
	bull->drop();
	pencil(bull);
}

