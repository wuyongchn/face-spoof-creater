#ifndef FACE_SPOOF_PROCTOOLS_UTILS_CORNER_DETECTOR_H_
#define FACE_SPOOF_PROCTOOLS_UTILS_CORNER_DETECTOR_H_

#include <opencv2/opencv.hpp>
#include <vector>

struct CornerParam {
  int max_corners;
  double quality_level;
  double min_distance;
  int block_size;
  bool use_harris;
  double k;
};

class CornerDetector {
 public:
  explicit CornerDetector(const std::vector<cv::Point2f>& init_pos,
                          const cv::Size& size, const CornerParam& param,
                          const cv::TermCriteria& criteria);
  std::vector<cv::Point2f> Detect(const cv::Mat& img);
  static void SortPoints(std::vector<cv::Point2f>& points);

 private:
  std::vector<cv::Rect> RectOutPoint(const cv::Size& img_size);
  std::vector<cv::Point2f> CornersInLocal(const cv::Mat& gray,
                                          const cv::Rect& rect);
  void Check(const std::vector<cv::Point2f>& points, int idx);

  std::vector<cv::Point2f> points_;
  cv::Size size_;
  CornerParam param_;
  cv::TermCriteria criteria_;
};

#endif