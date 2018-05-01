/*
 * SpaceSheep, ncurses game. Code is hosted on GitHub.
 *
 * File: hitbox.cpp
 *
 * Implementation of hitbox.h classes.
 *
 * Authors:
 *  Martina Crippa 				<martina.crippa2@studenti.unimi.it>
 *  Pietro Francesco Fontana 	<pietrofrancesco.fontana@studenti.unimi.it>
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

bool overlap_RectRect(HitBoxRect& a, HitBoxRect& b)
{
  //we have to cast width and height in 'int' for better math

  position a_v = a.get_v();
  position b_v = b.get_v();
  rectangle a_rec = a.get_rec();
  rectangle b_rec = b.get_rec();

  if (a_v.x < b_v.x + (int)b_rec.width
  		and b_v.x < a_v.x + (int)a_rec.width
  		and a_v.y < b_v.y + (int)b_rec.height
  		and b_v.y < a_v.y + (int)a_rec.height) {
  	return true;
  } else {
  	return false;
  }
}

bool overlap_RectCircle(HitBoxRect& r, HitBoxCircle& c)
{
	//We have to substract 1 from height and width, so (x_ref+width)
	// is inside the hitbox.

	//we have to cast width and height in 'int' for better math

	position r_v = r.get_v();
	int r_w = (int)(r.get_rec()).width - 1;
	int r_h = (int)(r.get_rec()).height - 1;

	position c_c = c.get_ref();
	int c_r = (int)c.get_radius();

	if ( r_w <= 0 or r_h <= 0 or c_r <= 0 ) {
		throw "HitBox::overlapRectCircle() ERROR: width, height or radius <= 0";
	}

	// if they are too far away do not check further
	// (this can be done in a more useful way probably)
	if (abs(2*r_v.x + r_w - 2*c_c.x) + abs(2*r_v.y + r_h - 2*c_c.y)
			> 2*c_r + r_w + r_h) {
		return false;
	}

	position r_pos[4];
	r_pos[0] = r_v;
	r_pos[1] = {r_v.x + r_w, r_v.y};
	r_pos[2] = {r_v.x + r_w, r_v.y + r_h};
	r_pos[3] = {r_v.x, r_v.y + r_h};

	position c_pos[4];
	c_pos[0] = {c_c.x, c_c.y - c_r};
	c_pos[1] = {c_c.x + c_r, c_c.y};
	c_pos[2] = {c_c.x, c_c.y + c_r};
	c_pos[3] = {c_c.x - c_r, c_c.y};

	// one of 'circle vertices' is inside the rectangle
	for (short unsigned int i=0; i<4; ++i) {
		if (r_pos[0].x <= c_pos[i].x and c_pos[i].x <= r_pos[1].x
				and r_pos[0].y <= c_pos[i].y and c_pos[i].y <= r_pos[3].y) {
			return true;
		}
	}

	// one of the rectangle vertices is inside the circle
	for (short unsigned int i=0; i<4; ++i) {
		if (abs(c_c.x - r_pos[i].x) + abs(c_c.y - r_pos[i].y) <= c_r) {
			return true;
		}
	}

	return false;
}

bool overlap_CircleCircle(HitBoxCircle& a, HitBoxCircle& b)
{
  position a_pos = a.get_ref();
  position b_pos = b.get_ref();

  int a_r = (int)a.get_radius();
  int b_r = (int)b.get_radius();

  if ( a_r <= 0 or b_r <= 0 ) {
  	throw "HitBox::overlap_CircleCircle() ERROR: radius <= 0";
  }

  if ( abs(a_pos.x - b_pos.x) + abs(a_pos.y - b_pos.y) <= (a_r+b_r) ) {
  	return true;
  }
  else {
  	return false;
  }
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

// vim: set expandtab ts=2 sw=2:
