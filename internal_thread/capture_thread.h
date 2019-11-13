#ifndef FACE_SPOOF_PROCTOOLS_INTERNAL_THREAD_CAPTURE_THREAD_H_
#define FACE_SPOOF_PROCTOOLS_INTERNAL_THREAD_CAPTURE_THREAD_H_

#include "internal_thread/internal_data.h"
#include "utils/thread.h"

class CaptureThread : public Thread {
 public:
  CaptureThread(CVMatQueue& queue, DisplayAction& action, int delay, int begin,
                int end);
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
  int delay_;
  int begin_;
  int end_;
  cv::VideoCapture cap_;
};
#endif