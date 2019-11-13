#include "unit/loading_decode_unit.h"
#include <sys/socket.h>
#include <unistd.h>

LoadingDecodeUnit::LoadingDecodeUnit(const std::string& src, CVMatQueue& queue,
                                     std::atomic_bool& loading, int batch_size,
                                     int begin, int end)
    : vec_(),
      loading_thread_(src, vec_, loading, batch_size, begin, end),
      decode_thread_(vec_, queue, loading, begin, end) {
  if (::socketpair(AF_LOCAL, SOCK_STREAM, 0, fds_)) {
    printf("%s:%d socketpair error, %s", __FILE__, __LINE__, strerror(errno));
    exit(0);
  }
  loading_thread_.set_fd(fds_[0]);
  decode_thread_.set_fd(fds_[1]);
}

LoadingDecodeUnit::~LoadingDecodeUnit() {
  ::close(fds_[0]);
  ::close(fds_[1]);
}

void LoadingDecodeUnit::Start() {
  loading_thread_.StartThread();
  decode_thread_.StartThread();
}

void LoadingDecodeUnit::Stop() {
  loading_thread_.StopThread();
  decode_thread_.StopThread();
}