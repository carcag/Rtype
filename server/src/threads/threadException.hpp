#ifndef    _THREADEXCEPTION_HPP_
# define  _THREADEXCEPTION_HPP_

# include  <exception>
# include  <string>
# include  <string.h>

using    std::string;

/*
** Exception function
*/

class    threadException : public std::exception
{

private:
  const string  _msg;

public:
  threadException(string const &err);
  ~threadException() throw();
  const char  *what() const throw();

};

#endif    /* _THREADEXCEPTION_HPP_ */ 
