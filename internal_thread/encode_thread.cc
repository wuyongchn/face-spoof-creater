#include "internal_thread/encode_thread.h"
#include <unistd.h>

EncodeThread::EncodeThread(EncodedImgVecPair& vec_pair, CVMatQueue& queue,
                           std::atomic_bool& dumping, int total, int batch_size)
    : Thread(),
      vec_pair_(vec_pair),
      queue_(queue),
      dumping_(dumping),
      total_(total),
      batch_size_(batch_size),
      fd_(-1) {}

void EncodeThread::ThreadEntry() {
  char dummy[1] = {};
  int count = 0;
  cv::Mat img;
  for (int i = 0; i < total_; ++i) {
    img = queue_.pop();
    std::vector<unsigned char> buf;
    cv::imencode(".jpg", img, buf);
    vec_pair_.first.emplace_back(std::move(buf));
    ++count;
    if (count >= batch_size_ && !dumping_.load()) {
      vec_pair_.first.swap(vec_pair_.second);
      count = 0;
      write(fd_, dummy, sizeof(dummy));  // wake up dumping thread
    }
  }

  if (count) {  // the last batch
    while (dumping_.load()) {
      usleep(100 * 1000);  // sleep 100 millisecodes
    }
    vec_pair_.first.swap(vec_pair_.second);
    write(fd_, dummy, sizeof(dummy));
  }
}