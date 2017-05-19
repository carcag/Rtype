#include "net/win32/WinTcpSocket.h"

WinTcpSocket::WinTcpSocket() : WinSocket(TCP) {
}

WinTcpSocket::~WinTcpSocket() {
}

void WinTcpSocket::Connect(const IpAddress& address, uint16_t port) {
  struct sockaddr_in sin = CreateAddr(address, port);
  int size = sizeof(sin);

  if (connect(sock_,
    reinterpret_cast<struct sockaddr *>(&sin), size) == INVALID_SOCKET)
    throw new SocketException("could not connect socket:", strerror(errno));
}

void WinTcpSocket::Send(const void *data, std::size_t size) {
  /*DWORD byteSent = 0;
  DWORD flags = 0;
  WSABUF wbuff;
  wbuff.buf = (char *)data;
  wbuff.len = strlen(wbuff.buf);

  if (WSASend(sock_, &wbuff, 1, &byteSent, flags, NULL, NULL) == SOCKET_ERROR)
    throw new SocketException("could not send data:", strerror(errno));
  std::cout << "bytes sent: " << byteSent << std::endl;*/
  if (send(sock_, (const char *)data, size, 0) == -1)
    throw new SocketException("could not send data:", strerror(errno));

}

Socket::Status WinTcpSocket::Receive(void *buf,
                                     std::size_t size,
                                     std::size_t *received) {
  struct timeval t;
  t.tv_sec = 0;
  t.tv_usec = 0;
  fd_set readset;
  FD_ZERO(&readset);
  FD_SET(sock_, &readset);
  DWORD bytesReceived = 0;
  DWORD flags = 0;
  WSABUF wbuff;
  wbuff.buf = (char *)buf;
  wbuff.len = size;
  LPDWORD test = reinterpret_cast<LPDWORD>(received);

  switch (select(sock_ + 1, &readset, NULL, NULL, &t)) {
    case -1:
      return Socket::kError;
    case 0:
      return Socket::kNotReady;
    default:
      if (WSARecv(sock_,
          &wbuff, 1, test, &flags,
          NULL, NULL) == SOCKET_ERROR) {
            if (WSAGetLastError() == WSAECONNABORTED
                || WSAGetLastError() == WSAECONNRESET)
              return Socket::kDisconnected;
            else
              throw new SocketException(
                "could not receive data:", strerror(errno));
          }
      if (*received == 0)
        return Socket::kDisconnected;
    *received = *test;
      return Socket::kOk;
  }
}
