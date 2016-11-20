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
	M_xDim = xDim;
	M_yDim = yDim;
	M_GameW = xDim - 2;
	M_GameH = yDim - 2;
	if (COLS >= M_xDim and LINES >= M_yDim) {
		// (Offset+1) because we want all body drawn without overlapping
		//  the game table
		M_xOffset = ((COLS - M_xDim) / 2) + 1 ;
		M_yOffset = ((LINES - M_yDim) / 2) + 1 ;
	}
	else {
	//	throw "Sketcher::Sketcher() ERROR: terminal window is too small! Check the current game table size."; tmp
	}

	// define some color we use in draw functions
	init_pair(1,COLOR_BLUE,COLOR_BLUE);
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	init_pair(3,COLOR_MAGENTA,COLOR_BLACK);
	init_pair(4,COLOR_GREEN,COLOR_BLACK);
	init_pair(5,COLOR_RED,COLOR_BLACK);
}

void Sketcher :: GameTable ()
{
	attron(COLOR_PAIR(1));
	// (Offset-1) beacuse we have put a +1 in the offset, see previous comment
	for (unsigned int i=1; i<(M_yDim-1); ++i) {
		mvprintw(M_yOffset-1+i, M_xOffset-1, "|");
		mvprintw(M_yOffset-1+i, M_xOffset-1+M_xDim-1, "|");
	}
	for (unsigned int i=1; i<(M_xDim-1); ++i) {
		mvprintw(M_yOffset-1, M_xOffset-1+i, "-");
		mvprintw(M_yOffset-1+M_yDim-1, M_xOffset-1+i, "-");
	}
	mvprintw(M_yOffset-1, M_xOffset-1, "/");
	mvprintw(M_yOffset-1+M_yDim-1, M_xOffset-1, "\\");
	mvprintw(M_yOffset-1, M_xOffset-1+M_xDim-1, "\\");
	mvprintw(M_yOffset-1+M_yDim-1, M_xOffset-1+M_xDim-1, "/");
	attroff(COLOR_PAIR(1));
}

void Sketcher :: WelcomeScreen ()
{
	timeout(0); // getch() waits no time for input [ncurses]
	erase();

	GameTable();
	attron(COLOR_PAIR(4));
	mvprintw(M_yOffset+4,M_xOffset+(M_GameW/2)-28,"________________________________________________________");
	attroff(COLOR_PAIR(4));
	attron(COLOR_PAIR(3));
	mvprintw(M_yOffset+5,M_xOffset+(M_GameW/2)-28,"   _____                      _____ __                  ");
	mvprintw(M_yOffset+6,M_xOffset+(M_GameW/2)-28,"  / ___/____  ____  ________ / ___// /_  ___  ___  ____ ");
	mvprintw(M_yOffset+7,M_xOffset+(M_GameW/2)-28,"  \\__ \\/ __ \\/ __ `/ ___/ _ \\\\__ \\/ __ \\/ _ \\/ _ \\/ __ \\");
	mvprintw(M_yOffset+8,M_xOffset+(M_GameW/2)-28," ___/ / /_/ / /_/ / /__/  __/__/ / / / /  __/  __/ /_/ /");
	mvprintw(M_yOffset+9,M_xOffset+(M_GameW/2)-28,"/____/ .___/\\__,_/\\___/\\___/____/_/ /_/\\___/\\___/ .___/ ");
	mvprintw(M_yOffset+10,M_xOffset+(M_GameW/2)-28,"    /_/                                        /_/      ");
	attroff(COLOR_PAIR(3));
	attron(COLOR_PAIR(4));
	mvprintw(M_yOffset+11,M_xOffset+(M_GameW/2)-28,"________________________________________________________");
	attroff(COLOR_PAIR(4));
	mvprintw(M_yOffset+15,M_xOffset+(M_GameW/2)-28,"Welcome in the Sheeps Galaxy!");
	mvprintw(M_yOffset+17,M_xOffset+(M_GameW/2)-28,"Help SpaceSheep to avoid space bushes using 'j' and 'l' key.");
	mvprintw(M_yOffset+18,M_xOffset+(M_GameW/2)-28,"Press 'p' during the game to take a break.");
	mvprintw(M_yOffset+20,M_xOffset+(M_GameW/2)-28,"Press 'n' to begin!");

	std::chrono::system_clock::time_point t_tmp_sheep = std::chrono::system_clock::now();
	std::chrono::duration<int,std::milli> dt_tmp_sheep(300);

	SpaceSheep *tmp = new SpaceSheep((M_GameW/2)-16,M_GameH-3,2);
	Pencil(tmp);
	bool ctrl = false;
	char tmp_ch;
	unsigned int tmp_count = 0;
	while (!ctrl) {
		refresh();
		t_tmp_sheep += dt_tmp_sheep;
		std::this_thread::sleep_until(t_tmp_sheep);
		if ( tmp_count%32 < 16 ) {
			Animation(tmp,1);
		}
		else if ( tmp_count%32 >= 16 ) {
			Animation(tmp,0);
		}
		++tmp_count;
		tmp_ch = getch();
		if ( tmp_ch == 'n' ) ctrl = true;
	}
	delete tmp;
	erase();
}

bool Sketcher :: ExitScreen (unsigned int score)
{
	timeout(-1); // getch() waits endlessly for input [ncurses]
	erase();
	GameTable();

	attron(COLOR_PAIR(5));
	mvprintw(M_yOffset+4,M_xOffset+(M_GameW/2)-19,"______________________________________");
	attroff(COLOR_PAIR(5));
	mvprintw(M_yOffset+5,M_xOffset+(M_GameW/2)-19,"__   __            _              _  ");
	mvprintw(M_yOffset+6,M_xOffset+(M_GameW/2)-19,"\\ \\ / /__  _   _  | |    ___  ___| |_ ");
	mvprintw(M_yOffset+7,M_xOffset+(M_GameW/2)-19," \\ V / _ \\| | | | | |   / _ \\/ __| __|");
	mvprintw(M_yOffset+8,M_xOffset+(M_GameW/2)-19,"  | | (_) | |_| | | |__| (_) \\__ \\ |_ ");
	mvprintw(M_yOffset+9,M_xOffset+(M_GameW/2)-19,"  |_|\\___/ \\__,_| |_____\\___/|___/\\__|");
	attron(COLOR_PAIR(5));
	mvprintw(M_yOffset+10,M_xOffset+(M_GameW/2)-19,"______________________________________");
	attroff(COLOR_PAIR(5));

	std::string msg = "Your score is ";
	std::string str_score = std::to_string(score);
	msg += str_score;
	const char * c = msg.c_str();
	mvprintw(M_yOffset+14,M_xOffset+(M_GameW/2)-19,c);
	mvprintw(M_yOffset+16,M_xOffset+(M_GameW/2)-19,"Press 'n' to start a new game, 'q' to exit.");

	refresh();
	char tmp_ch = '0';
	while ( !(tmp_ch == 'q') and !(tmp_ch == 'n') ) {
		tmp_ch = getch();
	}
	erase();
	timeout(0);
	if ( tmp_ch == 'q' ) return false;
	else if ( tmp_ch == 'n' ) return true;
}

bool Sketcher :: PauseScreen ()
{
	timeout(-1); // getch() waits endlessly for input [ncurses]
	erase();
	GameTable();
	mvprintw(M_yOffset+15,M_xOffset+(M_GameW/2)-28,"The game is paused. Meantime SpaceSheep is getting gas.");
	mvprintw(M_yOffset+17,M_xOffset+(M_GameW/2)-28,"Press 'p' to continue, 'q' to exit the game.");
	refresh();
	char tmp_ch = '0';
	while ( !(tmp_ch == 'p' or tmp_ch == 'q') ) {
		tmp_ch = getch();
	}
	erase();
	timeout(0);
	if ( tmp_ch == 'p' ) return 0;
	else if ( tmp_ch == 'q' ) return 1;
}

void Sketcher :: Score (unsigned int score)
{
	std::string s_ch[6];
	s_ch[0] = std::to_string(score % 10);
	s_ch[1] = std::to_string((int)((score % 100) / 10));
	s_ch[2] = std::to_string((int)((score % 1000) / 100));
	s_ch[3] = std::to_string((int)((score % 10000) / 1000));
	s_ch[4] = std::to_string((int)((score % 100000) / 10000));
	s_ch[5] = std::to_string((int)((score % 1000000) / 100000));
	for (int i=0; i<6; ++i) {
		const char * c = s_ch[i].c_str();
		mvprintw(M_yOffset-1, M_xOffset+M_xDim-4-i, c);
	}
}

void Sketcher :: Pencil (RectObstacle* bush)
{
	// (width-1) and (height-1) because we have a side from x to x+5 we
	//  have a width of 6

	attron(COLOR_PAIR(2)); // enable bush color pair
	if ( abs((bush->get_ref()).y) < (M_yDim-2) ) {
		unsigned int i_stop_top = 1;
		unsigned int i_stop_bottom = ((bush->get_rec()).height-1);
		if ( ((bush->get_ref()).y+(bush->get_rec()).height) >= (M_yDim-1) ) {
			if ( ((int)M_yDim-1-(bush->get_ref()).y-2) < 0 ) i_stop_bottom = 0;
			else i_stop_bottom = M_yDim - 1 - (bush->get_ref()).y - 2;
		}
		if ( (bush->get_ref()).y < 0 ) {
			i_stop_top = -(bush->get_ref()).y;
		} else {
			for (unsigned int i=1; i < ((bush->get_rec()).width-1); ++i) {
				mvprintw(M_yOffset+(bush->get_ref()).y, M_xOffset+(bush->get_ref()).x+i, "-");
			}
			mvprintw(M_yOffset+(bush->get_ref()).y, M_xOffset+(bush->get_ref()).x, "*");
			mvprintw(M_yOffset+(bush->get_ref()).y, M_xOffset+(bush->get_ref()).x+((bush->get_rec()).width-1), "*");
		}

		for (unsigned int i=i_stop_bottom; i>=i_stop_top; --i) {
			mvprintw(M_yOffset+(bush->get_ref()).y+i, M_xOffset+(bush->get_ref()).x, "|");
			mvprintw(M_yOffset+(bush->get_ref()).y+i, M_xOffset+(bush->get_ref()).x+((bush->get_rec()).width-1), "|");
		}

		if ( (bush->get_ref()).y+(bush->get_rec()).height > 0 and (bush->get_ref()).y+(bush->get_rec()).height < (M_yDim-2) ) {
			for (unsigned int i=1; i < ((bush->get_rec()).width-1); ++i) {
				mvprintw(M_yOffset+(bush->get_ref()).y+((bush->get_rec()).height-1), M_xOffset+(bush->get_ref()).x+i, "-");
			}
			mvprintw(M_yOffset+(bush->get_ref()).y+((bush->get_rec()).height-1), M_xOffset+(bush->get_ref()).x, "*");
			mvprintw(M_yOffset+(bush->get_ref()).y+((bush->get_rec()).height-1), M_xOffset+(bush->get_ref()).x+((bush->get_rec()).width-1), "*");
		}
	}
	attroff(COLOR_PAIR(2)); // disable bush color pair
}

void Sketcher :: Pencil (SpaceSheep* sheep)
{
	mvprintw(M_yOffset+(sheep->get_ref()).y-sheep->get_fatness(),
			M_xOffset+(sheep->get_ref()).x, "O");
	mvprintw(M_yOffset+(sheep->get_ref()).y-sheep->get_fatness()+1,
			M_xOffset+(sheep->get_ref()).x-1, "*@*");
	for (unsigned int i=2; i < (sheep->get_fatness()+1); ++i) {
		for (unsigned int j=0; j < (2*i)+1; ++j) {
			mvprintw(M_yOffset+(sheep->get_ref()).y-sheep->get_fatness()+i,
					M_xOffset+(sheep->get_ref()).x-i+j, "@");
		}
	}
	for (unsigned int i=sheep->get_fatness(); i > 0 ; --i) {
		for (unsigned int j=0; j < (2*i)-1; ++j) {
			mvprintw(M_yOffset+(sheep->get_ref()).y+(sheep->get_fatness()-i+1),
					M_xOffset+(sheep->get_ref()).x-i+1+j, "@");
		}
	}
}

void Sketcher :: Rubber (RectObstacle* bush)
{
	for (unsigned int i=0; i < (bush->get_rec()).height; ++i) {
		if ( (bush->get_ref()).y+i != -1 and (bush->get_ref()).y+i != (M_yDim-2) ) {
			mvprintw(M_yOffset+(bush->get_ref()).y+i, M_xOffset+(bush->get_ref()).x, " ");
			mvprintw(M_yOffset+(bush->get_ref()).y+i, M_xOffset+(bush->get_ref()).x+((bush->get_rec()).width-1), " ");
		}
	}

	if ( (bush->get_ref()).y != -1 and (bush->get_ref()).y != (M_yDim-2) ) {
		for (unsigned int i=0; i < (bush->get_rec()).width; ++i) {
			mvprintw(M_yOffset+(bush->get_ref()).y, M_xOffset+(bush->get_ref()).x+i, " ");
		}
	}
	if ( (bush->get_ref()).y+((bush->get_rec()).height-1) != -1 and (bush->get_ref()).y+((bush->get_rec()).height-1) != (M_yDim-2) ) {
		for (unsigned int i=0; i < (bush->get_rec()).width; ++i) {
			mvprintw(M_yOffset+(bush->get_ref()).y+((bush->get_rec()).height-1), M_xOffset+(bush->get_ref()).x+i, " ");
		}
	}
}

void Sketcher :: Rubber (SpaceSheep* sheep)
{
	for (unsigned int i=0; i < (sheep->get_fatness()+1); ++i) {
		for (unsigned int j=0; j < (2*i)+1; ++j) {
			mvprintw(M_yOffset+(sheep->get_ref()).y-sheep->get_fatness()+i,
					M_xOffset+(sheep->get_ref()).x-i+j, " ");
		}
	}
	for (unsigned int i=sheep->get_fatness(); i > 0 ; --i) {
		for (unsigned int j=0; j < (2*i)-1; ++j) {
			mvprintw(M_yOffset+(sheep->get_ref()).y+(sheep->get_fatness()-i+1),
					M_xOffset+(sheep->get_ref()).x-i+1+j, " ");
		}
	}
}

void Sketcher :: Animation (RectObstacle* bush)
{
	Rubber(bush);
	bush->move();
	Pencil(bush);
}

void Sketcher :: Animation (SpaceSheep* sheep, bool dir)
{
	Rubber(sheep);
	sheep->move(dir);
	Pencil(sheep);
}
