#include "internal_thread/capture_thread.h"

CaptureThread::CaptureThread(CVMatQueue& queue, DisplayAction& action,
                             int total, int delay)
    : Thread(),
      queue_(queue),
      action_(action),
      total_(total),
      delay_(delay),
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
  for (int i = 0; i < total_;) {
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