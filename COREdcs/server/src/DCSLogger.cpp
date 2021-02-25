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
    if(!logC->file.empty() && level >= logC->logLevelFile) {
        FILE *file = nullptr;
        file = fopen(logC->file.c_str(), "a");
        if(file != nullptr) {
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
    }
    if(level >= logC->logLevelTTY) {
        printf("%s%s %s/%s%s\t", date, logC->logLeveLColors[level],
               logC->logLevelNames[level], logC->logCategoryNames[category],
               logC->logLevelResetColor);
        vprintf(msg, args);
        printf("\n");
        fflush(stdout);
    }
}

void DCSLogger::setFile(const std::string &logFile) {
    auto &c = getInstance().context;
    const std::lock_guard<std::mutex> lock(c.mutex);
    c.file = logFile;
};

void DCSLogger::setLogLevel(const std::string &level, DCSLogger::Output output) {
    if(output == Output::all) {
        for(int i = 0; i < Output::all; ++i)
            setLogLevel(level, static_cast<DCSLogger::Output>(i));
        return;
    }
    auto &c = getInstance().context;
    const std::lock_guard<std::mutex> lock(c.mutex);
    UA_LogLevel *outputLogLevel = nullptr;
    switch(output) {
    case Output::tty:
        outputLogLevel = &c.logLevelTTY;
        break;
    case Output::file:
        outputLogLevel = &c.logLevelFile;
        break;
    default:
        return;
        break;
    }
    if(level == "trace") {
        *outputLogLevel = UA_LOGLEVEL_TRACE;
    } else if(level == "debug") {
        *outputLogLevel = UA_LOGLEVEL_DEBUG;
    } else if(level == "info") {
        *outputLogLevel = UA_LOGLEVEL_INFO;
    } else if(level == "warn") {
        *outputLogLevel = UA_LOGLEVEL_WARNING;
    } else if(level == "error") {
        *outputLogLevel = UA_LOGLEVEL_ERROR;
    } else if(level == "fatal") {
        *outputLogLevel = UA_LOGLEVEL_FATAL;
    }
};
