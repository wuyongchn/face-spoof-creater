#include "make_spoof.h"

MakeSpoof::MakeSpoof(const std::string& src, const std::string& dst, int total,
                     int batch_size, int queue_capacity, Background* background)
    : loading_(true),
      in_(queue_capacity),
      out_(queue_capacity),
      loading_decode_unit_(src, total, batch_size, in_, loading_),
      display_capture_unit_(in_, out_, background, total),
      encode_dumping_unit(out_, dst, loading_, total, batch_size / 10) {}

MakeSpoof::~MakeSpoof() = default;

void MakeSpoof::Start(int display_delay, int capture_delay, int camera_id,
                      const cv::Size& frame_size) {
  loading_decode_unit_.Start();
  display_capture_unit_.Start(display_delay, capture_delay, camera_id,
                              frame_size);
  encode_dumping_unit.Start();
}

void MakeSpoof::StartDisplay(int display_delay) {
  loading_decode_unit_.Start();
  display_capture_unit_.StartDisplay(display_delay);
}

void MakeSpoof::Stop() {
  loading_decode_unit_.Stop();
  display_capture_unit_.Stop();
  if (!display_capture_unit_.display_only()) {
    encode_dumping_unit.Stop();
  }
}
