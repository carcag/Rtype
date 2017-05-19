#include "net/unix/UnixSocket.h"

UnixSocket::UnixSocket(Type type) : type_(type) {
  if ((sock_ =
      socket(AF_INET, (type == UDP) ? SOCK_DGRAM : SOCK_STREAM, 0)) == -1)
    throw new SocketException("could not open socket:", strerror(errno));

  int reuse = 1;
  if (setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR,
    (const char*)&reuse, sizeof(reuse)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");

#ifdef SO_REUSEPORT
  if (setsockopt(sock_, SOL_SOCKET, SO_REUSEPORT,
    (const char*)&reuse, sizeof(reuse)) < 0)
    perror("setsockopt(SO_REUSEPORT) failed");
#endif
}

UnixSocket::~UnixSocket() {
  if (close(sock_) != 0)
    throw new SocketException("could not close socket:", strerror(errno));
}

struct sockaddr_in UnixSocket::CreateAddr(const IpAddress& addr,
                                          uint16_t port) {
  struct sockaddr_in sin = {0};
  sin.sin_addr.s_addr = htonl(addr.toInteger());
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  return sin;
}
