#ifndef FACE_SPOOF_PROCTOOLS_INTERNAL_THREAD_LOADING_THREAD_H_
#define FACE_SPOOF_PROCTOOLS_INTERNAL_THREAD_LOADING_THREAD_H_

#include "internal_thread/internal_data.h"
#include "utils/thread.h"

class LoadingThread : public Thread {
 public:
  LoadingThread(const std::string& source, EncodedImgVec& vec,
                std::atomic_bool& loading, int batch_size, int begin, int end,
                int fd = -1);
  void set_fd(int fd) { fd_ = fd; }

 private:
  void ThreadEntry() override;

  std::string source_;
  EncodedImgVec& vec_;
  std::atomic_bool& loading_;
  int batch_size_;
  int begin_;
  int end_;
  int fd_;
};
#endif