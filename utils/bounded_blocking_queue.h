#ifndef SPOOF_CREATER_UTILS_BOUNDED_BLOCKING_QUEUE_H_
#define SPOOF_CREATER_UTILS_BOUNDED_BLOCKING_QUEUE_H_

#include <condition_variable>
#include <mutex>
#include <vector>

template <typename T>
class BoundedBlockingQueue {
 public:
  explicit BoundedBlockingQueue(size_t capacity)
      : head_(0),
        tail_(0),
        mutex_(),
        not_full_(),
        not_empty_(),
        buffer_(capacity + 1) {}

  void push(const T& t);
  bool try_pop(T* t);
  T pop();
  bool try_peek(T* t);
  T peek();
  bool empty() const;
  bool full() const;
  size_t size() const;
  size_t capacity() const;

 private:
  bool buffer_empty() const { return head_ == tail_; }
  bool buffer_full() const { return (tail_ + 1) % buffer_.capacity() == head_; }

  size_t head_;
  size_t tail_;
  mutable std::mutex mutex_;
  std::condition_variable not_full_;
  std::condition_variable not_empty_;
  std::vector<T> buffer_;
};

template <typename T>
inline void BoundedBlockingQueue<T>::push(const T& t) {
  std::unique_lock<std::mutex> lock(mutex_);
  while (buffer_full()) {
    not_full_.wait(lock);
  }
  buffer_[tail_] = t;
  tail_ = (tail_ + 1) % buffer_.capacity();
  not_empty_.notify_one();
}

template <typename T>
inline bool BoundedBlockingQueue<T>::try_pop(T* t) {
  std::unique_lock<std::mutex> lock(mutex_);
  if (buffer_empty()) {
    return false;
  }
  *t = buffer_[head_];
  //  buffer_.erase(head_);
  head_ = (head_ + 1) % buffer_.capacity();
  not_empty_.notify_one();
  return true;
}

template <typename T>
inline T BoundedBlockingQueue<T>::pop() {
  std::unique_lock<std::mutex> lock(mutex_);
  while (buffer_empty()) {
    not_empty_.wait(lock);
  }
  T t(buffer_[head_]);
  //  buffer_.erase(head_);
  head_ = (head_ + 1) % buffer_.capacity();
  not_full_.notify_one();
  return t;
}

template <typename T>
inline bool BoundedBlockingQueue<T>::try_peek(T* t) {
  std::unique_lock<std::mutex> lock(mutex_);
  if (buffer_empty()) {
    return false;
  }
  *t = buffer_[head_];
  return true;
}

template <typename T>
inline T BoundedBlockingQueue<T>::peek() {
  std::unique_lock<std::mutex> lock(mutex_);
  while (buffer_empty()) {
    not_empty_.wait(lock);
  }
  T t(buffer_[head_]);
  return t;
}

template <typename T>
inline bool BoundedBlockingQueue<T>::empty() const {
  std::unique_lock<std::mutex> lock(mutex_);
  return buffer_empty();
}

template <typename T>
inline bool BoundedBlockingQueue<T>::full() const {
  std::unique_lock<std::mutex> lock(mutex_);
  return buffer_full();
}

template <typename T>
inline size_t BoundedBlockingQueue<T>::size() const {
  std::unique_lock<std::mutex> lock(mutex_);
  size_t capacity = buffer_.capacity();
  return (tail_ + capacity - head_) % capacity;
}

template <typename T>
inline size_t BoundedBlockingQueue<T>::capacity() const {
  std::unique_lock<std::mutex> lock(mutex_);
  return buffer_.capacity() - 1;
}

#endif  // SPOOF_CREATER_UTILS_BOUNDED_BLOCKING_QUEUE_H_
