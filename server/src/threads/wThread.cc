#ifdef      WIN32

#include    "wThread.hpp"

/*
** Windows thread
*/

wThread::wThread(threadFuncPtr ptr, void *arg) : _thread(0) {
  _thread = CreateThread(NULL, 0, reinterpret_cast<winThreadFuncPtr>(ptr), arg, 0, &_id);
  if (_thread == NULL)
    throw new threadexception("could not create thread");
}

wThread::~wThread() {
  CloseHandle(_thread);
}

int      wThread::join() {
  DWORD      res;

  if (WaitForSingleObject(_thread, -1) == 0xFFFFFFFF)
    throw new threadException("could not wait for thread");
  if (GetExitCodeThread(_thread, &res) == 0)
    throw new threadException("could not get exit code");
  return res;
}

/*
** Windows mutex
*/

wMutex::wMutex() : _state(false) {
  InitializeCriticalSection(&_critSection);
}

wMutex::~wMutex() {
  DeleteCriticalSection(&_critSection);
}

void      wMutex::lock() {
  EnterCriticalSection(&_critSection);
  _state = true;
}

void      wMutex::release() {
  LeaveCriticalSection(&_critSection);
  _state = false;
}

bool      wMutex::state() {
  return _state;
}

#endif
