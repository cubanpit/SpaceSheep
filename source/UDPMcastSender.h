//
// File: UDPMcastSender.h
//
// Author: Francesco Prelz (Francesco.Prelz@mi.infn.it)
//
// Revision history:
// 23-Apr-2010 Initial version.
// 20-Jun-2011 Support fallback to unicast.
//
// Description:
// Base class for sending data via IPV4 or IPV6 multicast.
//

#ifndef _UDPMcastSender_h_included
#define _UDPMcastSender_h_included

#include <string>
#include <sstream>
#include <vector>

// Need socket library.

#ifndef __BEGIN_DECLS
# define __BEGIN_DECLS  extern "C" {
#endif
#ifndef __END_DECLS
# define __END_DECLS  }
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h> // if_nametoindex

#include <ctime>
#include <cerrno>
#include <cstring> // strerror()
#include <unistd.h> // close()

// The default (IPv4) multicast address is taken out of the GLOP (RFC3180) /24
// space for Autonomous System 64516.

#define _UDPMcastSender_h_DEFAULT_MCAST_ADDR "233.252.4.13"
#define _UDPMcastSender_h_DEFAULT_PORT       58432
#define _UDPMcastSender_h_DEFAULT_TTL        2

class UDPMcastSender
{
  public:

    // Constructor.
    UDPMcastSender(unsigned char ttl = _UDPMcastSender_h_DEFAULT_TTL);
    UDPMcastSender(
        const std::string &source_interface = "",
        unsigned char ttl = _UDPMcastSender_h_DEFAULT_TTL,
        const std::string &dest_address = _UDPMcastSender_h_DEFAULT_MCAST_ADDR,
        unsigned short dest_port = _UDPMcastSender_h_DEFAULT_PORT);

    // Destructor.
    virtual ~UDPMcastSender();

    bool good() const;

    bool set_address_port(
        const std::string &source_interface = "",
        const std::string &dest_address = _UDPMcastSender_h_DEFAULT_MCAST_ADDR,
        unsigned short dest_port = _UDPMcastSender_h_DEFAULT_PORT);

    unsigned char get_ttl() const;
    bool set_ttl(unsigned char ttl);

    const std::string &get_dest_address() const;
    unsigned short get_dest_port() const;
    const std::string &get_error() const;
    std::string get_source_interface() const;

  protected:

    bool m_good;
    int m_sfd;
    timespec m_creation_time;
    std::string m_error_string;

  private:

    unsigned char m_ttl;
    std::string m_dest_address;
    unsigned short m_dest_port;
    unsigned int m_src_ifindex;
    int m_dest_addr_family;
    bool m_is_multicast;

    bool m_setup_and_bind_socket();
};

#endif /* defined _UDPMcastSender_h_included */

// vim: set expandtab ts=2 sw=2:
