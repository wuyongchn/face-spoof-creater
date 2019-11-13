#ifndef FACE_SPOOF_PROCTOOLS_UINT_DISPLAY_CAPTURE_UNIT_H_
#define FACE_SPOOF_PROCTOOLS_UINT_DISPLAY_CAPTURE_UNIT_H_

#include "internal_thread/capture_thread.h"
#include "internal_thread/display_thread.h"

class Background;
class DisplayCaptureUnit {
 public:
  DisplayCaptureUnit(CVMatQueue& in, CVMatQueue& out, Background* background,
                     int begin, int end);
  void Start(int display_delay, int capture_delay, int camera_id,
             const cv::Size& frame_size);
  void StartDisplay(int display_delay);
  void Stop();
  bool display_only() const { return display_only_; }

 private:
  void PrepareDisplay();
  void PrepareCapture();

  DisplayAction action_;
  DisplayThread display_thread_;
  CaptureThread capture_thread_;
  bool display_only_;
};

#endif  // FACE_SPOOF_PROCTOOLS__DISPLAY_CAPTURE_UNIT_H_
