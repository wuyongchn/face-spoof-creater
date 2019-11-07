#include "internal_thread/dumping_thread.h"
#include <unistd.h>
#include "utils/io.h"

DumpingThread::DumpingThread(EncodedImgVecPair& vec_pair,
                             const std::string& source,
                             std::atomic_bool& dumping,
                             std::atomic_bool& loading, int total)
    : Thread(),
      vec_pair_(vec_pair),
      source_(source),
      dumping_(dumping),
      loading_(loading),
      total_(total),
      fd_(-1) {}

void DumpingThread::ThreadEntry() {
  char dummy[1] = {}, name[128];
  for (int i = 0; i < total_;) {
    read(fd_, dummy, sizeof(dummy));  // wait for dumping
    dumping_.store(true);
    for (auto& data_vec : vec_pair_.second) {
      snprintf(name, sizeof(name), "%s/%d/%05d.jpg", source_.c_str(), i / 10000,
               i);
      while (loading_.load()) {  // loading, wait for ending.
        usleep(50 * 1000);       // the value should be check
      }
      WriteFile(name, data_vec);
      ++i;
    }
    vec_pair_.second.clear();
    dumping_.store(false);
  }
}