#ifndef SPOOF_CREATER_TOOLS_SPOOF_ALIGNER_H_
#define SPOOF_CREATER_TOOLS_SPOOF_ALIGNER_H_

#include <opencv2/opencv.hpp>
#include <vector>
#include "unit/encode_dumping_unit.h"
#include "unit/loading_decode_unit.h"

struct FeaturesParam {
  int max_corners;
  double quality_level;
  double min_distance;
  int block_size;
  bool use_harris;
  double k;
};

using CVPoint2fVec = cv::vector<cv::Point2f>;

class SpoofAligner {
  SpoofAligner(const std::string& src, const std::string& dst, int total,
               int batch_size, int queue_capacity,
               const CVPoint2fVec& dst_points, const cv::Size& dst_size,
               const cv::Size& crop_size);
  ~SpoofAligner();
  void Start(const cv::Rect& external, const cv::Rect& internal,
             const cv::Rect& text, const FeaturesParam& param,
             const cv::TermCriteria& criteria);

 private:
  static void SortPoints(const CVPoint2fVec& points);
  CVPoint2fVec FindAlignBorder(const cv::Mat& src, const FeaturesParam& param,
                               const cv::TermCriteria& criteria);
  void FillRegion(cv::Mat& gray, const cv::Rect& region);

  int total_;
  std::atomic_bool loading_;
  CVMatQueue in_;
  CVMatQueue out_;
  LoadingDecodeUnit loading_decode_unit_;
  EncodeDumpingUnit encode_dumping_unit_;
  cv::vector<cv::Point2f> dst_points_;
  cv::Size dst_size_;
  cv::Size crop_size_;
};
#endif