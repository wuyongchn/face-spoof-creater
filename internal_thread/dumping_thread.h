#ifndef MAKE_SPOOF_INTERNAL_THREAD_DUMPING_THREAD_H_
#define MAKE_SPOOF_INTERNAL_THREAD_DUMPING_THREAD_H_

#include "internal_thread/internal_data.h"
#include "utils/thread.h"

class DumpingThread : public Thread {
 public:
  DumpingThread(EncodedImgVecPair& vec_pair, const std::string& source,
                std::atomic_bool& dumping, std::atomic_bool& loading,
                int total);
  void set_fd(int fd) { fd_ = fd; }

 private:
  void ThreadEntry() override;

  EncodedImgVecPair& vec_pair_;
  std::string source_;
  std::atomic_bool& dumping_;
  std::atomic_bool& loading_;
  int total_;
  int fd_;
};

#endif