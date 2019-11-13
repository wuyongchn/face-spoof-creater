#ifndef FACE_SPOOF_PROCTOOLS_UINT_LOADING_DECODE_UNIT_H_
#define FACE_SPOOF_PROCTOOLS_UINT_LOADING_DECODE_UNIT_H_

#include "internal_thread/decode_thread.h"
#include "internal_thread/loading_thread.h"

class LoadingDecodeUnit {
 public:
  LoadingDecodeUnit(const std::string& src, CVMatQueue& queue,
                    std::atomic_bool& loading, int batch_size, int begin,
                    int end);
  ~LoadingDecodeUnit();
  void Start();
  void Stop();

 private:
  int fds_[2];
  EncodedImgVec vec_;
  LoadingThread loading_thread_;
  DecodeThread decode_thread_;
};

#endif  // FACE_SPOOF_PROCTOOLS__LOADING_DECODE_UNIT_H_