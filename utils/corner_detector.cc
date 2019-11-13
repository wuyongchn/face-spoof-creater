#include "utils/corner_detector.h"
#include <assert.h>

CornerDetector::CornerDetector(const std::vector<cv::Point2f>& init_pos,
                               const cv::Size& size, const CornerParam& param,
                               const cv::TermCriteria& criteria)
    : points_(init_pos), size_(size), param_(param), criteria_(criteria) {}

std::vector<cv::Point2f> CornerDetector::Detect(const cv::Mat& img) {
  assert(img.channels() == 1 || img.channels() == 3);
  cv::Mat gray;
  if (img.channels() == 3) {
    cv::cvtColor(img, gray, cv::COLOR_RGB2GRAY);
  } else {
    gray = img.clone();
  }
  std::vector<cv::Rect> rects = RectOutPoint(gray.size());
  for (int i = 0; i < rects.size(); ++i) {
    std::vector<cv::Point2f> points = CornersInLocal(gray, rects[i]);
    Check(points, i);
  }
  cv::cornerSubPix(gray, points_, cv::Size(5, 5), cv::Size(-1, -1), criteria_);
  return points_;
}

std::vector<cv::Rect> CornerDetector::RectOutPoint(const cv::Size& img_size) {
  std::vector<cv::Rect> ans;
  cv::Rect rect;
  for (auto& point : points_) {
    rect.x = std::max(point.x - size_.width / 2, 0.0F);
    rect.y = std::max(point.y - size_.height / 2, 0.0F);
    rect.width = std::min(size_.width, img_size.width - rect.x);
    rect.height = std::min(size_.height, img_size.height - rect.y);
    ans.emplace_back(rect);
  }
  return ans;
}

std::vector<cv::Point2f> CornerDetector::CornersInLocal(const cv::Mat& gray,
                                                        const cv::Rect& rect) {
  std::vector<cv::Point2f> corners;
  cv::goodFeaturesToTrack(gray(rect), corners, param_.max_corners,
                          param_.quality_level, param_.min_distance, cv::Mat(),
                          param_.block_size, param_.use_harris, param_.k);
  for (auto& corner : corners) {
    corner.x += rect.x;
    corner.y += rect.y;
  }
  return corners;
}

void CornerDetector::Check(const std::vector<cv::Point2f>& points, int idx) {
  if (points.empty()) {
    return;
  }
  cv::Point2f diff;
  float distance, min_value = 100;
  int target = -1;
  for (int i = 0; i < points.size(); ++i) {
    diff = points[i] - points_[idx];
    distance = diff.x * diff.x + diff.y * diff.y;
    if (distance < min_value) {
      min_value = distance;
      target = i;
    }
  }
  if (target != -1) {
    points_[idx] = points[target];
  }
}

void CornerDetector::SortPoints(std::vector<cv::Point2f>& points) {
  if (points.empty()) {
    return;
  }
  cv::Point2f center(0, 0);
  center = std::accumulate(points.begin(), points.end(), center);
  center.x /= points.size();
  center.y /= points.size();

  auto less = [&center](const cv::Point2f& a, const cv::Point2f& b) {
    if (a.x - center.x >= 0 && b.x - center.x < 0) return true;
    if (a.x - center.x < 0 && b.x - center.x >= 0) return false;
    if (a.x - center.x == 0 && b.x - center.x == 0) {
      if (a.y - center.y >= 0 || b.y - center.y >= 0) return a.y > b.y;
      return b.y > a.y;
    }

    // compute the cross product of vectors (center -> a) x (center -> b)
    double det = (a.x - center.x) * (b.y - center.y) -
                 (b.x - center.x) * (a.y - center.y);
    if (det < 0) return true;
    if (det > 0) return false;

    // points a and b are on the same line from the center
    // check which point is closer to the center
    double d1 = (a.x - center.x) * (a.x - center.x) +
                (a.y - center.y) * (a.y - center.y);
    double d2 = (b.x - center.x) * (b.x - center.x) +
                (b.y - center.y) * (b.y - center.y);
    return d1 > d2;
  };
  sort(points.begin(), points.end(), less);
}