#ifndef FACE_SPOOF_PROCTOOLS_UTILS_MOUSE_INPUT_H_
#define FACE_SPOOF_PROCTOOLS_UTILS_MOUSE_INPUT_H_

#include <opencv2/opencv.hpp>
#include <string>

class MouseInput {
 public:
  MouseInput(const std::string& winname, std::vector<cv::Point2f>& points,
             const cv::Mat& img);

 private:
  static void OnMouse(int event, int x, int y, int flags, void* userdata);

  struct UserData {
    UserData(std::vector<cv::Point2f>& points1, const cv::Mat& img1,
             const std::string& winname1);

    std::vector<cv::Point2f>& points;
    cv::Mat img;
    std::string winname;
  };

  UserData user_data_;
};

#endif