#ifndef NET_WIN_UDP_SOCKET_H
# define NET_WIN_UDP_SOCKET_H

# include "net/IpAddress.h"
# include "net/IUdpSocket.h"
# include "net/win32/WinSocket.h"
# include "net/SocketException.h"

class WinUdpSocket : public WinSocket, public IUdpSocket {
 public:
  WinUdpSocket();
  ~WinUdpSocket();

  void Bind(uint16_t port, const IpAddress& address = IpAddress::Any);
  void Send(const void *data,
            std::size_t size,
            const IpAddress& remote_addr,
            uint16_t remote_port);
  Socket::Status Receive(void *buf,
                         std::size_t size,
                         std::size_t *received,
                         IpAddress *remote_addr,
                         uint16_t *remote_port);
};

typedef WinUdpSocket UdpSocket;

#endif  // NET_WIN_UDP_SOCKET_H
