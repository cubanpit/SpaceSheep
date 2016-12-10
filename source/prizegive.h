/*
 * SpaceSheep, ncurses game. Code is hosted on GitHub.
 *
 * File: prizegive.h
 *
 * Here stay all functions that handle score file, writing it, reading it.
 * The judge always says the truth.
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

#ifndef _PRIZEGIVE_H_
#define _PRIZEGIVE_H_

#include <vector> // std::vector
#include <string> // std::string, std::stoi
#include <fstream> // std::fstream
#include <algorithm> // std::sort

// default score file
const std::string _prizigive_h_SCORE_FILE_ = ".score.dat";

// add a score to file, with player name
bool add_score(const unsigned int new_score, const std::string& player_name);

// get a list of 'nscore' scores read from file
const std::vector<std::string> get_score(unsigned short int nscore);

// compare function to sort the score list
bool compare_score (std::string strj,std::string strk);

#endif //_PRIZEGIVE_H_

/*
 * SCORE FILE FORMAT:
 * 
 * 1234_MyName
 * 987_HarryFoster
 * 36789_SurName
 * 51_mermaid-LOL
 *
 */
