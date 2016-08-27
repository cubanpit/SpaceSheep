/*
 * File: sketcher.cpp
 *
 * Implementation of sketcher.h classes.
*/

#include "sketcher.h"

Sketcher :: Sketcher (unsigned int xDim, unsigned int yDim)
{
	M_xDim = xDim;
	M_yDim = yDim;

	if (COLS > M_xDim and LINES > M_yDim) {
		// (Offset+1) because we want all body drawn without overlapping 
		//  the game table
		M_xOffset = ((COLS - M_xDim) / 2) + 1 ;
		M_yOffset = ((LINES - M_yDim) / 2) + 1 ;
	}
	else {
		mvprintw(0,0,"ERROR: terminal window is too small! Press a key to exit.");
		std::cerr << "The terminal window is too small! You need at least a 101x31 size." << std::endl;
	}
}

void Sketcher :: GameTable ()
{
	// (Offset-1) beacuse we have put a +1 in the offset, see previous comment
	for (unsigned int i=1; i<(M_yDim); ++i) {
		mvprintw(M_yOffset-1+i, M_xOffset-1, "|");
		mvprintw(M_yOffset-1+i, M_xOffset-1+M_xDim, "|");
	}

	for (unsigned int i=1; i<(M_xDim); ++i) {
		mvprintw(M_yOffset-1, M_xOffset-1+i, "-");
		mvprintw(M_yOffset-1+M_yDim, M_xOffset-1+i, "-");
	}

	mvprintw(M_yOffset-1, M_xOffset-1, "/");
	mvprintw(M_yOffset-1+M_yDim, M_xOffset-1, "\\");
	mvprintw(M_yOffset-1, M_xOffset-1+M_xDim, "\\");
	mvprintw(M_yOffset-1+M_yDim, M_xOffset-1+M_xDim, "/");
}

void Sketcher :: Pencil(RectObstacle* bush) 
{
	// (width-1) and (height-1) because we have a side from x to x+5 we
	//  have a width of 6
	
	if ( abs((bush->get_ref()).y) < (M_yDim-1) ) {
		unsigned int i_stop_top = 1;
		unsigned int i_stop_bottom = ((bush->get_rec()).height-1);
		if ( ((bush->get_ref()).y+(bush->get_rec()).height) >= M_yDim ) {
			if ( ((int)M_yDim-(bush->get_ref()).y-2) < 0 ) i_stop_bottom = 0;
			else i_stop_bottom = M_yDim - (bush->get_ref()).y - 2;
		}
		if ( (bush->get_ref()).y < 0 ) {
			i_stop_top = -(bush->get_ref()).y;
		} else {
			for (unsigned int i=1; i < ((bush->get_rec()).width-1); ++i) {
				mvprintw(M_yOffset+(bush->get_ref()).y, M_xOffset+(bush->get_ref()).x+i, "-");
			}
			mvprintw(M_yOffset+(bush->get_ref()).y, M_xOffset+(bush->get_ref()).x, "*");
			mvprintw(M_yOffset+(bush->get_ref()).y, M_xOffset+(bush->get_ref()).x+((bush->get_rec()).width-1), "*");
		}

		for (unsigned int i=i_stop_bottom; i>=i_stop_top; --i) {
			mvprintw(M_yOffset+(bush->get_ref()).y+i, M_xOffset+(bush->get_ref()).x, "|");
			mvprintw(M_yOffset+(bush->get_ref()).y+i, M_xOffset+(bush->get_ref()).x+((bush->get_rec()).width-1), "|");
		}

		if ( (bush->get_ref()).y+(bush->get_rec()).height > 0 and (bush->get_ref()).y+(bush->get_rec()).height < (M_yDim-1) ) {
			for (unsigned int i=1; i < ((bush->get_rec()).width-1); ++i) {
				mvprintw(M_yOffset+(bush->get_ref()).y+((bush->get_rec()).height-1), M_xOffset+(bush->get_ref()).x+i, "-");
			}
			mvprintw(M_yOffset+(bush->get_ref()).y+((bush->get_rec()).height-1), M_xOffset+(bush->get_ref()).x, "*");
			mvprintw(M_yOffset+(bush->get_ref()).y+((bush->get_rec()).height-1), M_xOffset+(bush->get_ref()).x+((bush->get_rec()).width-1), "*");
		}
	}
}

void Sketcher :: Pencil(SpaceSheep* sheep)
{
	mvprintw(M_yOffset+(sheep->get_ref()).y-sheep->get_fatness(),
			M_xOffset+(sheep->get_ref()).x, "O");
	mvprintw(M_yOffset+(sheep->get_ref()).y-sheep->get_fatness()+1,
			M_xOffset+(sheep->get_ref()).x-1, "*@*");
	for (unsigned int i=2; i < (sheep->get_fatness()+1); ++i) {
		for (unsigned int j=0; j < (2*i)+1; ++j) {
			mvprintw(M_yOffset+(sheep->get_ref()).y-sheep->get_fatness()+i,
					M_xOffset+(sheep->get_ref()).x-i+j, "@");
		}
	}
	for (unsigned int i=sheep->get_fatness(); i > 0 ; --i) {
		for (unsigned int j=0; j < (2*i)-1; ++j) {
			mvprintw(M_yOffset+(sheep->get_ref()).y+(sheep->get_fatness()-i+1),
					M_xOffset+(sheep->get_ref()).x-i+1+j, "@");
		}
	}
}

void Sketcher :: Rubber(RectObstacle* bush)
{	
	for (unsigned int i=0; i < (bush->get_rec()).height; ++i) {
		if ( (bush->get_ref()).y+i != -1 and (bush->get_ref()).y+i != (M_yDim-1) ) {
			mvprintw(M_yOffset+(bush->get_ref()).y+i, M_xOffset+(bush->get_ref()).x, " ");
			mvprintw(M_yOffset+(bush->get_ref()).y+i, M_xOffset+(bush->get_ref()).x+((bush->get_rec()).width-1), " ");
		}
	}

	if ( (bush->get_ref()).y != -1 and (bush->get_ref()).y != (M_yDim-1) ) {
		for (unsigned int i=0; i < (bush->get_rec()).width; ++i) {
			mvprintw(M_yOffset+(bush->get_ref()).y, M_xOffset+(bush->get_ref()).x+i, " ");
		}
	}
	if ( (bush->get_ref()).y+((bush->get_rec()).height-1) != -1 and (bush->get_ref()).y+((bush->get_rec()).height-1) != (M_yDim-1) ) {
		for (unsigned int i=0; i < (bush->get_rec()).width; ++i) {
			mvprintw(M_yOffset+(bush->get_ref()).y+((bush->get_rec()).height-1), M_xOffset+(bush->get_ref()).x+i, " ");
		}
	}
}

void Sketcher :: Rubber(SpaceSheep* sheep)
{
	for (unsigned int i=0; i < (sheep->get_fatness()+1); ++i) {
		for (unsigned int j=0; j < (2*i)+1; ++j) {
			mvprintw(M_yOffset+(sheep->get_ref()).y-sheep->get_fatness()+i,
					M_xOffset+(sheep->get_ref()).x-i+j, " ");
		}
	}
	for (unsigned int i=sheep->get_fatness(); i > 0 ; --i) {
		for (unsigned int j=0; j < (2*i)-1; ++j) {
			mvprintw(M_yOffset+(sheep->get_ref()).y+(sheep->get_fatness()-i+1),
					M_xOffset+(sheep->get_ref()).x-i+1+j, " ");
		}
	}
}

void Sketcher :: Animation(RectObstacle* bush)
{
	Rubber(bush);
	bush->move();
	Pencil(bush);
}

void Sketcher :: Animation(SpaceSheep* sheep, bool dir)
{
	Rubber(sheep);
	sheep->move(dir);
	Pencil(sheep);
}
