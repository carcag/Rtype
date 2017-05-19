#include <Server.h>
#include <net/SocketException.h>

#ifdef __linux__
# include <signal.h>

void sig_handler(int signum);
#endif

#include <iostream>

int main() {
#ifdef __linux__
  signal(SIGINT, sig_handler);
#endif
  Server::get().Loop();
  return 0;
}
