/*
 * SpaceSheep, ncurses game. Code is hosted on GitHub.
 *
 * File: obstacle.cpp
 *
 * Implementation of obstacle.h classes.
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

#include "obstacle.h"

RectObstacle :: RectObstacle(position v, rectangle rec):
	m_hitbox(v, rec), m_v(v), m_rec(rec)
{
}

RectObstacle :: RectObstacle
(int x, int y, unsigned int width, unsigned int height):
	m_hitbox(x,y,width,height), m_v{x,y}, m_rec{width,height}
{
}

void RectObstacle :: drop()
{
	++m_v.y;
	m_hitbox.set_v(m_v);
}

CircleObstacle :: CircleObstacle(int x, int y, unsigned int radius):
	m_hitbox(x,y,radius), m_ref{x,y}, m_radius(radius)
{
}

CircleObstacle :: CircleObstacle(position ref, unsigned int radius):
	m_hitbox(ref, radius), m_ref(ref), m_radius(radius)
{
}

SpaceSheep :: SpaceSheep(int x, int y, unsigned int radius):
	CircleObstacle(x, y, radius)
{
}

SpaceSheep :: SpaceSheep(position ref, unsigned int radius):
	CircleObstacle(ref, radius)
{
}

void SpaceSheep :: move(char dir)
{
	if ( dir == 'r' ) m_ref.x += 2 ;
	else if ( dir == 'l' ) m_ref.x -= 2 ;
	m_hitbox.set_ref(m_ref);
}

void SpaceSheep :: move_to(unsigned int x)
{
	m_ref.x = x;
	m_hitbox.set_ref(m_ref);
}

SpaceBull :: SpaceBull(int x, int y, unsigned int radius):
	CircleObstacle(x, y, radius)
{
}

SpaceBull :: SpaceBull(position ref, unsigned int radius):
	CircleObstacle(ref, radius)
{
}

void SpaceBull :: drop()
{
	++m_ref.y;
	m_hitbox.set_ref(m_ref);
}

// vim: set noexpandtab:
