#ifndef MAKE_SPOOF_UINT_LOADING_DECODE_UNIT_H_
#define MAKE_SPOOF_UINT_LOADING_DECODE_UNIT_H_

#include "internal_thread/loading_thread.h"
#include "internal_thread/decode_thread.h"

class LoadingDecodeUnit {
 public:
  LoadingDecodeUnit(const std::string& src, int total, int batch_size,
                    CVMatQueue& queue, std::atomic_bool& loading);
  ~LoadingDecodeUnit();
  void Start();
  void Stop();

 private:
  int fds_[2];
  EncodedImgVec vec_;
  LoadingThread loading_thread_;
  DecodeThread decode_thread_;
};

#endif  // MAKE_SPOOF__LOADING_DECODE_UNIT_H_