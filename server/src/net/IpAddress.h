#ifndef NET_IP_ADDRESS_H
# define NET_IP_ADDRESS_H

# ifdef __linux__
#  include <arpa/inet.h>
# elif defined _WIN32
#  pragma comment(lib, "Ws2_32.lib")
#  include <Winsock2.h>
# endif

# include <iostream>
# include <cstdint>
# include <string>

class IpAddress {
 private:
  uint32_t addr_;
  bool valid_;

 public:
  IpAddress();
  IpAddress(const char *addr);  // NOLINT(runtime/explicit)
  IpAddress(const std::string& addr);  // NOLINT(runtime/explicit)
  IpAddress(uint32_t addr);  // NOLINT(runtime/explicit)
  IpAddress(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3);
  ~IpAddress() {}

  std::string toString() const;
  uint32_t toInteger() const;

  static const IpAddress None;
  static const IpAddress Any;
  static const IpAddress Localhost;
  static const IpAddress Broadcast;

 private:
  void resolve(const std::string& addr);

  friend bool operator ==(const IpAddress& l, const IpAddress& r);
  friend std::ostream& operator<<(std::ostream& os, const IpAddress& addr);
};

bool operator ==(const IpAddress& l, const IpAddress& r);
std::ostream& operator<<(std::ostream& os, const IpAddress& addr);

#endif  // NET_IP_ADDRESS_H
