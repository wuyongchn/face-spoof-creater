#ifndef MAKE_SPOOF_UTILS_BACKGROUND_H_
#define MAKE_SPOOF_UTILS_BACKGROUND_H_

#include <opencv2/opencv.hpp>
class Background {
 public:
  Background(int img_width, int img_height, int delta1, int delta2, int delta3);
  cv::Mat background() const { return background_; }
  cv::Rect img_rect() const { return img_rect_; }
  cv::Rect text_rect() const { return text_rect_; }
  double font_scale() const { return font_scale_; }
  void set_font_scale(double font_scale) { font_scale_ = font_scale; }
  int thickness() const { return thickness_; }
  void set_thickness(int thickness) { thickness_ = thickness; }
  void PutImage(const cv::Mat& img) { img.copyTo(background_(img_rect_)); }
  void TextTemplate(const char* example, double font_scale, int thickness,
                    const cv::Scalar& color);
  void PutText(const char* text);
  void PutImageAndText(const cv::Mat& img, const char* text) {
    PutText(text);
    PutImage(img);
  }

 private:
  cv::Mat background_;
  cv::Rect img_rect_;
  cv::Rect text_rect_;
  cv::Point text_origin_;
  cv::Scalar color_;
  double font_scale_;
  int thickness_;
};

#endif  // MAKE_SPOOF_UTILS_BACKGROUND_H_
