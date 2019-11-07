#ifndef MAKE_SPOOF_INTERNAL_THREAD_LOADING_THREAD_H_
#define MAKE_SPOOF_INTERNAL_THREAD_LOADING_THREAD_H_

#include "internal_thread/internal_data.h"
#include "utils/thread.h"

class LoadingThread : public Thread {
 public:
  LoadingThread(const std::string& source, EncodedImgVec& vec, int total,
                int batch_size, std::atomic_bool& loading, int fd);
  LoadingThread(const std::string& source, EncodedImgVec& vec, int total,
                int batch_size, std::atomic_bool& loading);
  void set_fd(int fd) { fd_ = fd; }

 private:
  void ThreadEntry() override;

  std::string source_;
  EncodedImgVec& vec_;
  int total_;
  int batch_size_;
  std::atomic_bool& loading_;
  int fd_;
};
#endif