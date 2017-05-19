#include "net/unix/UnixTcpSocket.h"

UnixTcpSocket::UnixTcpSocket() : UnixSocket(TCP) {
}

UnixTcpSocket::~UnixTcpSocket() {
}

void UnixTcpSocket::Connect(const IpAddress& address, uint16_t port) {
  struct sockaddr_in sin = CreateAddr(address, port);
  socklen_t size = sizeof(sin);

  if (connect(sock_, reinterpret_cast<struct sockaddr *>(&sin), size) == -1)
    throw new SocketException("could not connect socket:", strerror(errno));
}

void UnixTcpSocket::Send(const void *data, std::size_t size) {
  if (send(sock_, data, size, 0) == -1)
    throw new SocketException("could not send data:", strerror(errno));
}

Socket::Status UnixTcpSocket::Receive(void *buf,
                                      std::size_t size,
                                      std::size_t *received) {
  struct timeval t;
  t.tv_sec = 0;
  t.tv_usec = 0;
  fd_set readset;
  FD_ZERO(&readset);
  FD_SET(sock_, &readset);
  int ret = 0;

  switch (select(sock_ + 1, &readset, NULL, NULL, &t)) {
    case -1:
      return Socket::kError;
    case 0:
      return Socket::kNotReady;
    default:
      if ((ret = recv(sock_, buf, size, 0)) == -1) {
        if (errno == ECONNRESET)
          return Socket::kDisconnected;
        else
          throw new SocketException("could not receive data:", strerror(errno));
      }
      if (ret == 0)
        return Socket::kDisconnected;
      *received = ret;
      return Socket::kOk;
  }
}
