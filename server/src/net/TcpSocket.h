#ifndef NET_TCP_SOCKET_H
# define NET_TCP_SOCKET_H

# ifdef __linux__
#  include "net/unix/UnixTcpSocket.h"
# elif defined _WIN32
#  include "net/win32/WinTcpSocket.h"
# endif

#endif  // NET_TCP_SOCKET_H
