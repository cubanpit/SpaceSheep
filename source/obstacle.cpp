/*
 * File: obstacle.cpp
 *
 * Implementation of obstacle.h classes.
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

void RectObstacle :: move()
{
	position tmp_ref;
	tmp_ref.x = get_ref().x;
	tmp_ref.y = get_ref().y + 1 ;
	set_ref(tmp_ref);
	m_hitbox.set_v(tmp_ref);
}

SpaceSheep :: SpaceSheep(position ref, unsigned int fatness):
	Obstacle(ref), m_hitbox(ref, fatness)
{
	m_fatness = fatness;
}

SpaceSheep :: SpaceSheep(int x, int y, unsigned int fatness):
	Obstacle(x,y), m_hitbox(x,y,fatness)
{
	m_fatness = fatness;
}

void SpaceSheep :: move(bool dir)
{
	position tmp_ref;
	tmp_ref.y = get_ref().y;
	if ( dir ) tmp_ref.x = get_ref().x + 1 ;
	else tmp_ref.x = get_ref().x - 1 ;
	set_ref(tmp_ref);
	m_hitbox.set_ref(tmp_ref);
}
