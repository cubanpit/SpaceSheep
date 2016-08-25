#include <unistd.h> // useless if we use std::this_thread
#include <random> // std::default_random_engine , std::uniform_int_distribution
#include <chrono> // std::chrono::system_clock
#include <thread> // std::this_thread::sleep_until
#include <vector> //std::vector
#include <iostream>
#include "sketcher.h"

int main (int argc, char **argv)
{
	/*
	 * INTRODUCTION
	 */
	initscr(); // start the terminal world [ncurses]
	curs_set(0); // hide cursor position [ncurses]
	timeout(0); // getch() does not wait for input [ncurses]
	noecho(); // to hide input obtained with getch() [ncurses]

	Sketcher game(100,30);
	game.GameTable();
	refresh();

	SpaceSheep* sheep = new SpaceSheep(120,28-2,2);
	game.Artist(sheep);
	refresh();

	/*
	 * TIME STUFF
	 */
	// WARNING: clocks goes on during execution, if you use sleep_until(20:00)
	//  for obstacle and sleep(19:00) for sheep, the second will be ignored,
	//  time has passed!
	// SOLUTION: reassign time_point before using it, if necessary.
	std::chrono::system_clock::time_point t_track_sheep = std::chrono::system_clock::now();
	unsigned int dt_uint_obstacle = 150;
	unsigned int dt_uint_sheep = 10; 
	std::chrono::duration<int,std::milli> dt_obstacle(dt_uint_obstacle);
	std::chrono::duration<int,std::milli> dt_sheep(dt_uint_sheep);

	/*
	 * ANIMATION AND HITBOX
	 */
	std::vector <RectObstacle*> bushes;
	unsigned int w = 0, h = 0;
	int x = 0, y = 0;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(0,100);
	bool ctrl = false; //to check obstacle spawn
	bool dead = false;
	unsigned int production = 14;
	unsigned int count = 0;
	char ch;
	while (!dead) {
		if ( !(count%production) ) {
			for ( unsigned int i=0; i<2; ++i) {
				RectObstacle* tmp_bush = new RectObstacle(x,y,w,h);
				while (!ctrl) {
					w = (distribution(generator) % 22) + 20;
					x = distribution(generator);
					if ( (x + w) < 100 ) ctrl = true;
					if ( i > 0 ) {
						if ( ctrl ) {
							if ( ((x >= (((*(bushes.back())).get_ref()).x + (int)((*(bushes.back())).get_rec()).width)) and 
										(x - (((*(bushes.back())).get_ref()).x +
											  (int)((*(bushes.back())).get_rec()).width)) <
										((int)(*sheep).get_fatness()*2+7)) ) ctrl = false;
							if ( (((x+(int)w) <= ((*(bushes.back())).get_ref()).x) and 
										(((*(bushes.back())).get_ref()).x - (x + (int)w)) <
										((int)(*sheep).get_fatness()*2+5)) ) ctrl = false;
						}
						if ( ctrl ) {
							if ( (int)((*(bushes.back())).get_rec()).width+(int)w < 50 ) {
								ctrl = false;
							}
						}
					}

					h = (distribution(generator) % 4) + 3 ;
					y = -h ;
					RectObstacle* t_b = new RectObstacle(x,y,w,h);
					delete tmp_bush;
					tmp_bush = t_b;

					if ( ctrl ) {
						for (auto it = bushes.begin(); it != bushes.end(); it++) {
							if ( ((*(*it)).m_hitbox).Overlap((*tmp_bush).m_hitbox) ) {
								ctrl = false;
								break;
							}
						}
					}
				}
				ctrl = false;
				bushes.push_back(tmp_bush);

				for (auto it = bushes.begin(); it != bushes.end(); it++) {
					if ( ((*(*it)).get_ref()).y > (int)game.get_yDim() ) {
						delete *it;
						bushes.erase(it);
					}
				}
			}
		}
		++count;

		for (auto it = bushes.begin(); it != bushes.end(); it++) {
			game.Artist(*it);
		}
		refresh();

		for (auto it = bushes.begin(); it != bushes.end(); it++) {
			game.Animation(*it);
		}
		refresh();

		for (int i=0; i<dt_uint_obstacle; i+=dt_uint_sheep) {
			ch = getch();
			if ( ch == 'j' ) game.Animation(sheep,0);
			else if ( ch == 'l' ) game.Animation(sheep,1);

			for (auto it = bushes.begin(); it != bushes.end(); it++) {
				//game.Animation(sheep,1);
				if ( ((*(*it)).m_hitbox).Overlap((*sheep).m_hitbox) or
						((*(*it)).m_hitbox).Overlap((*sheep).m_hitbox) ) {

					dead = true;
					break;
				}
			}
			refresh();
			t_track_sheep += dt_sheep;
			std::this_thread::sleep_until(t_track_sheep);
		}
		refresh();
	}

	endwin(); // end terminal world [ncurses]
	return 0;
}
