#include "net/unix/UnixUdpSocket.h"

UnixUdpSocket::UnixUdpSocket() : UnixSocket(UDP) {
}

UnixUdpSocket::~UnixUdpSocket() {
}

void UnixUdpSocket::Bind(uint16_t port, const IpAddress& address) {
  struct sockaddr_in sin = CreateAddr(address, port);

  if (bind(sock_, (struct sockaddr *)&sin, sizeof(sin)) == -1)
    throw new SocketException("could not bind socket:", strerror(errno));
}

void UnixUdpSocket::Send(const void *data,
    std::size_t size,
    const IpAddress& remote_addr,
    uint16_t remote_port) {
  struct sockaddr_in sin = CreateAddr(remote_addr, remote_port);

  if (::sendto(sock_, data, size, 0, reinterpret_cast<struct sockaddr *>(&sin),
    sizeof(sin)) == -1)
    throw new SocketException("could not sendto:", strerror(errno));
}

Socket::Status UnixUdpSocket::Receive(void *buf,
                                      std::size_t size,
                                      std::size_t *received,
                                      IpAddress *remote_addr,
                                      uint16_t *remote_port) {
  *remote_addr = IpAddress();
  *remote_port = 0;
  struct timeval t;
  t.tv_sec = 0;
  t.tv_usec = 0;
  fd_set readset;
  FD_ZERO(&readset);
  FD_SET(sock_, &readset);
  struct sockaddr_in remote;
  socklen_t remoteSize = sizeof(remote);
  int ret = 0;

  switch (select(sock_ + 1, &readset, NULL, NULL, &t)) {
    case -1:
      return Socket::kError;
    case 0:
      return Socket::kNotReady;
    default:
      if ((ret = ::recvfrom(sock_, buf, size, 0,
        reinterpret_cast<struct sockaddr *>(&remote), &remoteSize)) == -1)
        throw new SocketException("could not receive data:", strerror(errno));
      *remote_addr = IpAddress(ntohl(remote.sin_addr.s_addr));
      *remote_port = ntohs(remote.sin_port);
      *received = ret;
      return Socket::kOk;
  }
}
