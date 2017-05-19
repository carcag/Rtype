#include    "uThread.hpp"

/*
** Unix thread
*/

uThread::uThread(threadFuncPtr ptr, void *arg) : _thread(0) {
  if (pthread_create(&_thread, NULL, reinterpret_cast<unixThreadFuncPtr>(ptr), arg))
    throw new threadException("could not create thread");
}

uThread::~uThread() {
}

int      uThread::join() {
  int      res;

  if (pthread_join(_thread, reinterpret_cast<void**>(&res)))
    throw new threadException("could not wait for thread");
}

/*
** Unix mutex
*/

uMutex::uMutex() : _state(false) {
  pthread_mutex_init(&_mutex, NULL);
}

uMutex::~uMutex() {
  pthread_mutex_destroy(&_mutex);
}

void      uMutex::lock() {
  pthread_mutex_lock(&_mutex);
  _state = true;
}

void      uMutex::release() {
  pthread_mutex_unlock(&_mutex);
  _state = false;
}

bool      uMutex::state() {
  return _state;
}
