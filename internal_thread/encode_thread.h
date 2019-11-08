#ifndef SPOOF_CREATER_INTERNAL_THREAD_ENCODE_THREAD_H_
#define SPOOF_CREATER_INTERNAL_THREAD_ENCODE_THREAD_H_

#include "internal_thread/internal_data.h"
#include "utils/thread.h"

class EncodeThread : public Thread {
 public:
  EncodeThread(EncodedImgVecPair& vec_pair, CVMatQueue& queue,
               std::atomic_bool& dumping, int total, int batch_size);
  void set_fd(int fd) { fd_ = fd; }

 private:
  void ThreadEntry() override;

  EncodedImgVecPair& vec_pair_;
  CVMatQueue& queue_;
  std::atomic_bool& dumping_;
  int total_;
  int batch_size_;
  int fd_;
};
#endif