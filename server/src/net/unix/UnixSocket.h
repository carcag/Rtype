#ifndef NET_UNIX_SOCKET_H
# define NET_UNIX_SOCKET_H

# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>

# include "net/ISocket.h"
# include "net/SocketException.h"

class UnixSocket : public ISocket {
 protected:
  Type type_;
  int sock_;
  explicit UnixSocket(Type type);

 public:
  UnixSocket() = delete;
  virtual ~UnixSocket();

 protected:
  struct sockaddr_in CreateAddr(const IpAddress& addr, uint16_t port);
};

#endif  // NET_UNIX_SOCKET_H
