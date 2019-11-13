#include "utils/text_recognizer.h"

TextRecognizer::TextRecognizer(const std::string& language)
    : api_(new tesseract::TessBaseAPI()) {
  api_->Init(nullptr, language.c_str());
  api_->SetPageSegMode(tesseract::PSM_SINGLE_LINE);
}

TextRecognizer::~TextRecognizer() { api_->End(); }

std::string TextRecognizer::Recognize(const cv::Mat& img) {
  cv::Mat gray;
  if (img.channels() == 3) {
    cv::cvtColor(img, gray, cv::COLOR_RGB2GRAY);
  } else {
    gray = img.clone();
  }
  BinaryProc(gray);
  std::string ans(api_->TesseractRect(gray.data, gray.elemSize1(), gray.step, 0,
                                      0, gray.cols, gray.rows));
  while (!ans.empty() && ans.back() == '\n') {
    ans.pop_back();
  }
}

void TextRecognizer::BinaryProc(cv::Mat& gray) {
  cv::medianBlur(gray, gray, 3);
  cv::threshold(gray, gray, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
  int row1 = 0, row2 = 0;
  for (int i = gray.rows / 2, j = 0; i >= 0; --i) {
    for (j = 0; j < gray.cols; ++j) {
      if (!gray.at<uchar>(i, j)) {
        break;
      }
    }
    if (j == gray.cols) {
      row1 = i;
      break;
    }
  }

  for (int i = gray.rows / 2, j = 0; i < gray.rows; ++i) {
    for (j = 0; j < gray.cols; ++j) {
      if (!gray.at<uchar>(i, j)) {
        break;
      }
    }
    if (j == gray.cols) {
      row2 = i;
      break;
    }
  }
  if (row1) {
    gray(cv::Rect(0, 0, gray.cols, row1)) = 255;
  }
  if (row2) {
    gray(cv::Rect(0, row2, gray.cols, gray.rows - row2)) = 255;
  }
}