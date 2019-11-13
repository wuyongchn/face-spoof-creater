#include "tools/spoof_align.h"

SpoofAlign::SpoofAlign(const std::string& src, const std::string& dst,
                           int total, int batch_size, int queue_capacity,
                           const CVPoint2fVec& dst_points,
                           const cv::Size& dst_size, const cv::Size& crop_size)
    : total_(total),
      loading_(true),
      in_(queue_capacity),
      out_(queue_capacity),
      loading_decode_unit_(src, total, batch_size, in_, loading_),
      encode_dumping_unit_(out_, dst, loading_, total, batch_size / 10),
      dst_points_(dst_points),
      dst_size_(dst_size),
      crop_size_(crop_size) {
  SortPoints(dst_points_);
}

SpoofAlign::~SpoofAlign() {
  loading_decode_unit_.Stop();
  encode_dumping_unit_.Stop();
}

void SpoofAlign::Start(const cv::Rect& external, const cv::Rect& internal,
                         const cv::Rect& text, const FeaturesParam& param,
                         const cv::TermCriteria& criteria) {
  loading_decode_unit_.Start();
  encode_dumping_unit_.Start();

  cv::Rect crop_rect((dst_size_.width - crop_size_.width) / 2,
                     (dst_size_.height - crop_size_.height) / 2,
                     crop_size_.width, crop_size_.height);
  cv::Mat gray;
  CVPoint2fVec src_points;
  for (int i = 0; i < total_; ++i) {
    cv::Mat img = in_.pop();
    cv::cvtColor(img, gray, cv::COLOR_RGB2GRAY);
    FillRegion(gray, internal);
    FillRegion(gray, text);
    src_points = FindAlignBorder(gray(external), param, criteria);
    for (auto& point : src_points) {
      point += cv::Point2f(external.x, external.y);
    }
    cv::Mat h = cv::findHomography(src_points, dst_points_);
    cv::Mat img_align;
    warpPerspective(img, img_align, h, dst_size_);
    img_align = img_align(crop_rect).clone();
    out_.push(img_align);
  }
}

void SpoofAlign::SortPoints(CVPoint2fVec& points) {
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

CVPoint2fVec SpoofAlign::FindAlignBorder(const cv::Mat& gray,
                                           const FeaturesParam& param,
                                           const cv::TermCriteria& criteria) {
  std::vector<cv::Point2f> corners;
  cv::goodFeaturesToTrack(gray, corners, param.max_corners, param.quality_level,
                          param.min_distance, cv::Mat(), param.block_size,
                          param.use_harris, param.k);
  cv::cornerSubPix(gray, corners, cv::Size(5, 5), cv::Size(-1, -1), criteria);
  std::vector<cv::Point2f> border(corners.begin(), corners.begin() + 20);
  SortPoints(border);
  return border;
}

void SpoofAlign::FillRegion(cv::Mat& gray, const cv::Rect& region) {
  uchar c1 = gray.at<uchar>(region.tl());
  uchar c2 = gray.at<uchar>(region.br());
  uchar c3 = gray.at<uchar>(region.tl() + cv::Point(region.width, 0));
  uchar c4 = gray.at<uchar>(region.tl() + cv::Point(0, region.height));
  gray(region) = (c1 + c2 + c3 + c4) / 4;
}