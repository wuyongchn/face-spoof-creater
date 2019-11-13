#include "internal_thread/capture_thread.h"

CaptureThread::CaptureThread(CVMatQueue& queue, DisplayAction& action,
                             int delay, int begin, int end)
    : Thread(),
      queue_(queue),
      action_(action),
      delay_(delay),
      begin_(begin),
      end_(end),
      cap_() {}

CaptureThread::~CaptureThread() { CloseCamera(); }

void CaptureThread::CloseCamera() {
  if (cap_.isOpened()) {
    cap_.release();
  }
}

void CaptureThread::ThreadEntry() {
  cv::Mat frame;
  for (int i = 0; i < 10; ++i) {
    cap_ >> frame;
  }
  for (int i = begin_; i < end_;) {
    for (int k = 0; k < 3; ++k) {
      cap_ >> frame;
    }
    if (action_.load() == STOP) {
      ++i;
      queue_.push(frame.clone());
      action_.store(NEXT);
    }
  }
}