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

#include <cmath>

struct position
{
	int x;
	int y;
	position& operator= (const position& p) {
		x = p.x;
		y = p.y;
		return *this;
	}
	bool operator== (const position& p) {
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

class HitBox
{
	public:
		virtual bool Overlap(HitBox&) = 0;
		static bool Overlap(HitBox& a,HitBox& b) { return a.Overlap(b); }
		virtual bool Overlap(HitBoxRect&) = 0;
		virtual bool Overlap(HitBoxCircle&) = 0;

	protected:
		bool Overlap_RectRect(HitBoxRect& a, HitBoxRect& b);
		bool Overlap_RectCircle(HitBoxRect& r, HitBoxCircle& c);
		bool Overlap_CircleCircle(HitBoxCircle& a, HitBoxCircle& b);
};

class HitBoxRect : public HitBox
{
	public:
		HitBoxRect(position v, rectangle rec);
		HitBoxRect (int x, int y, unsigned int width, unsigned int height);
		~HitBoxRect(){}

		position get_v(){ return m_v; }
		void set_v(position& new_v){ m_v = new_v; }
		rectangle get_rec(){ return m_rec; }

		virtual bool Overlap(HitBox& b)     { return b.Overlap(*this); }
		virtual bool Overlap(HitBoxRect& b) { Overlap_RectRect(*this,b); }
		virtual bool Overlap(HitBoxCircle& b) { Overlap_RectCircle(*this,b); }

	private:
		position m_v;
		rectangle m_rec;
};

class HitBoxCircle : public HitBox
{
	public:
		HitBoxCircle(position ref, unsigned int radius);
		HitBoxCircle(int x, int y, unsigned int radius);
		~HitBoxCircle() { }

		position get_ref(){ return m_ref; }
		void set_ref(position& new_ref){ m_ref = new_ref; }
		unsigned int get_radius(){ return m_radius; }

		virtual bool Overlap(HitBox& b)     { return b.Overlap(*this); }
		virtual bool Overlap(HitBoxRect& b) { Overlap_RectCircle(b,*this); }
		virtual bool Overlap(HitBoxCircle& b) { Overlap_CircleCircle(*this,b); }

	private:
		unsigned int m_radius;
		position m_ref;
};

#endif // _HITBOX_H_
