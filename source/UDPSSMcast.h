/*
 * SpaceSheep, ncurses game. Code is hosted on GitHub.
 *
 * File: UDPSSMcast.h
 *
 * This header declares classes needed to handle network communication through
 *  UDP packets.
 *
 * Authors:
 *  Martina Crippa             <martina.crippa2@studenti.unimi.it>
 *  Pietro Francesco Fontana   <pietrofrancesco.fontana@studenti.unimi.it>
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

#include <sys/poll.h> // poll(), pollfd

#include "obstacle.h"
#include "UDPMcastSender.h"
#include "UDPMcastReceiver.h"

//message lenght in byte
const unsigned short int _UDPSSMcast_h_DEFAULT_MSG_LEN = 5;
//number of sockets are listened
const unsigned short int _UDPSSMcast_h_SOCK_N = 1;
//socket timeout in ms
const unsigned short int _UDPSSMcast_h_SOCK_TMOUT = 5;

class UDPSSMcastSender : public UDPMcastSender
{
  public:
    UDPSSMcastSender(
        const std::string& source_if = "",
        unsigned char ttl = _UDPMcastSender_h_DEFAULT_TTL,
        const std::string& dest_address =
        std::string(_UDPMcastSender_h_DEFAULT_MCAST_ADDR),
        unsigned short dest_port = _UDPMcastSender_h_DEFAULT_PORT);

    ~UDPSSMcastSender();

    bool send_msg(std::string msg) const;
};

class UDPSSMcastReceiver : public UDPMcastReceiver
{
  public:
    UDPSSMcastReceiver(
        const std::string& listen_interface = "",
        const std::string& listen_address =
        std::string(_UDPMcastReceiver_h_DEFAULT_MCAST_ADDR),
        unsigned short listen_port = _UDPMcastReceiver_h_DEFAULT_PORT,
        unsigned short stimeout = _UDPSSMcast_h_SOCK_TMOUT);

    ~UDPSSMcastReceiver();

    bool recv_msg();
    const char* get_msg() const { return m_msg; };
    void flush_socket();

  private:
    unsigned short m_stimeout;
    pollfd m_psfd[_UDPSSMcast_h_SOCK_N];
    char m_msg[_UDPSSMcast_h_DEFAULT_MSG_LEN];
};

//compose message for SpaceSheep obstacle types
std::string compose_msg(CircleObstacle* circle);
std::string compose_msg(RectObstacle* rect);

#endif // _UDPSSMCAST_H_

// vim: set expandtab ts=2 sw=2:
