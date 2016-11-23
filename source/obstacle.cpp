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
	Obstacle(v), m_hitbox(v, rec)
{
	m_rec = rec;
}

RectObstacle :: RectObstacle
(int x, int y, unsigned int width, unsigned int height):
	Obstacle(x,y), m_hitbox(x,y,width,height)
{
	m_rec.width = width;
	m_rec.height = height;
}

void RectObstacle :: drop()
{
	position tmp_ref;
	tmp_ref.x = get_ref().x;
	tmp_ref.y = get_ref().y + 1 ;
	set_ref(tmp_ref);
	m_hitbox.set_v(tmp_ref);
}

CircleObstacle :: CircleObstacle(int x, int y, unsigned int fatness):
	Obstacle(x,y), m_hitbox(x,y,fatness)
{
	m_fatness = fatness;
}

CircleObstacle :: CircleObstacle(position ref, unsigned int fatness):
	Obstacle(ref), m_hitbox(ref, fatness)
{
	m_fatness = fatness;
}

SpaceSheep :: SpaceSheep(int x, int y, unsigned int fatness):
	CircleObstacle(x, y, fatness)
{
}

SpaceSheep :: SpaceSheep(position ref, unsigned int fatness):
	CircleObstacle(ref, fatness)
{
}

void SpaceSheep :: move(char dir)
{
	position tmp_ref;
	tmp_ref.y = get_ref().y;
	if ( dir == 'r' ) tmp_ref.x = get_ref().x + 2 ;
	else tmp_ref.x = get_ref().x - 2 ;
	set_ref(tmp_ref);
	m_hitbox.set_ref(tmp_ref);
}

void SpaceSheep :: move(unsigned int x)
{
	position tmp_ref;
	tmp_ref.y = get_ref().y;
	tmp_ref.x = x;
	set_ref(tmp_ref);
	m_hitbox.set_ref(tmp_ref);
}

SpaceBull :: SpaceBull(int x, int y, unsigned int fatness):
	CircleObstacle(x, y, fatness)
{
}

SpaceBull :: SpaceBull(position ref, unsigned int fatness):
	CircleObstacle(ref, fatness)
{
}

void SpaceBull :: drop()
{
	position tmp_ref;
	tmp_ref.x = get_ref().x;
	tmp_ref.y = get_ref().y + 1 ;
	set_ref(tmp_ref);
	m_hitbox.set_ref(tmp_ref);
}
