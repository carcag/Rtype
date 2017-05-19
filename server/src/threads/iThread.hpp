#ifndef      _ITHREAD_HPP_
# define    _ITHREAD_HPP_

# include    "threadException.hpp"

/*
** Thread interface
*/

class      iThread
{

public:
  typedef int    (*threadFuncPtr)(void*);

  virtual    ~iThread() {};
  virtual int    join() = 0;

};

/*
** Mutex interface
*/

class      iMutex
{

public:
  virtual    ~iMutex() {};
  virtual void    lock() = 0;
  virtual void    release() = 0;
  virtual bool    state() = 0;

};

#endif      /* _ITHREAD_HPP_ */ 
