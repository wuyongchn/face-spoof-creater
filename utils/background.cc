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
      thickness_(-1),
      corners_() {
  int delta = delta1 + delta2;
  background_(cv::Rect(delta, delta, background_.cols - delta * 2,
                       background_.rows - delta * 2)) = cv::Scalar::all(0);
  background_(cv::Rect((background_.cols - delta3) / 2, delta1, delta3,
                       background_.rows - delta1 * 2)) = cv::Scalar::all(0);
  background_(cv::Rect(delta1, (background_.cols - delta3) / 2,
                       background_.rows - delta1 * 2, delta3)) =
      cv::Scalar::all(0);

  Corners(delta1, delta2, delta3);
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

void Background::Corners(int delta1, int delta2, int delta3) {
  corners_.reserve(20);
  int background_width = background_.cols;
  int background_height = background_.rows;
  // starting from left-up point with clockwise order
  corners_.emplace_back(cv::Point2f(delta1 + delta2, delta1 + delta2));  // #1
  corners_.emplace_back(
      cv::Point2f((background_width - delta3) / 2, corners_.back().y));
  corners_.emplace_back(cv::Point2f(corners_.back().x, delta1));  // #3
  corners_.emplace_back(
      cv::Point2f((background_width + delta3) / 2, corners_.back().y));
  corners_.emplace_back(cv::Point2f(corners_.back().x, delta1 + delta2));  // #5
  corners_.emplace_back(
      cv::Point2f(background_width - delta1 - delta2, corners_.back().y));
  corners_.emplace_back(
      cv::Point2f(corners_.back().x, (background_height - delta3) / 2));  // #7
  corners_.emplace_back(
      cv::Point2f(background_width - delta1, corners_.back().y));
  corners_.emplace_back(
      cv::Point2f(corners_.back().x, (background_height + delta3) / 2));  // #9
  corners_.emplace_back(
      cv::Point2f(corners_.back().x - delta2, corners_.back().y));
  corners_.emplace_back(cv::Point2f(
      corners_.back().x, background_height - delta1 - delta2));  // #11
  corners_.emplace_back(
      cv::Point2f((background_width + delta3) / 2, corners_.back().y));
  corners_.emplace_back(
      cv::Point2f(corners_.back().x, corners_.back().y + delta2));  // #13
  corners_.emplace_back(
      cv::Point2f((background_width - delta3) / 2, corners_.back().y));
  corners_.emplace_back(
      cv::Point2f(corners_.back().x, corners_.back().y - delta2));  // #15
  corners_.emplace_back(cv::Point2f(delta1 + delta2, corners_.back().y));
  corners_.emplace_back(
      cv::Point2f(corners_.back().x, (background_height + delta3) / 2));  // #17
  corners_.emplace_back(cv::Point2f(delta1, corners_.back().y));
  corners_.emplace_back(
      cv::Point2f(corners_.back().x, (background_height - delta3) / 2));  // #19
  corners_.emplace_back(cv::Point2f(delta1 + delta2, corners_.back().y));
}