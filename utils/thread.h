#ifndef SPOOF_CREATER_UTILS_THREAD_H_
#define SPOOF_CREATER_UTILS_THREAD_H_

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

#endif  // SPOOF_CREATER_UTILS_THREAD_H_
