#ifndef MAKE_SPOOF_INTERNAL_THREAD_DECODE_THREAD_H_
#define MAKE_SPOOF_INTERNAL_THREAD_DECODE_THREAD_H_

#include "internal_thread/internal_data.h"
#include "utils/thread.h"

class DecodeThread : public Thread {
 public:
  DecodeThread(EncodedImgVec& vec, CVMatQueue& queue, int total,
               std::atomic_bool& loading, int fd);
  DecodeThread(EncodedImgVec& vec, CVMatQueue& queue, int total,
               std::atomic_bool& loading);
  void set_fd(int fd) { fd_ = fd; };

 private:
  void ThreadEntry() override;

  EncodedImgVec& vec_;
  CVMatQueue& queue_;
  int total_;
  std::atomic_bool& loading_;
  int fd_;
};

#endif