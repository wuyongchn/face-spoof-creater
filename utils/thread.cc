#include "utils/thread.h"
#include "utils/logging.h"

Thread::~Thread() { StopThread(); }

void Thread::StartThread() {
  LOG_ASSERT(!is_started());
  try {
    thread_.reset(new std::thread(&Thread::ThreadEntry, this));
  } catch (std::exception& e) {
    LOG_ERROR("%s:%d Thread exception: %s\n", __FILE__, __LINE__, e.what());
  }
}

void Thread::StopThread() {
  if (is_started()) {
    try {
      thread_->join();
    } catch (std::exception& e) {
      LOG_ERROR("%s:%d Thread exception: %s\n", __FILE__, __LINE__, e.what());
    }
  }
}

bool Thread::is_started() const { return thread_ && thread_->joinable(); }