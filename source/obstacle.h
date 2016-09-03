/* 
 * File: obstacle.h
 *
 * Class that defines obstacles, every object has a HitBox member to handle
 *  collision between obstacles.
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
		HitBoxRect m_hitbox;
	
		RectObstacle(int x, int y, unsigned int width, unsigned int height);
		RectObstacle(position v, rectangle rec);
		~RectObstacle(){ }

		HitBoxRect get_hitbox(){ return m_hitbox; }
		rectangle get_rec(){ return m_rec; }
		void set_hitbox(HitBoxRect& tmp_hitbox) { m_hitbox = tmp_hitbox; }
		void move();

	private:
		rectangle m_rec;

};

// cause everything is more cool in space!
class SpaceSheep : public Obstacle
{
	public:
		HitBoxCircle m_hitbox;

		SpaceSheep(int x, int y, unsigned int fatness = 1);
		SpaceSheep(position ref, unsigned int fatness = 1);

		HitBoxCircle get_hitbox(){ return m_hitbox; }
		void set_fatness(int fatness){ m_fatness = fatness; }
		unsigned int get_fatness(){ return m_fatness; }
		void move(bool dir);

	private:
		unsigned int m_fatness;

};

#endif // _OBSTACLE_H_
