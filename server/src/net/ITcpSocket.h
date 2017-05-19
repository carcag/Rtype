#ifndef NET_ITCP_SOCKET_H
# define NET_ITCP_SOCKET_H

# include "net/ISocket.h"
# include "net/IpAddress.h"

class ITcpSocket {
 public:
  virtual ~ITcpSocket() {}

  virtual void Connect(const IpAddress& address, uint16_t port) = 0;
  virtual void Send(const void *data, std::size_t size) = 0;
  virtual Socket::Status Receive(void *buf,
                                 std::size_t size,
                                 std::size_t *received) = 0;
};

#endif  // NET_ITCP_SOCKET_H
