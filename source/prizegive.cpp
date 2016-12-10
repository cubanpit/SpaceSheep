/*
 * SpaceSheep, ncurses game. Code is hosted on GitHub.
 *
 * File: prizegive.cpp
 *
 * This file contains implementation of functions in prizegive.h file.
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

#include "prizegive.h"

bool add_score(const unsigned int new_score, const std::string& player_name)
{
	// check player name doesn't contain dangerous characters
	if ( player_name.length() == 0
			or player_name.find("_") != std::string::npos
			or player_name.find("\\") != std::string::npos ) {
		return false;
	}
	std::fstream score_file;
	score_file.open(_prizigive_h_SCORE_FILE_,std::ios_base::app);
	if ( score_file.fail() ) {
		throw "ERROR: Impossible to open score file "+_prizigive_h_SCORE_FILE_;
	}
	score_file << new_score << "_" << player_name << std::endl;
	score_file.close();
	if ( score_file.fail() ) return false;
	else return true;
}

const std::vector<std::string> get_score(unsigned short int nscore)
{
	bool empty_file = false;
	std::fstream score_file;
	score_file.open(_prizigive_h_SCORE_FILE_,std::fstream::in);

	//if the file doesn't exist it's created
	if ( score_file.fail() ) {
		score_file.close();
		score_file.open(_prizigive_h_SCORE_FILE_,std::fstream::out);
		if ( score_file.fail() ) {
			throw "ERROR: Impossible to open score file "+
					_prizigive_h_SCORE_FILE_;
		}
		else empty_file = true;
	}

	std::vector<std::string> score_list;
	std::string tmp_string;
	while ( !empty_file ) {
		std::getline(score_file,tmp_string);
		if ( score_file.eof() ) break;
		if ( score_file.fail() ) {
			throw "ERROR: Impossible to read from score file "+
					_prizigive_h_SCORE_FILE_;
		}
		score_list.push_back(tmp_string);
	}
	if ( score_list.size() == 0 ) empty_file = true;
	else {
		//sort score list, we know that the file contains "###_NAME"
		std::sort(score_list.begin(),score_list.end(),compare_score);
	}
	if ( score_list.size() > nscore ) score_list.resize(nscore);

	return score_list;
}


bool compare_score (std::string strj,std::string strk)
{
	std::size_t posj = strj.find("_");
	std::size_t posk = strk.find("_");
	return (std::stoi(strj.substr(0,posj)) > stoi(strk.substr(0,posk)));
}
