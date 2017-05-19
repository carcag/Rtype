#ifndef NET_WIN_SOCKET_H
# define NET_WIN_SOCKET_H

# pragma comment(lib, "Ws2_32.lib")

# include <Winsock2.h>

# include "net/ISocket.h"
# include "net/SocketException.h"

class WinSocket : public ISocket {
 protected:
  Type type_;
  WSADATA wsaData_;
  SOCKET sock_;
  explicit WinSocket(Type type);

 public:
  WinSocket() = delete;
  virtual ~WinSocket();

 protected:
  struct sockaddr_in CreateAddr(const IpAddress& addr, uint16_t port);
};

#endif  // NET_WIN_SOCKET_H
