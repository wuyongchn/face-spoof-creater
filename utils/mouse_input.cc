#include "utils/mouse_input.h"

MouseInput::MouseInput(const std::string& winname,
                       std::vector<cv::Point2f>& points, const cv::Mat& img)
    : user_data_(points, img, winname) {
  cv::setMouseCallback(winname, OnMouse, &user_data_);
}

void MouseInput::OnMouse(int event, int x, int y, int flags, void* userdata) {
  auto ptr = static_cast<UserData*>(userdata);
  if (event == cv::EVENT_LBUTTONDOWN) {
    ptr->points.emplace_back(x, y);
    cv::circle(ptr->img, cv::Point(x, y), 2, cv::Scalar(0, 0, 255));
    cv::imshow(ptr->winname, ptr->img);
  }
}

MouseInput::UserData::UserData(std::vector<cv::Point2f>& points1,
                               const cv::Mat& img1, const std::string& winname1)
    : points(points1), img(img1.clone()), winname(winname1) {
  if (img.channels() == 1) {
    cv::cvtColor(img, img, cv::COLOR_GRAY2RGB);
  }
}