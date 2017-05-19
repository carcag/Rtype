#ifndef NET_UDP_SOCKET
# define NET_UDP_SOCKET

# ifdef __linux__
#  include "net/unix/UnixUdpSocket.h"
# elif defined _WIN32
#  include "net/win32/WinUdpSocket.h"
# endif

#endif  // NET_UDP_SOCKET
