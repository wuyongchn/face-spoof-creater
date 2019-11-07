#include "utils/background.h"

Background::Background(int img_width, int img_height, int delta1, int delta2,
                       int delta3)
    : background_(img_width + (delta1 * 2 + delta2) * 2,
                  img_height + (delta1 * 2 + delta2) * 2, CV_8UC3,
                  cv::Scalar::all(255)),
      img_rect_((delta1 * 2 + delta2), (delta1 * 2 + delta2), img_width,
                img_height),
      text_rect_(img_rect_.x + (img_rect_.width - delta3) / 2, delta1, delta3,
                 delta1 + delta2),
      text_origin_(),
      color_(),
      font_scale_(-1),
      thickness_(-1) {
  int delta = delta1 + delta2;
  background_(cv::Rect(delta, delta, background_.cols - delta * 2,
                       background_.rows - delta * 2)) = cv::Scalar::all(0);
  background_(cv::Rect((background_.cols - delta3) / 2, delta1, delta3,
                       background_.rows - delta1 * 2)) = cv::Scalar::all(0);
  background_(cv::Rect(delta1, (background_.cols - delta3) / 2,
                       background_.rows - delta1 * 2, delta3)) =
      cv::Scalar::all(0);
}

void Background::TextTemplate(const char* example, double font_scale,
                              int thickness, const cv::Scalar& color) {
  int baseline = 0;
  font_scale_ = font_scale;
  thickness_ = thickness;
  color_ = color;
  cv::Size text_size = cv::getTextSize(example, cv::FONT_HERSHEY_COMPLEX,
                                       font_scale, thickness_, &baseline);
  text_origin_ =
      cv::Point((background_.cols - text_size.width) / 2,
                text_rect_.y + (text_rect_.height + text_size.height) / 2);
}

void Background::PutText(const char* text) {
  background_(text_rect_) = cv::Scalar::all(0);
  cv::putText(background_, text, text_origin_, cv::FONT_HERSHEY_COMPLEX,
              font_scale_, color_, thickness_);
}