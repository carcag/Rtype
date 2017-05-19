#ifndef NET_WIN32_WIN_TCP_SOCKET_H
# define NET_WIN32_WIN_TCP_SOCKET_H

# include "net/IpAddress.h"
# include "net/ITcpSocket.h"
# include "net/win32/WinSocket.h"
# include "net/SocketException.h"

class WinTcpListener;

class WinTcpSocket : public WinSocket, public ITcpSocket {
 public:
  WinTcpSocket();
  ~WinTcpSocket();

  void Connect(const IpAddress& address, uint16_t port);
  void Send(const void *data, std::size_t size);
  Socket::Status Receive(void *buf, std::size_t size, std::size_t *received);

  friend class WinTcpListener;
};

typedef WinTcpSocket TcpSocket;

#endif  // NET_WIN32_WIN_TCP_SOCKET_H
