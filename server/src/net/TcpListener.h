#ifndef NET_TCP_LISTENER_H
# define NET_TCP_LISTENER_H

# ifdef __linux__
#  include "net/unix/UnixTcpListener.h"
# elif defined _WIN32
#  include "net/win32/WinTcpListener.h"
# endif

#endif  // NET_TCP_LISTENER_H
