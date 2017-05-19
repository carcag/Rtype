#include "net/win32/WinUdpSocket.h"

#include <iostream>

WinUdpSocket::WinUdpSocket() : WinSocket(UDP) {
}

WinUdpSocket::~WinUdpSocket() {
}

void WinUdpSocket::Bind(uint16_t port, const IpAddress& address) {
  struct sockaddr_in sin = CreateAddr(address, port);

  if (bind(sock_, (struct sockaddr *)&sin, sizeof(sin)) == -1)
    throw new SocketException("could not bind socket:", strerror(errno));
}

void WinUdpSocket::Send(const void *data,
                        std::size_t size,
                        const IpAddress& remote_addr,
                        uint16_t remote_port) {
  struct sockaddr_in sin = CreateAddr(remote_addr, remote_port);

  DWORD byteSent = 0;
  WSABUF wbuff;
  wbuff.buf = (char *)data;
  wbuff.len = strlen(wbuff.buf);

  if (WSASendTo(sock_,
      &wbuff, 1, &byteSent, 0,
      reinterpret_cast<struct sockaddr *>(&sin),
      sizeof(sin), NULL, NULL) == SOCKET_ERROR)
    throw new SocketException("could not send data:", strerror(errno));
}

Socket::Status WinUdpSocket::Receive(void *buf,
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
  int remoteSize = sizeof(remote);
  DWORD bytesReceived = 0;
  DWORD flags = 0;
  WSABUF wbuff;
  wbuff.buf = reinterpret_cast<char *>(buf);
  wbuff.len = size;
  LPDWORD test = reinterpret_cast<LPDWORD>(received);

  switch (select(sock_ + 1, &readset, NULL, NULL, &t)) {
    case -1:
      return Socket::kError;
    case 0:
      return Socket::kNotReady;
    default:
      if (WSARecvFrom(sock_,
          &wbuff, 1, test, &flags,
          reinterpret_cast<struct sockaddr *>(&remote),
          &remoteSize, NULL, NULL) == SOCKET_ERROR)
        throw new SocketException("could not receive data:", strerror(errno));
      *remote_addr = IpAddress(ntohl(remote.sin_addr.s_addr));
      *remote_port = ntohs(remote.sin_port);
	  *received = *test;
      return Socket::kOk;
  }
}
