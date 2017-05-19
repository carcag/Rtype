#include "IpAddress.h"

const IpAddress IpAddress::None;
const IpAddress IpAddress::Any(0, 0, 0, 0);
const IpAddress IpAddress::Localhost(127, 0, 0, 1);
const IpAddress IpAddress::Broadcast(255, 255, 255, 255);

IpAddress::IpAddress() : addr_(0), valid_(false) {}

IpAddress::IpAddress(const char *addr) : addr_(0), valid_(false) {
  resolve(addr);
}

IpAddress::IpAddress(const std::string &addr) : addr_(0), valid_(false) {
  resolve(addr);
}

IpAddress::IpAddress(uint32_t addr) : addr_(htonl(addr)), valid_(true) {}

IpAddress::IpAddress(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
    : addr_(htonl((b0 << 24) | (b1 << 16) | (b2 << 8) | b3)), valid_(true) {}

std::string IpAddress::toString() const {
  struct in_addr addr;
  addr.s_addr = addr_;
  return inet_ntoa(addr);
}

uint32_t IpAddress::toInteger() const { return ntohl(addr_); }

void IpAddress::resolve(const std::string &addr) {
  if (addr == "255.255.255.255") {
    addr_ = INADDR_BROADCAST;
    valid_ = true;
  } else {
    addr_ = inet_addr(addr.c_str());
    valid_ = (addr_ == INADDR_NONE) ? false : true;
  }
}

bool operator==(const IpAddress &l, const IpAddress &r) {
  return std::make_pair(l.valid_, l.addr_) == std::make_pair(r.valid_, r.addr_);
}

std::ostream &operator<<(std::ostream &os, const IpAddress &addr) {
  if (addr.valid_) {
    return os << addr.toString();
  } else {
    return os << "<invalid ip address>";
  }
}
