#ifndef NET_UNIX_UNIX_TCP_SOCKET_H
# define NET_UNIX_UNIX_TCP_SOCKET_H

# include "net/IpAddress.h"
# include "net/ITcpSocket.h"
# include "net/unix/UnixSocket.h"
# include "net/SocketException.h"

class UnixTcpListener;

class UnixTcpSocket : public UnixSocket, public ITcpSocket {
 public:
  UnixTcpSocket();
  ~UnixTcpSocket();

  void Connect(const IpAddress& address, uint16_t port);
  void Send(const void *data, std::size_t size);
  Socket::Status Receive(void *buf, std::size_t size, std::size_t *received);

  friend class UnixTcpListener;
};

typedef UnixTcpSocket TcpSocket;

#endif  // NET_UNIX_UNIX_TCP_SOCKET_H
