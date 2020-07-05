#include "DCSLogger.h"

void DCSLogger::log(void *context, UA_LogLevel level, UA_LogCategory category,
                    const char *msg, va_list args) {
  auto logC = static_cast<logContext *>(context);
  char date[37];
  {
    UA_Int64 tOffset = UA_DateTime_localTimeUtcOffset();
    UA_DateTimeStruct dts = UA_DateTime_toStruct(UA_DateTime_now() + tOffset);

    sprintf(date, "[%04u-%02u-%02u %02u:%02u:%02u.%03u (UTC%+05d)]", dts.year,
            dts.month, dts.day, dts.hour, dts.min, dts.sec, dts.milliSec,
            (int)(tOffset / UA_DATETIME_SEC / 36));
  }
  const std::lock_guard<std::mutex> lock(logC->mutex);
  FILE *file = nullptr;
  file = fopen(logC->file.c_str(), "a");
  if (file != nullptr) {
    std::va_list args2;
    va_copy(args2, args);
    fprintf(file, "%s %s/%s\t", date, logC->logLevelNames[level],
            logC->logCategoryNames[category]);
    vfprintf(file, msg, args2);
    fprintf(file, "\n");
    fflush(file);
    fclose(file);
    va_end(args2);
  } else {
    printf("%s%s %s/%s%s\t", date, logC->logLeveLColors[UA_LOGLEVEL_ERROR],
           logC->logLevelNames[UA_LOGLEVEL_ERROR],
           logC->logCategoryNames[UA_LOGCATEGORY_SERVER],
           logC->logLevelResetColor);
    printf("Can't open log file: %s\n", logC->file.c_str());
    fflush(stdout);
  }
  printf("%s%s %s/%s%s\t", date, logC->logLeveLColors[level],
         logC->logLevelNames[level], logC->logCategoryNames[category],
         logC->logLevelResetColor);
  vprintf(msg, args);
  printf("\n");
  fflush(stdout);
}

void DCSLogger::setFile(std::string logFile) {
    auto &c = getInstance().context;
    const std::lock_guard<std::mutex> lock(c.mutex);
    c.file = logFile;
  };
