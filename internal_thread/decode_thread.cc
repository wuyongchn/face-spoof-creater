#include "internal_thread/decode_thread.h"
#include <unistd.h>

DecodeThread::DecodeThread(EncodedImgVec& vec, CVMatQueue& queue,
                           std::atomic_bool& loading, int begin, int end,
                           int fd)
    : Thread(),
      vec_(vec),
      queue_(queue),
      loading_(loading),
      begin_(begin),
      end_(end),
      fd_(fd) {}

void DecodeThread::ThreadEntry() {
  char dummy[1] = {};
  for (int i = begin_; i < end_;) {
    if (loading_.load()) {
      ::read(fd_, dummy, sizeof(dummy));  // waiting for waking up me
    } else {
      for (auto& data_vec : vec_) {
        cv::Mat img = cv::imdecode(data_vec, CV_LOAD_IMAGE_COLOR);
        queue_.push(img);
        ++i;
      }
      vec_.clear();
      if (i != end_) {
        loading_.store(true);
        ::write(fd_, dummy, sizeof(dummy));  // to wake up read thread
      }
    }
  }
}