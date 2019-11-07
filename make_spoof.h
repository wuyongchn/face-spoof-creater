#ifndef MAKE_SPOOF__MAKE_SPOOF_H_
#define MAKE_SPOOF__MAKE_SPOOF_H_

#include <opencv2/opencv.hpp>
#include "unit/display_capture_unit.h"
#include "unit/encode_dumping_unit.h"
#include "unit/loading_decode_unit.h"

class Background;
class MakeSpoof {
 public:
  MakeSpoof(const std::string& src, const std::string& dst, int total,
            int batch_size, int queue_capacity,
            Background* background = nullptr);
  ~MakeSpoof();
  void Start(int display_delay, int capture_delay, int camera_id,
             const cv::Size& frame_size = cv::Size(0, 0));
  void StartDisplay(int display_delay);
  void Stop();

 private:
  std::atomic_bool loading_;
  CVMatQueue in_;
  CVMatQueue out_;
  LoadingDecodeUnit loading_decode_unit_;
  DisplayCaptureUnit display_capture_unit_;
  EncodeDumpingUnit encode_dumping_unit;
};

#endif  // MAKE_SPOOF__MAKE_SPOOF_H_
