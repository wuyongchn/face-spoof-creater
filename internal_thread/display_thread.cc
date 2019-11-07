#include <unistd.h>
#include "internal_thread/display_thread.h"
#include "utils/background.h"

DisplayThread::DisplayThread(CVMatQueue& queue, Background* background,
                             DisplayAction& action, int total, int delay)
    : Thread(),
      queue_(queue),
      background_(background),
      window_name_(""),
      action_(action),
      total_(total),
      delay_(delay) {}

void DisplayThread::ThreadEntry() {
  cv::Mat img;
  char text[6];
  for (int i = 0; i < total_; ++i) {
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