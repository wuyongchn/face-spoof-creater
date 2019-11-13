#include <X11/Xlib.h>
#include "tools/spoof_capture.h"
#include "utils/background.h"

int main() {
  const char* src = "/home/wuyong/Datasets/ffhq-nvidia/image1024x1024-jpg";
  const char* dst = "/home/wuyong/Desktop/capture";
  int begin = 0, end = 70000, batch_size = 1000, queue_capacity = 500,
      delay1 = 60, delay2 = 1, camera_id = 0;
  const cv::Size frame_size(1280, 720);
  Background background(1024, 1024, 20, 30, 500);
  background.TextTemplate("12345", 1.5, 2, cv::Scalar::all(255));
  XInitThreads();

  SpoofCapture make_spoof(src, dst, begin, end, batch_size, queue_capacity,
                          &background);
  // make_spoof.Start(kDelay1, kDelay2, kCameraId, frame_size);
  // make_spoof.Start(kDelay1, kDelay2, kCameraId);
  make_spoof.StartDisplay(delay1);
  make_spoof.Stop();
  return 0;
}
