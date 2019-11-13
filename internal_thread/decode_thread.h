#ifndef FACE_SPOOF_PROCTOOLS_INTERNAL_THREAD_DECODE_THREAD_H_
#define FACE_SPOOF_PROCTOOLS_INTERNAL_THREAD_DECODE_THREAD_H_

#include "internal_thread/internal_data.h"
#include "utils/thread.h"

class DecodeThread : public Thread {
 public:
  DecodeThread(EncodedImgVec& vec, CVMatQueue& queue, std::atomic_bool& loading,
               int begin, int end, int fd = -1);
  void set_fd(int fd) { fd_ = fd; };

 private:
  void ThreadEntry() override;

  EncodedImgVec& vec_;
  CVMatQueue& queue_;
  std::atomic_bool& loading_;
  int begin_;
  int end_;
  int fd_;
};

#endif