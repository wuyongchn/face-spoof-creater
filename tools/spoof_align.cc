#include "tools/spoof_align.h"
#include <fstream>
#include "utils/corner_detector.h"

SpoofAlign::SpoofAlign(const std::string& src, const std::string& dst,
                       int batch_size, int begin, int end, int queue_capacity,
                       const std::vector<cv::Point2f>& dst_points,
                       const cv::Size& dst_size, const cv::Size& crop_size,
                       CornerDetector& detector, PostAlign* post_align)
    : begin_(begin),
      end_(end),
      loading_(true),
      in_(queue_capacity),
      out_(queue_capacity),
      loading_decode_unit_(src, in_, loading_, batch_size, begin, end),
      encode_dumping_unit_(out_, dst, loading_, batch_size / 10, begin, end),
      dst_points_(dst_points),
      dst_size_(dst_size),
      crop_size_(crop_size),
      detector_(detector),
      post_align_(post_align) {
  CornerDetector::SortPoints(dst_points_);
}

SpoofAlign::~SpoofAlign() {
  loading_decode_unit_.Stop();
  encode_dumping_unit_.Stop();
}

void SpoofAlign::Start() {
  loading_decode_unit_.Start();
  encode_dumping_unit_.Start();

  cv::Rect crop_rect((dst_size_.width - crop_size_.width) / 2,
                     (dst_size_.height - crop_size_.height) / 2,
                     crop_size_.width, crop_size_.height);
  cv::Mat gray, img_align, h;
  std::vector<cv::Point2f> src_points;
  for (int i = begin_; i < end_; ++i) {
    if (i % 5000 == 0) {
      std::cout << "processing... " << i << std::endl;
    }
    cv::Mat img = in_.pop();
    src_points = detector_.Detect(img);
    CornerDetector::SortPoints(src_points);
    h = cv::findHomography(src_points, dst_points_);
    warpPerspective(img, img_align, h, dst_size_);
    cv::Mat cropped_img = img_align(crop_rect).clone();
    out_.push(cropped_img);
    if (post_align_) {
      post_align_->Proc(img_align, i);
    }
  }
}

GetText::GetText(const cv::Rect& text_rect, const std::string& dst,
                 int batch_size)
    : text_rect_(text_rect),
      recognizer_(),
      dst_(dst),
      buf_(),
      batch_size_(batch_size),
      count_(0) {}

void GetText::Proc(const cv::Mat& align_img, int idx) {
  cv::Mat text_img = align_img(text_rect_);
  std::string text = recognizer_.Recognize(text_img);
  buf_.emplace_back(std::to_string(idx) + ' ' + text);
  if (buf_.size() == batch_size_) {
    Dump();
    buf_.clear();
  }
}

void GetText::Dump() {
  std::string filename = GenName();
  std::ostringstream oss;
  for (auto& line : buf_) {
    oss << line << '\n';
  }
  std::ofstream out(filename);
  if (out.is_open()) {
    out << oss.str();
  } else {
    filename = GenName(true);
    out.open(filename);
    out << oss.str();
  }
}

std::string GetText::GenName(bool current) {
  std::string ans;
  if (current) {
    ans = std::string("part_") + std::to_string(count_) + ".txt";
  } else {
    ans = dst_ + std::string("/part_") + std::to_string(count_) + ".txt";
  }
  ++count_;
  return ans;
}