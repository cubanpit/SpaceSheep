/* 
 * SpaceSheep, ncurses game. Code is hosted on GitHub.
 *
 * File: obstacle.h
 *
 * Class that defines obstacles, every object has a HitBox member to handle
 *  collision between obstacles.
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

#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "hitbox.h"

class Obstacle
{
	public:
		Obstacle(position ref){ m_ref = ref; }
		Obstacle(int x, int y){ m_ref.x = x; m_ref.y = y; }
		~Obstacle(){}

		position get_ref(){ return m_ref; }
		void set_ref(position& new_ref){ m_ref = new_ref; }

	private:
		position m_ref; //riferimento della figura
};

class RectObstacle : public Obstacle
{
	public:
		RectObstacle(int x, int y, unsigned int width, unsigned int height);
		RectObstacle(position v, rectangle rec);
		~RectObstacle(){ }

		HitBoxRect& get_hitbox(){ return m_hitbox; }
		rectangle get_rec(){ return m_rec; }
		void set_hitbox(HitBoxRect& tmp_hitbox) { m_hitbox = tmp_hitbox; }
		void move();

	private:
		HitBoxRect m_hitbox;
		rectangle m_rec;

};

// cause everything is more cool in space!
class SpaceSheep : public Obstacle
{
	public:
		SpaceSheep(int x, int y, unsigned int fatness = 1);
		SpaceSheep(position ref, unsigned int fatness = 1);

		HitBoxCircle& get_hitbox(){ return m_hitbox; }
		void set_fatness(int fatness){ m_fatness = fatness; }
		unsigned int get_fatness(){ return m_fatness; }
		void move(bool dir);

	private:
		HitBoxCircle m_hitbox;
		unsigned int m_fatness;

};

#endif // _OBSTACLE_H_
