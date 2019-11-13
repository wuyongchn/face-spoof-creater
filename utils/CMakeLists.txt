set(UTILS_SRCS
  logging.cc
  io.cc
  background.cc
  thread.cc
  corner_detector.cc
  text_recognizer.cc
  mouse_input.cc  
  )
find_package(Threads REQUIRED)
add_library(utils ${UTILS_SRCS})