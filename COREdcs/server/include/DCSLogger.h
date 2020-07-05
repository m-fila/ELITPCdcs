#if !defined(DCS_LOG_H)
#define DCS_LOG_H

#include <cstdarg>
#include <cstdio>
#include <mutex>
#include <open62541/plugin/log.h>
#include <open62541/types.h>
#include <string>

class DCSLogger {
public:
  static UA_Logger *getLogger() { return &getInstance().logger; }
  static void setFile(std::string logFile);

  DCSLogger(DCSLogger const &) = delete;
  void operator=(DCSLogger const &) = delete;

private:
  static DCSLogger &getInstance() {
    static DCSLogger instance;
    return instance;
  }

  DCSLogger() : logger({log, &context, nullptr}) {}

  static void log(void *context, UA_LogLevel level, UA_LogCategory category,
                  const char *msg, va_list args);

  UA_Logger logger;
  struct logContext {
    std::string file;
    std::mutex mutex;
    const char *const logLevelNames[6] = {"trace", "debug", "info",
                                          "warn",  "error", "fatal"};
    const char *const logCategoryNames[7] = {
        "network", "channel",  "session",       "server",
        "client",  "userland", "securitypolicy"};
    const char *const logLeveLColors[6] = {"",         "",         "\x1b[32m",
                                           "\x1b[33m", "\x1b[31m", "\x1b[35m"};
    const char *const logLevelResetColor = "\x1b[0m";
  } context;
};

#endif // DCS_LOG_H
