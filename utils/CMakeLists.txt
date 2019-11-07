set(UTILS_SRCS
  logging.cc
  io.cc
  background.cc
  thread.cc
  )
find_package(Threads REQUIRED)
add_library(utils ${UTILS_SRCS})