#include "tools/spoof_aligner.h"
#include "utils/background.h"

int main() {
  const char* src =
      "/home/wuyong/Datasets/ffhq-nvidia-spoof/samsung-logitech1080P-night";
  const char* dst = "/home/wuyong/Datasets/align/samsung-logitech1080P-night";
  const int kTotalImages = 70000, kBatchSize = 1000, kQueueCapacity = 500;
  float scale = 0.5;
  Background background(1024, 1024, 20, 30, 500);
  std::vector<cv::Point2f> dst_points = background.corners();
  cv::Size dst_size = background.background_size(), crop_size(512, 512);
  dst_size.width *= scale;
  dst_size.height *= scale;
  for (auto& point : dst_points) {
    point *= scale;
  }

  SpoofAligner aligner(src, dst, kTotalImages, kBatchSize, kQueueCapacity,
                       dst_points, dst_size, crop_size);

  cv::Rect external(584, 193, 732, 734);
  cv::Rect internal(615, 227, 667, 668);
  cv::Rect text(898, 204, 103, 28);

  FeaturesParam param = {.max_corners = 200,
                         .quality_level = 0.01,
                         .min_distance = 3.0,
                         .block_size = 3,
                         .use_harris = false,
                         .k = 0.04};
  cv::TermCriteria criteria = cv::TermCriteria(
      cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 40, 0.01);
  aligner.Start(external, internal, text, param, criteria);
  return 0;
}