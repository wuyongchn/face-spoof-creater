#ifndef MAKE_SPOOF_UTILS_THREAD_H_
#define MAKE_SPOOF_UTILS_THREAD_H_

#include <memory>
#include <thread>
class Thread {
 public:
  Thread() : thread_() {}
  virtual ~Thread();
  void StartThread();
  void StopThread();
  bool is_started() const;

 protected:
  virtual void ThreadEntry() {}

 private:
  std::shared_ptr<std::thread> thread_;
};

#endif  // MAKE_SPOOF_UTILS_THREAD_H_
