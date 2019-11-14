#include "tools/spoof_align.h"
#include "utils/background.h"
#include "utils/corner_detector.h"
#include "utils/mouse_input.h"

void GetInitPoints(const char *src, int begin,
                   std::vector<cv::Point2f> &init_points) {
  char name[128];
  cv::namedWindow("test", cv::WINDOW_NORMAL);
  snprintf(name, sizeof(name), "%s/%d/%06d.jpg", src, begin / 20000, begin);
  cv::Mat img = cv::imread(name, CV_LOAD_IMAGE_COLOR);
  cv::imshow("test", img);
  MouseInput input_point("test", init_points, img);
  cv::waitKey();
  cv::destroyWindow("test");
}

void Proc(const char *src, const char *dst, int begin, int end,
          std::vector<cv::Point2f> &init_points) {
  int batch_size = 1000, queue_capacity = 500;
  float scale = 0.5;
  Background background(1024, 1024, 20, 30, 500);
  std::vector<cv::Point2f> dst_points = background.corners();
  cv::Size dst_size = background.background_size(), crop_size(512, 512);
  cv::Rect text_rect(245, 10, 85, 25);

  for (auto &point : dst_points) {
    point *= scale;
  }
  dst_size.width *= scale;
  dst_size.height *= scale;

  CornerParam param = {.max_corners = 200,
                       .quality_level = 0.01,
                       .min_distance = 3.0,
                       .block_size = 3,
                       .use_harris = false,
                       .k = 0.04};
  cv::TermCriteria criteria = cv::TermCriteria(
      cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 40, 0.01);
  CornerDetector detector(init_points, cv::Size(30, 30), param, criteria);

  GetText get_text(text_rect, dst, batch_size * 10);

  SpoofAlign aligner(src, dst, batch_size, begin, end, queue_capacity,
                     dst_points, dst_size, crop_size, detector, &get_text);
  std::cout << "processing... " << src << std::endl;
  aligner.Start();
}

int main() {
  {
    const char *src = "/home/wuyong/Datasets/ffhq-nvidia-spoof/asus-huawei-day";
    const char *dst = "/home/wuyong/Datasets/align/asus-huawei-day";
    int begin = 49, end = 209746;
    std::vector<cv::Point2f> init_points = {
        {147, 276},  {367, 272},  {370, 249},  {758, 254},  {760, 278},
        {976, 280},  {978, 498},  {998, 503},  {996, 885},  {972, 885},
        {969, 1098}, {754, 1100}, {754, 1120}, {372, 1123}, {372, 1103},
        {156, 1101}, {154, 887},  {130, 887},  {123, 498},  {150, 496}};
    Proc(src, dst, begin, end, init_points);
  }
  {
    const char *src =
        "/home/wuyong/Datasets/ffhq-nvidia-spoof/asus-huawei-night";
    const char *dst = "/home/wuyong/Datasets/align/asus-huawei-night";
    int begin = 45, end = 215528;
    std::vector<cv::Point2f> init_points = {
        {185, 430},  {383, 420},  {385, 400},  {743, 385},  {743, 405},
        {952, 400},  {956, 605},  {978, 607},  {983, 972},  {958, 970},
        {963, 1176}, {756, 1178}, {754, 1200}, {396, 1198}, {400, 1178},
        {203, 1178}, {198, 985},  {178, 981},  {165, 629},  {190, 629}};
    Proc(src, dst, begin, end, init_points);
  }
  {
    const char *src =
        "/home/wuyong/Datasets/ffhq-nvidia-spoof/samsung-huawei-day";
    const char *dst = "/home/wuyong/Datasets/align/samsung-huawei-day";
    int begin = 82, end = 212056;
    std::vector<cv::Point2f> init_points = {
        {136, 450},  {350, 445},  {352, 420},  {732, 420},  {736, 440},
        {947, 438},  {949, 656},  {969, 658},  {969, 1027}, {947, 1029},
        {945, 1232}, {740, 1234}, {740, 1256}, {374, 1263}, {372, 1243},
        {163, 1247}, {158, 1043}, {134, 1043}, {121, 669},  {143, 669}};
    Proc(src, dst, begin, end, init_points);
  }
  {
    const char *src =
        "/home/wuyong/Datasets/ffhq-nvidia-spoof/samsung-huawei-night";
    const char *dst = "/home/wuyong/Datasets/align/samsung-huawei-night";
    int begin = 82, end = 212757;
    std::vector<cv::Point2f> init_points = {
        {118, 454},  {347, 450},  {349, 425},  {765, 427},   {767, 452},
        {1005, 452}, {1000, 687}, {1025, 689}, {1016, 1101}, {990, 1103},
        {989, 1332}, {758, 1329}, {758, 1350}, {354, 1341},  {352, 1323},
        {127, 1316}, {125, 1098}, {103, 1094}, {98, 689},    {121, 687}};
    Proc(src, dst, begin, end, init_points);
  }
  return 0;
}