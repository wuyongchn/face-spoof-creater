#include "internal_thread/loading_thread.h"
#include <unistd.h>
#include "utils/io.h"

LoadingThread::LoadingThread(const std::string& source, EncodedImgVec& vec,
                             std::atomic_bool& loading, int batch_size,
                             int begin, int end, int fd)
    : Thread(),
      source_(source),
      vec_(vec),
      loading_(loading),
      batch_size_(batch_size),
      begin_(begin),
      end_(end),
      fd_(fd) {}

void LoadingThread::ThreadEntry() {
  char name[128];
  char dummy[1] = {};
  for (int i = begin_; i < end_;) {
    if (loading_.load()) {
      for (int j = 0; i < end_ && j < batch_size_; ++i, ++j) {
        snprintf(name, sizeof(name), "%s/%d/%06d.jpg", source_.c_str(),
                 i / 10000, i);
        std::vector<unsigned char> buffer;
        ReadFile(name, buffer);
        vec_.emplace_back(std::move(buffer));
      }
      loading_.store(false);
      write(fd_, dummy, sizeof(dummy));  // to wake up decode thread
    } else {
      read(fd_, dummy, sizeof(dummy));  // waiting for waking up me
    }
  }
}