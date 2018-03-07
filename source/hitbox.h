/*
 * SpaceSheep, ncurses game. Code is hosted on GitHub.
 *
 * File: hitbox.h
 *
 * Header containing definition of position and rectangle struct, definition of
 *  HitBox class.
 * HitBox defines physic limits of objects and handle collision between them.
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

#ifndef _HITBOX_H_
#define _HITBOX_H_

#include <cstdlib> // abs

struct position
{
	int x;
	int y;
	position& operator= (const position& p) {
		x = p.x;
		y = p.y;
		return *this;
	}
	bool operator== (const position& p) const {
		if ( x == p.x and y == p.y ) return true;
		else return false;
	}
};

struct rectangle
{
	unsigned int width;
	unsigned int height;
	rectangle& operator= (const rectangle& r){
		width = r.width;
		height = r.height;
		return *this;
	}
};

class HitBoxRect;
class HitBoxCircle;

bool overlap_RectRect(HitBoxRect& a, HitBoxRect& b);
bool overlap_RectCircle(HitBoxRect& r, HitBoxCircle& c);
bool overlap_CircleCircle(HitBoxCircle& a, HitBoxCircle& b);

class HitBoxRect
{
	public:
		HitBoxRect(position v, rectangle rec);
		HitBoxRect (int x, int y, unsigned int width, unsigned int height);
		~HitBoxRect(){}

		const position& get_v() const { return m_v; }
		void set_v(position& new_v){ m_v = new_v; }
		const rectangle& get_rec() const { return m_rec; }

		bool overlap(HitBoxRect& b) { return overlap_RectRect(*this,b); }
		bool overlap(HitBoxCircle& b) { return overlap_RectCircle(*this,b); }

	private:
		position m_v;
		rectangle m_rec;
};

class HitBoxCircle
{
	public:
		HitBoxCircle(position ref, unsigned int radius);
		HitBoxCircle(int x, int y, unsigned int radius);
		~HitBoxCircle() { }

		const position& get_ref() const { return m_ref; }
		void set_ref(position& new_ref){ m_ref = new_ref; }
		unsigned int get_radius() const { return m_radius; }

		bool overlap(HitBoxRect& b) { return overlap_RectCircle(b,*this); }
		bool overlap(HitBoxCircle& b) { return overlap_CircleCircle(*this,b); }

	private:
		position m_ref;
		unsigned int m_radius;
};

#endif // _HITBOX_H_

/*
 *   This is a Rectangle:
 *
 *   V-------------*
 *   |             |
 *   |             |
 *   *-------------*
 *
 *   V is always the top-left corner
 *   In our coordinates system this point has the minimum of x and y value
 *
 *   This is a Circle:
 *
 *      C    | radius = height/2
 *     C#C   |
 *    C#R#C  -
 *     C#C   |
 *      C    |
 *    __|__
 *    radius = width/2
 *
 *    radius = abs(C.x - R.x) + abs(C.y - R.y)
 *    R = ref
 */

// vim: set noexpandtab:
