/*
 * SpaceSheep, ncurses game. Code is hosted on GitHub.
 *
 * File: UDPSSMcast.h
 *
 * This header declares classes needed to handle network communication through
 *  UDP packets.
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

#ifndef _UDPSSMCAST_H_
#define _UDPSSMCAST_H_

#include <typeinfo>
#include <sys/poll.h>

#include "obstacle.h"
#include "UDPMcastSender.h"
#include "UDPMcastReceiver.h"

#include <iostream> //TMP

const short int _UDPSSMcast_h_DEFAULT_MSG_LEN = 4; //message lenght in byte
const short int _UDPSSMcast_h_SOCK_N = 1; //number of sockets we listen
const unsigned short _UDPSSMcast_h_SOCK_TMOUT = 5000; //socket timeout in ms

class UDPSSMcastSender : public UDPMcastSender
{
	public:
		UDPSSMcastSender(const std::string source_if = "",
			unsigned char ttl = _UDPMcastSender_h_DEFAULT_TTL,
			const std::string &dest_address = std::string(_UDPMcastSender_h_DEFAULT_MCAST_ADDR),
			unsigned short dest_port = _UDPMcastSender_h_DEFAULT_PORT);

		~UDPSSMcastSender();

		bool send_msg(std::string msg);

	private:
		pollfd* m_psfd;
		unsigned short m_stimeout;
};

class UDPSSMcastReceiver : public UDPMcastReceiver
{
	public:
		UDPSSMcastReceiver(const std::string &listen_interface = "",
			const std::string &listen_address = std::string(_UDPMcastReceiver_h_DEFAULT_MCAST_ADDR),
			unsigned short listen_port = _UDPMcastReceiver_h_DEFAULT_PORT,
			unsigned short stimeout = _UDPSSMcast_h_SOCK_TMOUT);

		~UDPSSMcastReceiver();

		bool recv_msg();
		char* get_msg(){ return m_msg; };

	private:
		char m_msg[_UDPSSMcast_h_DEFAULT_MSG_LEN];
		pollfd* m_psfd;
		unsigned short m_stimeout;
};

std::string compose_msg(SpaceSheep* sheep);
std::string compose_msg(RectObstacle* rect);

#endif // _UDPSSMCAST_H_
