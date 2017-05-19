#ifndef NET_UNIX_TCP_LISTENER_H
# define NET_UNIX_TCP_LISTENER_H

# include "net/ITcpListener.h"
# include "net/unix/UnixSocket.h"
# include "net/TcpSocket.h"
# include "net/IpAddress.h"
# include "net/SocketException.h"

class UnixTcpListener : public UnixSocket, public ITcpListener {
 public:
  UnixTcpListener();
  ~UnixTcpListener();

  void Listen(uint16_t port, const IpAddress& address = IpAddress::Any);
  Socket::Status HasPendingConnection();
  Socket::Status Accept(TcpSocket *client, IpAddress *addr = nullptr);
};

typedef UnixTcpListener TcpListener;

#endif  // NET_UNIX_TCP_LISTENER_H
