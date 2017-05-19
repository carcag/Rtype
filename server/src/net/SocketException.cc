#include "net/SocketException.h"

#include <string>

SocketException::SocketException(const std::string& err)
  throw() : ctx_(""), err_(err) {}

SocketException::SocketException(const std::string& ctx, const std::string& err)
  throw() : ctx_(ctx), err_(err) {}

SocketException::~SocketException() throw() {}

const char* SocketException::what() const throw() {
  std::string out = "SocketError: ";
  if (ctx_ != "") {
    out += ctx_ + " ";
  }
  out += err_;
  return (strdup(out.c_str()));
}
