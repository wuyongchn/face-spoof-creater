#include "utils/logging.h"
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <cstdio>
#include <cstdlib>

namespace internal {
static LogLevel evimg_log_level = LOG_INFO;

LogLevel get_log_level() { return evimg_log_level; }

void set_log_level(LogLevel level) { evimg_log_level = level; }

void defualt_log_handler(LogLevel level, const char* filename, int lineno,
                         const char* fmt, va_list ap) {
  static const char* levelnames[] = {"INFO", "WARNING", "ERROR", "FATAL"};
  char buf[kBufferSize];
  vsnprintf(buf, kBufferSize, fmt, ap);

  struct timeval tv;
  gettimeofday(&tv, NULL);
  struct tm* p = localtime(&tv.tv_sec);
  fprintf(stderr, "%c%d%02d%02d %02d%02d%02d.%06ld %s:%d] %s\n",
          levelnames[level][0], 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday,
          p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec, filename, lineno, buf);
  fflush(stderr);
  if (level > LOG_ERROR) {
    abort();
  }
}

void log_handler(LogLevel level, const char* filename, int line,
                 const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  defualt_log_handler(level, strrchr(filename, '/') + 1, line, fmt, ap);
  va_end(ap);
}

}  // namespace internal
