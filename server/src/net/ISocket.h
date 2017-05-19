#ifndef NET_ISOCKET_H
# define NET_ISOCKET_H

# include "net/IpAddress.h"

namespace Socket {
enum Status {
  kOk = 0,
  kNotReady,
  kPartial,
  kDisconnected,
  kError
};
}

class ISocket {
 protected:
  enum Type {
    UDP,
    TCP
  };

 public:
  virtual ~ISocket() {}

 protected:
  virtual struct sockaddr_in CreateAddr(const IpAddress& addr,
                                        uint16_t port) = 0;
};

#endif  //! NET_ISOCKET_H
