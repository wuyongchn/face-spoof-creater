#include "internal_thread/dumping_thread.h"
#include <sys/stat.h>
#include <unistd.h>
#include "utils/io.h"

DumpingThread::DumpingThread(EncodedImgVecPair& vec_pair,
                             const std::string& source,
                             std::atomic_bool& dumping,
                             std::atomic_bool& loading, int begin, int end,
                             int fd)
    : Thread(),
      vec_pair_(vec_pair),
      source_(source),
      dumping_(dumping),
      loading_(loading),
      begin_(begin),
      end_(end),
      fd_(fd) {}

void DumpingThread::ThreadEntry() {
  char dummy[1] = {}, name[128];
  for (int i = begin_; i < end_; i += 10000) {
    snprintf(name, sizeof(name), "%s/%d", source_.c_str(), i / 10000);
    if (mkdir(name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
      if (errno != EEXIST) {
        std::cout << "cannot create sessionnamefolder error:" << strerror(errno)
                  << std::endl;
        exit(1);
      }
    }
  }
  for (int i = begin_; i < end_;) {
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