#ifndef      _WTHREADS_HPP_
# define    _WTHREADS_HPP_

# ifdef      WIN32

# include    <windows.h>
# include    <iostream>
# include    "iThread.hpp"

# pragma comment(lib, "Kernel32.lib")

/*
** Windows thread
*/

class      wThread : public iThread
{

 public:
  typedef DWORD  (*winThreadFuncPtr)(LPVOID);

 private:
  HANDLE    _thread;
  DWORD      _id;

 public:
  wThread(threadFuncPtr, void *arg = NULL);
  ~wThread();
  int      join();

};

/*
** Windows mutex
*/

class      wMutex : public iMutex
{

private:
  CRITICAL_SECTION  _critSection;
  bool      _state;

public:
  wMutex();
  ~wMutex();
  void      lock();
  void      release();
  bool      state();

};

/*
** Modulable typedef for if WIN or LINUX
*/

typedef wThread    aThread;
typedef wMutex    aMutex;

# endif

#endif      /* _WTHREADS_HPP_ */ 
