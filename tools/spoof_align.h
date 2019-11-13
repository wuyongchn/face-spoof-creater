#ifndef FACE_SPOOF_PROCTOOLS_TOOLS_SPOOF_ALIGNER_H_
#define FACE_SPOOF_PROCTOOLS_TOOLS_SPOOF_ALIGNER_H_

#include <opencv2/opencv.hpp>
#include <vector>
#include "unit/encode_dumping_unit.h"
#include "unit/loading_decode_unit.h"
#include "utils/text_recognizer.h"

class CornerDetector;
class SpoofAlign {
 public:
  class PostAlign {
   public:
    virtual ~PostAlign() = default;
    virtual void Proc(const cv::Mat& align_img, int idx) = 0;
  };

 public:
  SpoofAlign(const std::string& src, const std::string& dst, int batch_size,
             int begin, int end, int queue_capacity,
             const std::vector<cv::Point2f>& dst_points,
             const cv::Size& dst_size, const cv::Size& crop_size,
             CornerDetector& detector, PostAlign* post_align = nullptr);
  ~SpoofAlign();
  void Start();

 private:
  int begin_;
  int end_;
  std::atomic_bool loading_;
  CVMatQueue in_;
  CVMatQueue out_;
  LoadingDecodeUnit loading_decode_unit_;
  EncodeDumpingUnit encode_dumping_unit_;
  std::vector<cv::Point2f> dst_points_;
  cv::Size dst_size_;
  cv::Size crop_size_;
  CornerDetector& detector_;
  PostAlign* post_align_;
};

class GetText : public SpoofAlign::PostAlign {
 public:
  GetText(const cv::Rect& text_rect, const std::string& dst, int batch_size);
  void Proc(const cv::Mat& align_img, int idx) override;

 private:
  void Dump();
  std::string GenName(bool current = false);

  cv::Rect text_rect_;
  TextRecognizer recognizer_;
  std::string dst_;
  std::vector<std::string> buf_;
  int batch_size_;
  int count_;
};
#endif