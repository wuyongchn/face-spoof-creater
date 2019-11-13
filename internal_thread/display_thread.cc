#include "internal_thread/display_thread.h"
#include <unistd.h>
#include "utils/background.h"

DisplayThread::DisplayThread(CVMatQueue& queue, Background* background,
                             DisplayAction& action, int delay, int begin,
                             int end)
    : Thread(),
      queue_(queue),
      background_(background),
      window_name_(""),
      action_(action),
      delay_(delay),
      begin_(begin),
      end_(end) {}

void DisplayThread::ThreadEntry() {
  cv::Mat img;
  char text[6];
  for (int i = begin_; i < end_; ++i) {
    if (background_) {
      snprintf(text, sizeof(text), "%05d", i);
      background_->PutImageAndText(queue_.pop(), text);
      img = background_->background();
    } else {
      img = queue_.pop();
    }
    cv::imshow(window_name_, img);
    cv::waitKey(delay_);
    if (action_.load() != DISPLAY_ONLY) {  // cooperate with capture thread
      action_.store(STOP);
      while (action_.load() != NEXT) {  // wait for ending
        usleep(10 * 1000);
      }
    }
  }
}