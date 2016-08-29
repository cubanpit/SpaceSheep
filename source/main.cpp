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
#include <iostream>

int main (int argc, char **argv)
{
	initscr(); // start the terminal world [ncurses]
	curs_set(0); // hide cursor position [ncurses]
	noecho(); // to hide input obtained with getch() [ncurses]

	Engine game(100,30);
	game.run();

	endwin(); // end terminal world [ncurses]
	return 0;
}
