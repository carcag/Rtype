#ifndef NET_IUDP_SOCKET_H
# define NET_IUDP_SOCKET_H

# include "net/ISocket.h"
# include "net/IpAddress.h"

class IUdpSocket {
 public:
  virtual ~IUdpSocket() {}

  virtual void Bind(uint16_t port,
                    const IpAddress& address = IpAddress::Any) = 0;
  virtual void Send(const void *data,
                    std::size_t size,
                    const IpAddress& remote_addr,
                    uint16_t remote_port) = 0;
  virtual Socket::Status Receive(void *buf,
                                 std::size_t size,
                                 std::size_t *received,
                                 IpAddress *remote_addr,
                                 uint16_t *remote_port) = 0;
};

#endif  // NET_IUDP_SOCKET_H
