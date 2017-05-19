#include "net/win32/WinSocket.h"

WinSocket::WinSocket(Type type) : type_(type) {
  if (WSAStartup(MAKEWORD(2, 2), &wsaData_) != 0)
    throw new SocketException("WSAStartup failed:", strerror(errno));
  if ((sock_ =
    WSASocket(AF_INET, (type == UDP) ? SOCK_DGRAM : SOCK_STREAM, 0,
    NULL, 0, 0)) == INVALID_SOCKET)
    throw new SocketException("WSASocket failed:", strerror(errno));
}

WinSocket::~WinSocket() {
  if (closesocket(sock_) == INVALID_SOCKET)
    throw new SocketException("could not close socket:", strerror(errno));
  WSACleanup();
}

struct sockaddr_in WinSocket::CreateAddr(const IpAddress& addr, uint16_t port) {
  struct sockaddr_in sin = {0};
  sin.sin_addr.s_addr = htonl(addr.toInteger());
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  return sin;
}
