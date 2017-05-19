#ifndef NET_ITCP_LISTENER_H
# define NET_ITCP_LISTENER_H

# include "net/ISocket.h"
# include "net/TcpSocket.h"
# include "net/IpAddress.h"


class ITcpListener {
 public:
  virtual ~ITcpListener() {}

  virtual void Listen(uint16_t port,
                      const IpAddress& address = IpAddress::Any) = 0;
  virtual Socket::Status HasPendingConnection() = 0;
  virtual Socket::Status Accept(TcpSocket *client, IpAddress *addr) = 0;
};

#endif  // NET_ITCP_LISTENER_H
