/*
 * SpaceSheep, ncurses game. Code is hosted on GitHub.
 *
 * File: main.cpp
 * 
 * A beautiful game with a SpaceSheep as main character.
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

int main (int argc, char **argv)
{
	initscr(); // start the terminal world [ncurses]
	curs_set(0); // hide cursor position [ncurses]
	noecho(); // to hide input obtained with getch() [ncurses]

	try {
		if ( !has_colors() ) {
			throw "main() ERROR: this terminal doesn't support colors properly. "
				"Take five bucks and go buy a better computer!";
		}
		start_color(); // enable colors in this window [ncurses]
		Engine game(100,30);
		game.run();
		endwin(); // end terminal world [ncurses]
	}
	catch ( const char* msg ) {
		std::cerr << msg << std::endl;
	}
	catch (std::bad_alloc& ba)
	{
		std::cerr << "main() bad_alloc caught: " << ba.what() << std::endl;
		exit (EXIT_FAILURE);
	}

	return 0;
}
