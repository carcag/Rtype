#include "net/unix/UnixTcpListener.h"

UnixTcpListener::UnixTcpListener() : UnixSocket(TCP) {
}

UnixTcpListener::~UnixTcpListener() {
}

void UnixTcpListener::Listen(uint16_t port, const IpAddress& address) {
  struct sockaddr_in sin = CreateAddr(address, port);

  if (bind(sock_,
    reinterpret_cast<struct sockaddr *>(&sin), sizeof(sin)) == -1)
    throw new SocketException("could not bind tcp socket", strerror(errno));

  if (listen(sock_, 32) == -1)
    throw new SocketException("could not listen:", strerror(errno));
}

Socket::Status UnixTcpListener::HasPendingConnection() {
  struct timeval t;
  t.tv_sec = 0;
  t.tv_usec = 0;
  fd_set readset;
  FD_ZERO(&readset);
  FD_SET(sock_, &readset);

  switch (select(sock_ + 1, &readset, NULL, NULL, &t)) {
    case -1:
      return Socket::kError;
    case 0:
      return Socket::kNotReady;
    default:
      return Socket::kOk;
  }
}

Socket::Status UnixTcpListener::Accept(TcpSocket *client, IpAddress *addr) {
  struct timeval t;
  t.tv_sec = 0;
  t.tv_usec = 0;
  fd_set readset;
  FD_ZERO(&readset);
  FD_SET(sock_, &readset);
  struct sockaddr_in sin;
  socklen_t size = sizeof(sin);

  switch (select(sock_ + 1, &readset, NULL, NULL, &t)) {
    case -1:
      return Socket::kError;
    case 0:
      return Socket::kNotReady;
    default:
      if ((client->sock_ =
        accept(sock_, reinterpret_cast<struct sockaddr *>(&sin), &size)) == -1)
        throw new SocketException("could not accpect client:", strerror(errno));
      if (addr != nullptr)
        *addr = IpAddress(ntohl(sin.sin_addr.s_addr));
      return Socket::kOk;
  }
}
