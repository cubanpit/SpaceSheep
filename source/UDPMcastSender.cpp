//
// File: UDPMcastSender.cpp
//
// Author: Francesco Prelz (Francesco.Prelz@mi.infn.it)
//
// Revision history:
// 23-Apr-2010 Initial version - including IPv6 support.
// 20-Jun-2011 Use unicast as a fallback option.
//
// Description:
// Base class for sending data via IPV4 or IPV6 multicast.

#include <iostream>

#include <ctime> // clock_gettime()
#include <stdint.h> // cstdint only in c++0x

#include "UDPMcastSender.h"

UDPMcastSender::UDPMcastSender(unsigned char ttl) : m_sfd(-1), m_ttl(ttl)
{
  m_good = false;
  ::clock_gettime(CLOCK_REALTIME,&m_creation_time);
}

UDPMcastSender::UDPMcastSender(
    const std::string &source_interface,
    unsigned char ttl,
    const std::string &dest_address,
    unsigned short dest_port) :
  m_ttl(ttl), m_dest_address(dest_address), m_dest_port(dest_port)
{
  m_good = false;
  m_is_multicast = false;
  ::clock_gettime(CLOCK_REALTIME,&m_creation_time);

  if (source_interface.length() > 0)
   {
    // if_nametoindex returns 0 in case of failure, and this
    // means "bind on any interface", which is what we need.
    m_src_ifindex = ::if_nametoindex(source_interface.c_str());
   }
  else m_src_ifindex = 0;
  m_setup_and_bind_socket();
}

UDPMcastSender::~UDPMcastSender()
{
  if (m_sfd >= 0) ::close(m_sfd);
}

bool
UDPMcastSender::set_address_port(
    const std::string &source_interface,
    const std::string &dest_address,
    unsigned short dest_port)
{
  if (m_sfd >= 0) ::close(m_sfd);
  m_good = false;

  m_dest_address = dest_address;
  m_dest_port = dest_port;

  if (source_interface.length() > 0)
   {
    // if_nametoindex returns 0 in case of failure, and this
    // means "bind on any interface", which is what we need.
    m_src_ifindex = ::if_nametoindex(source_interface.c_str());
   }
  else m_src_ifindex = 0;
  return m_setup_and_bind_socket();
}

bool
UDPMcastSender::m_setup_and_bind_socket()
{
  // Figure out about the destination address via getaddrinfo
  struct addrinfo ai_req, *ai_ans, *cur_ans;

  ai_req.ai_flags = 0;
  ai_req.ai_family = PF_UNSPEC;
  ai_req.ai_socktype = SOCK_DGRAM;
  ai_req.ai_protocol = 0; /* Any protocol is OK */

  // Trim whitespace from m_dest_addr
  std::string::size_type last = m_dest_address.find_last_not_of("\r\n\t ");
  if (last != std::string::npos) m_dest_address.resize(last+1);
  // Convert m_dest_port to string
  std::ostringstream dest_portstr;
  dest_portstr << m_dest_port;

  if (::getaddrinfo(m_dest_address.c_str(), dest_portstr.str().c_str(),
      &ai_req, &ai_ans) < 0)
   {
    m_sfd = -1;
    std::ostringstream message;
    message << "Error converting address string '" <<  m_dest_address
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

    m_dest_addr_family = cur_ans->ai_family;
    m_is_multicast = false;
    switch(m_dest_addr_family)
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
      unsigned char multicast_loop;
      unsigned int ip6_multicast_loop;

      switch(m_dest_addr_family)
       {
        case AF_INET:
          // IPV4 case
          struct ip_mreqn mreq;
          std::memcpy(&mreq.imr_multiaddr,
                      &(((struct sockaddr_in*)(cur_ans->ai_addr))->sin_addr),
                      sizeof(struct in_addr));
          mreq.imr_address.s_addr = htonl(INADDR_ANY);
          mreq.imr_ifindex = m_src_ifindex;

          retsetsock = ::setsockopt(m_sfd, IPPROTO_IP, IP_MULTICAST_IF,
                       &mreq, sizeof(mreq));
          if (retsetsock < 0)
           {
            ::close(m_sfd);
            m_sfd = -1;
            std::ostringstream message;
            message << "Error: setsockopt (IP_MULTICAST_IF, if index == "
                    << m_src_ifindex << ") returns " << retsetsock
                      << ": " << ::strerror(errno);
            m_error_string = message.str();
            continue;
           }

          // Send multicast traffic to local node.
          multicast_loop = 1;
          retsetsock = ::setsockopt(m_sfd, IPPROTO_IP, IP_MULTICAST_LOOP,
                                    &multicast_loop, sizeof(multicast_loop));
          if (retsetsock < 0)
           {
            ::close(m_sfd);
            m_sfd = -1;
            std::ostringstream message;
            message << "Error: setsockopt (IP_MULTICAST_LOOP) returns " << retsetsock
                      << ": " << ::strerror(errno);
            m_error_string = message.str();
            continue;
           }

          break;

        case AF_INET6:
          // IPV6 case
          retsetsock = ::setsockopt(m_sfd, IPPROTO_IPV6, IPV6_MULTICAST_IF,
                       &m_src_ifindex, sizeof(m_src_ifindex));
          if (retsetsock < 0)
           {
            ::close(m_sfd);
            m_sfd = -1;
            std::ostringstream message;
            message << "Error: setsockopt (IPV6_MULTICAST_IF, if index == "
                    << m_src_ifindex << ") returns " << retsetsock
                      << ": " << ::strerror(errno);
            m_error_string = message.str();
            continue;
           }

          // Send multicast traffic to local node.
          ip6_multicast_loop = true;
          retsetsock = ::setsockopt(m_sfd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
                                    &ip6_multicast_loop, sizeof(ip6_multicast_loop));
          if (retsetsock < 0)
           {
            ::close(m_sfd);
            m_sfd = -1;
            std::ostringstream message;
            message << "Error: setsockopt (IPV6_MULTICAST_LOOP) returns " << retsetsock
                      << ": " << ::strerror(errno);
            m_error_string = message.str();
            continue;
           }

          break;

        default:
          continue;
       }

      if (!set_ttl(m_ttl)) continue;
     }

    if (::connect(m_sfd, cur_ans->ai_addr, cur_ans->ai_addrlen) < 0)
     {
      ::close(m_sfd);
      m_sfd = -1;
      std::ostringstream message;
      message << "Error connecting datagram socket: " << ::strerror(errno);
      m_error_string = message.str();
      continue;
     }
   }

  ::freeaddrinfo(ai_ans);
  if (m_sfd < 0) return false;

  m_good = true;
  return true;
}

bool
UDPMcastSender::good() const
{
  return m_good;
}

unsigned char
UDPMcastSender::get_ttl() const
{
 return m_ttl;
}

bool
UDPMcastSender::set_ttl(unsigned char ttl)
{
  int retsetsock;
  std::string opt_name;

  int hops;

  if (!m_is_multicast) return true;

  switch(m_dest_addr_family)
   {
    case AF_INET:
      opt_name = "IP_MULTICAST_TTL";
      retsetsock = ::setsockopt(m_sfd, IPPROTO_IP, IP_MULTICAST_TTL,
                                &ttl, sizeof(ttl));
      break;

    case AF_INET6:
      hops = ttl;
      opt_name = "IPV6_MULTICAST_HOPS";
      retsetsock = ::setsockopt(m_sfd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS,
                                &hops, sizeof(hops));
      break;

    default:
      return false;
   }

  if (retsetsock < 0)
   {
    ::close(m_sfd);
    m_sfd = -1;
    std::ostringstream message;
    message << "Error: setsockopt (" << opt_name << ") returns " << retsetsock
              << ": " << ::strerror(errno);
    m_error_string = message.str();
    return false;
   }
  return true;
}

const std::string &
UDPMcastSender::get_dest_address() const
{
  return m_dest_address;
}

unsigned short
UDPMcastSender::get_dest_port() const
{
  return m_dest_port;
}

const std::string &
UDPMcastSender::get_error() const
{
  return m_error_string;
}

std::string
UDPMcastSender::get_source_interface() const
{
  char iname[IF_NAMESIZE];

  iname[0] = '\000';
  if_indextoname(m_src_ifindex, iname);

  return std::string(iname);
}

// vim: set expandtab ts=2 sw=2:
