#include "unit/display_capture_unit.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

DisplayCaptureUnit::DisplayCaptureUnit(CVMatQueue& in, CVMatQueue& out,
                                       Background* background, int begin,
                                       int end)
    : action_(DISPLAY_ONLY),
      display_thread_(in, background, action_, 33, begin, end),
      capture_thread_(out, action_, 33, begin, end),
      display_only_(false) {}

void DisplayCaptureUnit::Start(int display_delay, int capture_delay,
                               int camera_id, const cv::Size& frame_size) {
  cv::namedWindow("display", CV_WINDOW_NORMAL);
  display_thread_.set_window_name("display");
  display_thread_.set_delay(display_delay);

  capture_thread_.set_delay(capture_delay);
  if (!capture_thread_.OpenCamera(camera_id)) {
    printf("%s:%d Cannot Open Camera: %d\n", __FILE__, __LINE__, camera_id);
    exit(1);
  }
  if (frame_size.width) {
    capture_thread_.set_frame_width(frame_size.width);
  }
  if (frame_size.height) {
    capture_thread_.set_frame_height(frame_size.height);
  }
  display_thread_.set_action(NEXT);
  display_thread_.set_action(NEXT);
  PrepareCapture();
  display_thread_.StartThread();
  capture_thread_.StartThread();
}

void DisplayCaptureUnit::StartDisplay(int display_delay) {
  cv::namedWindow("display", CV_WINDOW_NORMAL);
  display_thread_.set_window_name("display");
  display_thread_.set_delay(display_delay);
  display_only_ = true;
  PrepareDisplay();
  display_thread_.StartThread();
}

void DisplayCaptureUnit::Stop() {
  display_thread_.StopThread();
  if (!display_only_) {
    capture_thread_.StopThread();
  }
  cv::destroyWindow(display_thread_.window_name());
  if (!display_only_) {
    capture_thread_.CloseCamera();
    display_thread_.set_action(DISPLAY_ONLY);
    capture_thread_.set_action(DISPLAY_ONLY);
  }
}

void DisplayCaptureUnit::PrepareDisplay() {
  cv::Mat img = display_thread_.get_example_img();
  cv::imshow(display_thread_.window_name(), img);
  cv::waitKey();
}

void DisplayCaptureUnit::PrepareCapture() {
  cv::namedWindow("capture-test");
  cv::Mat img = display_thread_.get_example_img();
  std::string window_name = display_thread_.window_name();
  cv::VideoCapture& cap = capture_thread_.cap();
  cv::Mat frame;
  do {
    cv::imshow(window_name, img);
    cap >> frame;
    cv::imshow("capture-test", frame);
  } while (cv::waitKey(27) == -1);
  cv::destroyWindow("capture-test");
}
