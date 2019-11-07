set(INTERNAL_THREAD_SRCS
  decode_thread.cc
  encode_thread.cc
  display_thread.cc
  capture_thread.cc
  loading_thread.cc
  dumping_thread.cc
  )
add_library(internal_thread ${INTERNAL_THREAD_SRCS})