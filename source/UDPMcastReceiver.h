//
// File: UDPMcastReceiver.h
//
// Author: Francesco Prelz (Francesco.Prelz@mi.infn.it)
//
// Revision history:
// 23-Apr-2010 Initial version - with IPV6 support.
// 20-Jun-2011 Support fallback to unicast.
// 23-Feb-2015 Added unlisten method.
//
// Description:
// Base class for receiving data via IPV4 or IPV6 multicast.

#ifndef _UDPMcastReceiver_h_included
#define _UDPMcastReceiver_h_included

#include <string>
#include <sstream>
#include <vector>

// Need socket library.

#include <ctime>

#ifdef __CINT__
#ifndef __BEGIN_DECLS
# define __BEGIN_DECLS  extern "C" {
#endif
#ifndef __END_DECLS
# define __END_DECLS  }
#endif
struct timespec
{
    time_t tv_sec;            /* Seconds.  */
    long int tv_nsec;         /* Nanoseconds.  */
};
#include "/usr/include/sys/types.h"
#else
#include <sys/types.h>
#endif

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h> // if_nametoindex

#include <cerrno>
#include <cstring> // strerror()
#include <unistd.h> // close()

// The default (IPv4) multicast address is taken out of the GLOP (RFC3180) /24
// space for Autonomous System 64516.

#define _UDPMcastReceiver_h_DEFAULT_MCAST_ADDR "233.252.4.13"
#define _UDPMcastReceiver_h_DEFAULT_PORT       58432

class UDPMcastReceiver
{
  public:

    // Constructor.
    UDPMcastReceiver();
    UDPMcastReceiver(
        const std::string &listen_interface = "",
        const std::string &listen_address = std::string(_UDPMcastReceiver_h_DEFAULT_MCAST_ADDR),
        unsigned short listen_port = _UDPMcastReceiver_h_DEFAULT_PORT);

    // Destructor.
    virtual ~UDPMcastReceiver();

    bool good() const;
    bool got_data() const;

    bool set_address_port(
        const std::string &listen_interface = "",
        const std::string &listen_address = _UDPMcastReceiver_h_DEFAULT_MCAST_ADDR,
        unsigned short listen_port = _UDPMcastReceiver_h_DEFAULT_PORT);

    const std::string &get_listen_address() const;
    unsigned short get_listen_port() const;
    const std::string &get_error() const;

    std::string get_listen_interface() const;

  protected:

    bool m_good;
    int m_sfd;
    bool m_got_data;
    timespec m_creation_time;
    std::string m_error_string;
    void m_unlisten();

  private:

    std::string m_lst_address;
    unsigned short m_lst_port;
    unsigned int m_lst_ifindex;

    int m_lst_addr_family;
    struct ip_mreqn m_mreq4;
    struct ipv6_mreq m_mreq6;
    bool m_is_multicast;

    bool m_setup_socket_and_listen();
};

#endif /* defined _UDPMcastReceiver_h_included */

// vim: set expandtab ts=2 sw=2:
