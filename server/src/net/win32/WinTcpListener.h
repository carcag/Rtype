#ifndef NET_WIN_TCP_LISTENER_H
# define NET_WIN_TCP_LISTENER_H

# include "net/ITcpListener.h"
# include "net/win32/WinSocket.h"
# include "net/TcpSocket.h"
# include "net/IpAddress.h"
# include "net/SocketException.h"

class WinTcpListener : public WinSocket, public ITcpListener {
 public:
  WinTcpListener();
  ~WinTcpListener();

  void Listen(uint16_t port, const IpAddress& address = IpAddress::Any);
  Socket::Status HasPendingConnection();
  Socket::Status Accept(TcpSocket *client, IpAddress *addr = nullptr);

};

typedef WinTcpListener TcpListener;

#endif  // NET_WIN_TCP_LISTENER_H
