//
// File: UDPMcastReceiver.cpp
//
// Author: Francesco Prelz (Francesco.Prelz@mi.infn.it)
//
// Revision history:
// 23-Apr-2010 Initial version.
// 20-Jun-2011 Allow fallback to unicast.
//
// Description:
// Base class for receiving data via IPV4 or IPV6 multicast.

#include <iostream>
#include <cstring> // memcpy(),strcmp()

#include "UDPMcastReceiver.h"

UDPMcastReceiver::UDPMcastReceiver() : m_sfd(-1)
{
  m_good = false;
  ::clock_gettime(CLOCK_REALTIME,&m_creation_time);
}

UDPMcastReceiver::UDPMcastReceiver(const std::string &listen_interface,
                                   const std::string &listen_address,
                                   unsigned short listen_port):
   m_lst_address(listen_address), m_lst_port(listen_port)
{
  m_good = false;
  m_is_multicast = false;
  ::clock_gettime(CLOCK_REALTIME,&m_creation_time);

  if (listen_interface.length() > 0)
   {
    // if_nametoindex returns 0 in case of failure, and this
    // means "listen on any interface", which is what we need.
    m_lst_ifindex = ::if_nametoindex(listen_interface.c_str());
   }
  else m_lst_ifindex = 0;
  m_setup_socket_and_listen();
}

bool
UDPMcastReceiver::set_address_port(const std::string &listen_interface,
                                   const std::string &listen_address,
                                   unsigned short listen_port)
{
  if (m_sfd >= 0) ::close(m_sfd);
  m_good = false;

  m_lst_address = listen_address;
  m_lst_port = listen_port;

  if (listen_interface.length() > 0)
   {
    // if_nametoindex returns 0 in case of failure, and this
    // means "listen on any interface", which is what we need.
    m_lst_ifindex = ::if_nametoindex(listen_interface.c_str());
   }
  else m_lst_ifindex = 0;

  return m_setup_socket_and_listen();
}

bool
UDPMcastReceiver::m_setup_socket_and_listen()
{
  // Figure out about the listen address via getaddrinfo
  struct addrinfo ai_req, *ai_ans, *cur_ans;

  ai_req.ai_flags = AI_PASSIVE;
  ai_req.ai_family = PF_UNSPEC;
  ai_req.ai_socktype = SOCK_DGRAM;
  ai_req.ai_protocol = 0; /* Any protocol is OK */

  // Trim whitespace from m_lst_addr
  std::string::size_type last = m_lst_address.find_last_not_of("\r\n\t ");
  if (last != std::string::npos) m_lst_address.resize(last+1);
  // Convert m_lst_port to string
  std::ostringstream lst_portstr;
  lst_portstr << m_lst_port;

  const char *lst_address_c;
  if (m_lst_address.size() > 0) lst_address_c = m_lst_address.c_str();
  else lst_address_c = 0; // NULL for any address

  if (::getaddrinfo(lst_address_c, lst_portstr.str().c_str(),
      &ai_req, &ai_ans) < 0)
   {
    std::ostringstream message;
    message << "Error converting address string '" <<  m_lst_address
                   << "' via getaddrinfo : " << ::strerror(errno);
    m_error_string = message.str();
    return false;
   }

  // Look for a workable address
  for (cur_ans = ai_ans; cur_ans != NULL; cur_ans = cur_ans->ai_next)
   {
    m_sfd = ::socket(cur_ans->ai_family,
                     cur_ans->ai_socktype,
                     cur_ans->ai_protocol);

    if (m_sfd < 0)
     {
      std::ostringstream message;
      message << "Error creating socket: " << ::strerror(errno);
      m_error_string = message.str();
      continue;
     }

    m_lst_addr_family = cur_ans->ai_family;
    m_is_multicast = false;
    switch(m_lst_addr_family)
     {
      case AF_INET:
        if (((((struct sockaddr_in *)
               (cur_ans->ai_addr))->sin_addr.s_addr) & 0xe0)
                                                    == 0xe0)
          m_is_multicast = true;
        break;
      case AF_INET6:
        // IPV6 case
        if (IN6_IS_ADDR_MULTICAST(((struct sockaddr_in6 *)
                        (cur_ans->ai_addr))->sin6_addr.s6_addr))
          m_is_multicast = true;
        break;
      default:
        m_error_string = "getaddrinfo returns an address family that is neither IPV4 nor IPV6";
        continue;
     }

    if (m_is_multicast)
     {

      int retsetsock;

      switch(m_lst_addr_family)
       {

        case AF_INET:
          // IPV4 case

          std::memcpy(&m_mreq4.imr_multiaddr,
                      &(((struct sockaddr_in*)(cur_ans->ai_addr))->sin_addr),
                      sizeof(struct in_addr));
          m_mreq4.imr_address.s_addr = htonl(INADDR_ANY);
          m_mreq4.imr_ifindex = m_lst_ifindex;

          retsetsock= ::setsockopt(m_sfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                                   &m_mreq4, sizeof(m_mreq4));
          if (retsetsock < 0)
           {
            ::close(m_sfd);
            m_sfd = -1;
            std::ostringstream message;
            message << "Error: setsockopt (IP_ADD_MEMBERSHIP) returns " << retsetsock
                      << ": " << ::strerror(errno);
            m_error_string = message.str();
            continue;
           }
          // Bind to any address.
          ((struct sockaddr_in*)(cur_ans->ai_addr))->sin_addr.s_addr = INADDR_ANY;

          break;

        case AF_INET6:
          // IPV6 case

          std::memcpy(&m_mreq6.ipv6mr_multiaddr,
                      &(((struct sockaddr_in6*)(cur_ans->ai_addr))->sin6_addr),
                      sizeof(struct in6_addr));
          m_mreq6.ipv6mr_interface = m_lst_ifindex;

          retsetsock= ::setsockopt(m_sfd, IPPROTO_IPV6, IPV6_JOIN_GROUP,
                                   &m_mreq6, sizeof(m_mreq6));
          if (retsetsock < 0)
           {
            ::close(m_sfd);
            m_sfd = -1;
            std::ostringstream message;
            message << "Error: setsockopt (IPV6_JOIN_GROUP) returns " << retsetsock
                      << ": " << ::strerror(errno);
            m_error_string = message.str();
            continue;
           }
          ((struct sockaddr_in6*)(cur_ans->ai_addr))->sin6_addr = in6addr_any;
          break;

        default:
          continue;
       }
     }

    if (::bind(m_sfd, cur_ans->ai_addr, cur_ans->ai_addrlen) < 0)
     {
      ::close(m_sfd);
      m_sfd = -1;
      std::ostringstream message;
      message << "Error binding datagram socket: " << ::strerror(errno);
      m_error_string = message.str();
      continue;
     }
    else break;
   }

  ::freeaddrinfo(ai_ans);
  if (m_sfd < 0) return false;

  m_good = true;
  return true;
}

UDPMcastReceiver::~UDPMcastReceiver()
{
  m_unlisten();
}

bool
UDPMcastReceiver::good() const
{
  return m_good;
}

bool
UDPMcastReceiver::got_data() const
{
  return m_got_data;
}

const std::string &
UDPMcastReceiver::get_listen_address() const
{
  return m_lst_address;
}

unsigned short
UDPMcastReceiver::get_listen_port() const
{
  return m_lst_port;
}


const std::string &
UDPMcastReceiver::get_error() const
{
  return m_error_string;
}

std::string
UDPMcastReceiver::get_listen_interface() const
{
  char iname[IF_NAMESIZE];

  iname[0] = '\000';
  if_indextoname(m_lst_ifindex, iname);

  return std::string(iname);
}

void
UDPMcastReceiver::m_unlisten()
{
  m_good = false;
  if (m_sfd >= 0)
   {
    if (m_is_multicast)
     {
      switch(m_lst_addr_family)
       {

        case AF_INET:
          ::setsockopt(m_sfd, IPPROTO_IP, IP_DROP_MEMBERSHIP,
                       &m_mreq4, sizeof(m_mreq4));
          break;

        case AF_INET6:
          ::setsockopt(m_sfd, IPPROTO_IPV6, IPV6_LEAVE_GROUP,
                       &m_mreq6, sizeof(m_mreq6));
          break;

        default:
          break;
       }
     }
    ::close(m_sfd);
    m_sfd = -1;
   }
}
