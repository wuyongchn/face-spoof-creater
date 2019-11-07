#ifndef MAKE_SPOOF_INTERNAL_THREAD_CAPTURE_THREAD_H_
#define MAKE_SPOOF_INTERNAL_THREAD_CAPTURE_THREAD_H_

#include "internal_thread/internal_data.h"
#include "utils/thread.h"

class CaptureThread : public Thread {
 public:
  CaptureThread(CVMatQueue& queue, DisplayAction& action, int total, int delay);
  ~CaptureThread();
  bool OpenCamera(int camera_id) { return cap_.open(camera_id); }
  void CloseCamera();
  bool set_frame_width(int width) {
    return cap_.set(CV_CAP_PROP_FRAME_WIDTH, width);
  }
  bool set_frame_height(int height) {
    return cap_.set(CV_CAP_PROP_FRAME_HEIGHT, height);
  }
  void set_delay(int delay) { delay_ = delay; }
  int delay() const { return delay_; }
  void set_action(Action action) { action_.store(action); }
  cv::VideoCapture& cap() { return cap_; }

 private:
  void ThreadEntry() override;

  CVMatQueue& queue_;
  DisplayAction& action_;
  int total_;
  int delay_;
  cv::VideoCapture cap_;
};
#endif