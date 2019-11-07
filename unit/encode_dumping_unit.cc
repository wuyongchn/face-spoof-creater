#include "unit/encode_dumping_unit.h"
#include <sys/socket.h>
#include <unistd.h>

EncodeDumpingUnit::EncodeDumpingUnit(CVMatQueue& queue,
                                     const std::string& source,
                                     std::atomic_bool& loading, int total,
                                     int batch_size)
    : dumping_(false),
      vec_pair_(),
      encode_thread_(vec_pair_, queue, dumping_, total, batch_size),
      dumping_thread_(vec_pair_, source, dumping_, loading, total) {
  if (::socketpair(AF_LOCAL, SOCK_STREAM, 0, fds_)) {
    printf("%s:%d socketpair error, %s", __FILE__, __LINE__, strerror(errno));
    exit(0);
  }
  encode_thread_.set_fd(fds_[0]);
  dumping_thread_.set_fd(fds_[1]);
}
EncodeDumpingUnit::~EncodeDumpingUnit() {
  ::close(fds_[0]);
  ::close(fds_[1]);
}
void EncodeDumpingUnit::Start() {
  encode_thread_.StartThread();
  dumping_thread_.StartThread();
}
void EncodeDumpingUnit::Stop() {
  encode_thread_.StopThread();
  dumping_thread_.StopThread();
}