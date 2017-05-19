#ifndef NET_SOCKET_EXCEPTION_H
# define NET_SOCKET_EXCEPTION_H

#include <exception>
#include <string>
#include <cstring>

class SocketException : public std::exception {
 private:
      const std::string ctx_;
      const std::string err_;

 public:
  explicit SocketException(const std::string& error) throw();
  SocketException(const std::string& ctx, const std::string& error) throw();
  ~SocketException() throw();

  const char* what() const throw();
};

#endif  // NET_SOCKET_EXCEPTION_H
