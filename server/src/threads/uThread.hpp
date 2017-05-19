#ifndef      _UTHREAD_HPP_
# define    _UTHREAD_HPP_

# include    <pthread.h>
# include    <iostream>
# include    <fcntl.h>
# include    <unistd.h>
# include    "iThread.hpp"

/*
** Unix thread
*/

class      uThread : public iThread
{

private:
  pthread_t    _thread;
  bool      _state;

public:
  typedef void    *(*unixThreadFuncPtr)(void*);
  uThread(threadFuncPtr, void *arg = NULL);
  virtual    ~uThread();
  int      join();

};

/*
** Unix mutex
*/

class      uMutex : public iMutex
{

private:
  pthread_mutex_t  _mutex;
  bool      _state;

public:
  uMutex();
  virtual    ~uMutex();
  void      lock();
  void      release();
  bool      state();

};

/*
** Modulable typedef for if WIN or LINUX
*/

typedef uThread    aThread;
typedef uMutex    aMutex;

#endif      /* _UTHREAD_HPP_ */ 
