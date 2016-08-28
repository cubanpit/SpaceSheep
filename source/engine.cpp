/*
 * File: engine.cpp
 *
 * Implementation of classes and functions declared in engine.h
 */

#include "engine.h"

Engine::Engine(unsigned int xDim, unsigned int yDim):m_artist(xDim,yDim)
{ }

void Engine::run()
{
	m_artist.GameTable();
	SpaceSheep* sheep = new SpaceSheep(50,28-2,2);
	m_artist.Pencil(sheep);
	char ch; //needed for sheep movement
	char left_mov = 'j', right_mov = 'l';
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
	std::chrono::duration<int,std::milli> dt_sheep(dt_uint_sheep);

	/*
	 * ANIMATION AND HITBOX
	 */
	std::vector <RectObstacle*> bushes;

	// Random stuff
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(0,m_artist.get_xDim());

	unsigned int production = 14; //set vertical frequency of obstacle
	unsigned int count = 0; //counts refreshes
	unsigned int w_d = 4; //margin between bushes distance and sheep width
	unsigned int w_tot = 50; //minimal total width of bushes on the same row
	unsigned int w_m = 20; //minimal width of bushes
	unsigned int w_r = 22; //range of width, w_m+(w_r-1)=maximum width
	unsigned int h_m = 3; //minimal height of bushes
	unsigned int h_r = 4; //range of height
	unsigned int w = 0, h = 0;
	int x = 0, y = 0;
	bool ctrl = false; //to check obstacle spawn
	bool dead = false;

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
	 * It's not so simple to respect all this rules.
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
	 *
	 * This simple 'if' statement check that conditions can't cause a loop in
	 *  obstacle creation.
	 */
	if (   !((((int)m_artist.get_xDim()/2) - ((int)w_m/2) - ((int)(*sheep).get_fatness()*2+1+(int)w_d)) > w_m 
		 and (((int)m_artist.get_xDim()/2) - ((int)w_m/2) - ((int)(*sheep).get_fatness()*2+1+(int)w_d)) < ((int)w_m+(int)w_r-1)
		 and (((int)m_artist.get_xDim()/2) - (((int)w_m+int(w_r)-1)/2) - ((int)(*sheep).get_fatness()*2+1+(int)w_d)) > w_m 
		 and (((int)m_artist.get_xDim()/2) - (((int)w_m+int(w_r)-1)/2) - ((int)(*sheep).get_fatness()*2+1+(int)w_d)) < ((int)w_m+(int)w_r-1)) ) {
		std::cerr << "Engine::run() ERROR: minimum and range of obstacle's width can cause an infinite loop, change their value." << std::endl;
		dead = true;
	}

	while (!dead) {
		if ( !(count%production) ) {
			for ( unsigned int i=0; i<2; ++i) {
				RectObstacle* tmp_bush = new RectObstacle(x,y,w,h);
				while (!ctrl) {
					w = (distribution(generator) % w_r) + w_m; //(dist%range)+min
					x = distribution(generator);
					if ( (x + w) < (int)m_artist.get_xDim() ) ctrl = true;
					if ( i > 0 ) {
						if ( ctrl ) {
							if ( ((x >= (((*(bushes.back())).get_ref()).x + (int)((*(bushes.back())).get_rec()).width)) and 
										(x - (((*(bushes.back())).get_ref()).x +
											  (int)((*(bushes.back())).get_rec()).width)) <
										((int)(*sheep).get_fatness()*2+1+(int)w_d)) ) ctrl = false;
							if ( (((x+(int)w) <= ((*(bushes.back())).get_ref()).x) and 
										(((*(bushes.back())).get_ref()).x - (x + (int)w)) <
										((int)(*sheep).get_fatness()*2+1+(int)w_d)) ) ctrl = false;
						}
						if ( ctrl ) {
							if ( (int)((*(bushes.back())).get_rec()).width+(int)w < w_tot ) {
								ctrl = false;
							}
						}
					}

					h = (distribution(generator) % h_r) + h_m ;
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
				ctrl = false; //reset control bool
				bushes.push_back(tmp_bush); //only good obstacle are stored

				// Old obstacles need to be erased from the vector
				for (auto it = bushes.begin(); it != bushes.end(); it++) {
					if ( ((*(*it)).get_ref()).y > (int)m_artist.get_yDim() ) {
						delete *it;
						bushes.erase(it);
					}
				}
			}
		}
		++count;

		for (auto it = bushes.begin(); it != bushes.end(); it++) {
			m_artist.Pencil(*it);
		}

		for (auto it = bushes.begin(); it != bushes.end(); it++) {
			m_artist.Animation(*it);
		}

		for (int i=0; i<dt_uint_obstacle; i+=dt_uint_sheep) {
			ch = getch();
			if ( ch == left_mov and  (((*sheep).get_ref()).x - 
							(int)(*sheep).get_fatness()) > 1 ) {
					m_artist.Animation(sheep,0);
			}
			else if ( ch == right_mov and (((*sheep).get_ref()).x + 
					  (int)(*sheep).get_fatness()) < ((int)m_artist.get_xDim() - 2) ) {
					m_artist.Animation(sheep,1);
			}

			for (auto it = bushes.begin(); it != bushes.end(); it++) {
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
}

