#include    "threadException.hpp"

threadException::threadException(string const &err) : _msg(err) {
}

threadException::~threadException() throw() {
}

const char  *threadException::what() const throw() {
  string  out = "thread error : ";

  out += _msg;
  return (strdup(out.c_str()));
}
