#include "internal_thread/loading_thread.h"
#include <unistd.h>
#include "utils/io.h"

LoadingThread::LoadingThread(const std::string& source, EncodedImgVec& vec,
                             int total, int batch_size,
                             std::atomic_bool& loading, int fd)
    : Thread(),
      source_(source),
      vec_(vec),
      total_(total),
      batch_size_(batch_size),
      loading_(loading),
      fd_(fd) {}

LoadingThread::LoadingThread(const std::string& source, EncodedImgVec& vec,
                             int total, int batch_size,
                             std::atomic_bool& loading)
    : LoadingThread(source, vec, total, batch_size, loading, -1) {}

void LoadingThread::ThreadEntry() {
  char name[128];
  char dummy[1] = {};
  for (int i = 0; i < total_;) {
    if (loading_.load()) {
      for (int j = 0; i < total_ && j < batch_size_; ++i, ++j) {
        snprintf(name, sizeof(name), "%s/%d/%05d.jpg", source_.c_str(),
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