#ifndef FACE_SPOOF_PROCTOOLS_UINT_ENCODE_DUMPING_UNIT_H_
#define FACE_SPOOF_PROCTOOLS_UINT_ENCODE_DUMPING_UNIT_H_

#include "internal_thread/encode_thread.h"
#include "internal_thread/dumping_thread.h"

class EncodeDumpingUnit {
 public:
  EncodeDumpingUnit(CVMatQueue& queue, const std::string& source,
                    std::atomic_bool& loading, int total, int batch_size);
  ~EncodeDumpingUnit();
  void Start();
  void Stop();

 private:
  int fds_[2];
  std::atomic_bool dumping_;
  EncodedImgVecPair vec_pair_;
  EncodeThread encode_thread_;
  DumpingThread dumping_thread_;
};

#endif  // FACE_SPOOF_PROCTOOLS__ENCODE_DUMPING_UNIT_H_