/*
 * File: main.cpp
 * 
 * A beautiful game with a SpaceSheep as main character.
 *
 * Authors: 
 *	Martina Crippa 				<martina.crippa2@studenti.unimi.it>
 *	Pietro Francesco Fontana 	<pietrofrancesco.fontana@studenti.unimi.it>
 */

#include "engine.h"

int main (int argc, char **argv)
{
	initscr(); // start the terminal world [ncurses]
	curs_set(0); // hide cursor position [ncurses]
	noecho(); // to hide input obtained with getch() [ncurses]

	try {
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
