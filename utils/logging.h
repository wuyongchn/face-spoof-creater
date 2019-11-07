#ifndef MAKE_SPOOF_UTILS_LOGGING_H_
#define MAKE_SPOOF_UTILS_LOGGING_H_
#include <stdarg.h>
enum LogLevel {
  LOG_INFO = 0,
  LOG_WARNING = 1,
  LOG_ERROR = 2,
  LOG_FATAL = 3,
  NUM_LEVELS = 4,
};
typedef __gnuc_va_list va_list;
const int kBufferSize = 1024;

namespace internal {
LogLevel get_log_level();
void set_log_level(LogLevel level);
void log_handler(LogLevel level, const char* filename, int lineno,
                 const char* fmt, ...);
}  // namespace internal
#define LIKELY(x) __builtin_expect((x), 1)
#define UNLIKELY(x) __builtin_expect((x), 0)
#define EVIMG_SET_LOG_LEVEL(level) ::internal::set_log_level(LOG_##level)

#define LOG(level, fmt, arg...)               \
  (LOG_##level < ::internal::get_log_level()) \
      ? (void)0                               \
      : ::internal::log_handler(LOG_##level, __FILE__, __LINE__, fmt, ##arg)

#define LOG_IF(condition, level, fmt, arg...) \
  !(condition)                                \
      ? (void)0                               \
      : ::internal::log_handler(LOG_##level, __FILE__, __LINE__, fmt, ##arg)

#define LOG_INFO(fmt, arg...) LOG(INFO, fmt, ##arg)
#define LOG_WARNING(fmt, arg...) LOG(WARNING, fmt, ##arg)
#define LOG_ERROR(fmt, arg...) LOG(ERROR, fmt, ##arg)
#define LOG_FATAL(fmt, arg...) LOG(FATAL, fmt, ##arg)

#define LOG_ASSERT(x)                     \
  do {                                    \
    if (UNLIKELY(!(x))) {                 \
      LOG_FATAL("ASSERT failed: %s", #x); \
    }                                     \
  } while (0)

#define CHECK(expression) \
  LOG_IF(!(expression), FATAL, "CHECK failed: ", #expression)

#define CHECK_EQ(a, b) CHECK((a) == (b))
#define CHECK_NE(a, b) CHECK((a) !(b))
#define CHECK_LT(a, b) CHECK((a) < (b))
#define CHECK_LE(a, b) CHECK((a) <= (b))
#define CHECK_GT(a, b) CHECK((a) > (b))
#define CHECK_GE(a, b) CHECK((a) >= (b))

#endif  // MAKE_SPOOF_UTILS_LOGGING_H_