#ifndef FACE_SPOOF_PROCTOOLS_INTERNAL_THREAD_DISPLAY_THREAD_H_
#define FACE_SPOOF_PROCTOOLS_INTERNAL_THREAD_DISPLAY_THREAD_H_

#include "internal_thread/display_thread.h"
#include "internal_thread/internal_data.h"
#include "utils/thread.h"

class Background;
class DisplayThread : public Thread {
 public:
  DisplayThread(CVMatQueue& queue, Background* background,
                DisplayAction& action, int delay, int begin, int end);
  void set_window_name(const std::string& window_name) {
    window_name_ = window_name;
  }
  std::string window_name() const { return window_name_; }
  void set_delay(int delay) { delay_ = delay; }
  int delay() const { return delay_; }
  void set_action(Action action) { action_.store(action); }
  cv::Mat get_example_img() { return queue_.peek().clone(); }

 private:
  void ThreadEntry() override;

  CVMatQueue& queue_;
  Background* background_;
  std::string window_name_;
  DisplayAction& action_;
  int delay_;
  int begin_;
  int end_;
};
#endif