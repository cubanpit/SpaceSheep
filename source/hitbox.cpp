/*
 * SpaceSheep, ncurses game. Code is hosted on GitHub.
 *
 * File: hitbox.cpp
 *
 * Implementation of hitbox.h classes.
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

#include "hitbox.h"

bool HitBox :: overlap_RectRect(HitBoxRect& a, HitBoxRect& b)
{
	//we have to substract 1 from height and width, so (x_ref+width)
	// is inside the hitbox
	//we have to cast width and height in 'int' for better math

	position a_pos[4];
	a_pos[0] = a.get_v();
	a_pos[1] = {(a.get_v()).x+(int)(a.get_rec()).width-1,(a.get_v()).y};
	a_pos[2] = {(a.get_v()).x+(int)(a.get_rec()).width-1,
		(a.get_v()).y+(int)(a.get_rec()).height-1};
	a_pos[3] = {(a.get_v()).x,(a.get_v()).y+(int)(a.get_rec()).height-1};

	position b_pos[4];
	b_pos[0] = b.get_v();
	b_pos[1] = {(b.get_v()).x+(int)(b.get_rec()).width-1,(b.get_v()).y};
	b_pos[2] = {(b.get_v()).x+(int)(b.get_rec()).width-1,
		(b.get_v()).y+(int)(b.get_rec()).height-1};
	b_pos[3] = {(b.get_v()).x,(b.get_v()).y+(int)(b.get_rec()).height-1};

	for (int i=0; i<4; ++i) {
		// check if 'a' is inside 'b'
		if (!(a_pos[i].x < b_pos[0].x) != !(a_pos[i].x <= b_pos[2].x)) {
			if (!(a_pos[i].y < b_pos[0].y) != !(a_pos[i].y <= b_pos[2].y)) {
				return true;
			}
		}
		// check if 'b' is inside 'a'
		if (!(b_pos[i].x < a_pos[0].x) != !(b_pos[i].x <= a_pos[2].x)) {
			if (!(b_pos[i].y < a_pos[0].y) != !(b_pos[i].y <= a_pos[2].y)) {
				return true;
			}
		}
	}
	return false;
}

bool HitBox :: overlap_RectCircle(HitBoxRect& r, HitBoxCircle& c)
{
	int r_x = (r.get_v()).x;
	int r_y = (r.get_v()).y;
	int c_x = (c.get_ref()).x;
	int c_y = (c.get_ref()).y;

	//We have to substract 1 from height and width, so (x_ref+width)
	// is inside the hitbox.
	int r_w = (int)(r.get_rec()).width - 1 ;
	int r_h = (int)(r.get_rec()).height - 1 ;
	int c_r = (int)c.get_radius();

	if ( r_w <= 0 or r_h <= 0 or c_r <= 0 ) {
		throw "HitBox::overlapRectCircle() ERROR: width, height or radius <= 0";
	}

	if ( r_x >= c_x ) {
		if ( r_y >= c_y ) {
			if ( ((r_y-c_y)+(r_x-c_x)) <= c_r ) return true;
			else return false;
		}
		else if ( r_y < c_y and c_y < (r_y+r_h) ) {
			if ( (r_x-c_x) <= c_r ) return true;
			else return false;
		}
		// if ( c_y >= (r_y+r_h) )
		else {
			if ( ((c_y-(r_y+r_h))+(r_x-c_x)) <= c_r ) return true;
			else return false;
		}
	}
	else if ( r_x < c_x and c_x < (r_x+r_w) ) {
		if ( c_y < r_y and (r_y - c_y) <= c_r ) return true;
		else if ( c_y > (r_y+r_h) and (c_y - (r_y+r_h)) <= c_r ) return true;
		else if ( c_y > r_y and c_y < (r_y+r_h) ) return true;
		else return false;
	}
	// if ( c_x > (r_x+r_w) )
	else {
		if ( r_y >= c_y ) {
			if ( ((r_y-c_y)+(c_x-(r_x+r_w))) <= c_r ) return true;
			else return false;
		}
		else if ( r_y < c_y and c_y < (r_y+r_h) ) {
			if ( (c_x-(r_x+r_w)) <= c_r ) return true;
			else return false;
		}
		// if ( c_y >= (r_y+r_h) )
		else {
			if ( ((c_y-(r_y+r_h))+(c_x-(r_x+r_w))) <= c_r ) return true;
			else return false;
		}
	}
}

bool HitBox :: overlap_CircleCircle(HitBoxCircle& a, HitBoxCircle& b)
{
	int a_x = (a.get_ref()).x;
	int a_y = (a.get_ref()).y;
	int b_x = (b.get_ref()).x;
	int b_y = (b.get_ref()).y;

	int a_r = (int)a.get_radius();
	int b_r = (int)b.get_radius();

	if ( a_r <= 0 or b_r <= 0 ) {
		throw "HitBox::overlap_CircleCircle() ERROR: radius <= 0";
	}

	if ( abs(a_x-b_x) + abs(a_y-b_y) <= (a_r+b_r) ) return true;
	else return false;
}

HitBoxRect :: HitBoxRect (int x, int y, unsigned int width, unsigned int height)
{
	if ( width == 0 or height == 0 ) {
		throw "HitBoxRect::HitBoxRect() ERROR: width or height = 0";
	}
	m_v.x = x;
	m_v.y = y;
	m_rec.width = width;
	m_rec.height = height;
}

HitBoxRect :: HitBoxRect (position v, rectangle rec)
{
	if ( rec.width == 0 or rec.height == 0 ) {
		throw "HitBoxRect::HitBoxRect() ERROR: width or height = 0";
	}
	m_v = v;
	m_rec = rec;
}

HitBoxCircle :: HitBoxCircle(int x, int y, unsigned int radius)
{
	if ( radius == 0 ) {
		throw "HitBoxCircle::HitBoxCircle() ERROR: radius = 0";
	}
	m_radius = radius;
	m_ref.x = x;
	m_ref.y = y;
}

HitBoxCircle :: HitBoxCircle (position ref, unsigned int radius)
{
	if ( radius == 0 ) {
		throw "HitBoxCircle::HitBoxCircle() ERROR: radius = 0";
	}
	m_radius = radius;
	m_ref = ref;
}
