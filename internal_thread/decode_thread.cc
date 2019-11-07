#include "internal_thread/decode_thread.h"
#include <unistd.h>

DecodeThread::DecodeThread(EncodedImgVec& vec, CVMatQueue& queue, int total,
                           std::atomic_bool& loading, int fd)
    : Thread(),
      vec_(vec),
      queue_(queue),
      total_(total),
      loading_(loading),
      fd_(fd) {}

DecodeThread::DecodeThread(EncodedImgVec& vec, CVMatQueue& queue, int total,
                           std::atomic_bool& loading)
    : DecodeThread(vec, queue, total, loading, -1) {}

void DecodeThread::ThreadEntry() {
  char dummy[1] = {};
  for (int i = 0; i < total_;) {
    if (loading_.load()) {
      ::read(fd_, dummy, sizeof(dummy));  // waiting for waking up me
    } else {
      for (auto& data_vec : vec_) {
        cv::Mat img = cv::imdecode(data_vec, CV_LOAD_IMAGE_COLOR);
        queue_.push(img);
        ++i;
      }
      vec_.clear();
      if (i != total_) {
        loading_.store(true);
        ::write(fd_, dummy, sizeof(dummy));  // to wake up read thread
      }
    }
  }
}