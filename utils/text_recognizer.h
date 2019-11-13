#ifndef FACE_SPOOF_PROCTOOLS_UTILS_TEXT_RECOGNIZER_H
#define FACE_SPOOF_PROCTOOLS_UTILS_TEXT_RECOGNIZER_H

#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include <opencv2/opencv.hpp>
#include <string>

class TextRecognizer {
 public:
  explicit TextRecognizer(const std::string& language = "eng");
  ~TextRecognizer();
  std::string Recognize(const cv::Mat& img);

 private:
 void BinaryProc(cv::Mat& gray);
  tesseract::TessBaseAPI* api_;
};
#endif