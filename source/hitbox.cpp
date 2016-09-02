/*
 * File: hitbox.cpp
 *
 * Implementation of hitbox.h classes.
 */

#include "hitbox.h"

bool HitBox :: Overlap_RectRect(HitBoxRect& a, HitBoxRect& b)
{
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
	 */

	int a_x = (a.get_v()).x;
	int a_y = (a.get_v()).y;
	int b_x = (b.get_v()).x;
	int b_y = (b.get_v()).y;

	//we have to substract 1 from height and width, so (x_ref+width) 
	// is inside the hitbox
	//we have to cast width and height in 'int' for better math
	int a_w = (int)(a.get_rec()).width - 1 ;
	int a_h = (int)(a.get_rec()).height - 1 ;
	int b_w = (int)(b.get_rec()).width - 1 ;
	int b_h = (int)(b.get_rec()).height - 1 ;
	if ( a_w < 0 or b_w < 0 or a_h < 0 or b_h < 0 ) {
		throw "HitBox::OverlapRectRect() ERROR: width or height < 0";
	}

	//if the position it's the same they're always overlaped
	if ( a.get_v() == b.get_v() ) return true;

	if ( a_x <= b_x ) {
		if ( a_y <= b_y ) {
			if ( (a_x+a_w) >= b_x and (a_y+a_h) >= b_y ) return true;
			else return false;
		}
		// if ( a_y > b_y )
		else {
			if ( (a_x+a_w) >= b_x and a_y <= (b_y+b_h) ) return true;
			else return false;
		}
	}
	// if ( a_x > b_x )
	else {
		if ( a_y >= b_y ) {
			if ( (b_x+b_w) >= a_x and (b_y+b_h) >= a_y ) return true;
			else return false;
		}
		// if ( a_y < b_y )
		else {
			if ( (b_x+b_w) >= a_x and b_y <= (a_y+a_h) ) return true;
			else return false;
		}
	}
}

bool HitBox :: Overlap_RectCircle(HitBoxRect& r, HitBoxCircle& c)
{
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
	 *		C    | radius = height
	 *	   C#C   |
	 *    C#O#C  | 
	 *     C#C   |
	 *      C    |
	 *    _____
	 *    radius = width
	 *
	 *    radius = abs(C.x - O.x) + abs(C.y - O.y)
	 *    O = ref
	 */

	int r_x = (r.get_v()).x;
	int r_y = (r.get_v()).y;
	int c_x = (c.get_ref()).x;
	int c_y = (c.get_ref()).y;

	//we have to substract 1 from height and width, so (x_ref+width) 
	// is inside the hitbox
	int r_w = (int)(r.get_rec()).width - 1 ;
	int r_h = (int)(r.get_rec()).height - 1 ;
	int c_r = (int)c.get_radius();

	if ( r_w < 0 or r_h < 0 or c_r < 0 ) {
		throw "HitBox::OverlapRectCircle() ERROR: width, height or radius < 0";
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

bool HitBox :: Overlap_CircleCircle(HitBoxCircle& a, HitBoxCircle& b)
{
	/*
	 * This is a Circle:
	 *
	 *		C    | radius = height
	 *	   C#C   |
	 *    C#R#C  | 
	 *     C#C   |
	 *      C    |
	 *    _____
	 *    radius = width
	 *
	 *    radius = abs(C.x - R.x) + abs(C.y - R.y)
	 */

	int a_x = (a.get_ref()).x;
	int a_y = (a.get_ref()).y;
	int b_x = (b.get_ref()).x;
	int b_y = (b.get_ref()).y;

	//we have to substract 1 from height and width, so (x_ref+width) 
	// is inside the hitbox
	int a_r = (int)a.get_radius();
	int b_r = (int)b.get_radius();

	if ( a_r < 0 or b_r < 0 ) {
		throw "HitBox::Overlap_CircleCircle() ERROR: radius < 0";
	}

	if ( std::abs(a_x-b_x) + std::abs(a_y-b_y) <= (a_r+b_r) ) return true;
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
