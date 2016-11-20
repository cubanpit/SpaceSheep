/*
 * SpaceSheep, ncurses game. Code is hosted on GitHub.
 *
 * File: UDPSSMcast.cpp
 *
 * Implementation of classes and functions declared in UDPSSMcast.h
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

#include "UDPSSMcast.h"

UDPSSMcastSender :: UDPSSMcastSender(const std::string source_if,
									unsigned char ttl,
									const std::string &dest_address,
									unsigned short dest_port):
				UDPMcastSender(source_if, ttl, dest_address, dest_port)
{
}

UDPSSMcastSender :: ~UDPSSMcastSender()
{
}

bool UDPSSMcastSender :: send_msg(std::string msg)
{
	//set 0 as flag and use send as write
	if ( send(m_sfd, msg.c_str(), msg.size(), 0) == 0) return true;
	else return false;
}

UDPSSMcastReceiver :: UDPSSMcastReceiver(const std::string &listen_interface,
										const std::string &listen_address,
										unsigned short listen_port,
										unsigned short stimeout):
				UDPMcastReceiver(listen_interface, listen_address, listen_port)
{
	m_stimeout = stimeout;
	m_psfd = new pollfd{ m_sfd, POLLIN };
}
UDPSSMcastReceiver :: ~UDPSSMcastReceiver()
{
}

bool UDPSSMcastReceiver :: recv_msg()
{
	int p_result = ::poll(m_psfd, _UDPSSMcast_h_SOCK_N, m_stimeout);
	if( p_result == -1) {
		std::string err(::strerror(errno));
		throw  "UDPMcastReceiver::recv_data() ERROR: " + err;
		return false;
	}
	else if( p_result == 0 ) {
		//poll() reached timeout
		return false;
	}
	else {
		if( m_psfd->revents & POLLIN ) {
			if( ::recv(m_sfd, m_msg, _UDPSSMcast_h_DEFAULT_MSG_LEN, 0) == 0) return true;
			else return false;
		}
		else {
			if( m_psfd->revents & POLLNVAL ) {
				throw "UDPMcastReceiver::recv_data() ERROR: poll() found fd not open.";
			}
			if( m_psfd->revents & POLLHUP ) {
				throw "UDPMcastReceiver::recv_data() ERROR: poll() returned hang up.";
			}
			if( m_psfd->revents & POLLERR ) {
				throw "UDPMcastReceiver::recv_data() ERROR: poll() returned an error condition.";
			}
			return false;
		}
	}
}

std::string compose_msg(SpaceSheep* sheep)
{
	/*
	 * We can convert int to char without troubles only because we are sure
	 * that x position, width and height are less than 255 and non negative.
	 */
	std::string msg{'s', (char) (sheep->get_ref()).x,
					(char) sheep->get_fatness(), '0'};
	return msg;
}

std::string compose_msg(RectObstacle* rect)
{
	/*
	 * We can convert int to char without troubles only because we are sure
	 * that x position, width and height are less than 255 and non negative.
	 */
	std::string msg{'r', (char) (rect->get_ref()).x,
					(char) (rect->get_rec()).height,
					(char) (rect->get_rec()).width};
	return msg;
}
