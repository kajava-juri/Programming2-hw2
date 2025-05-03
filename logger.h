#ifndef LOGGER_H
#define LOGGER_H

#define LOG_FILE "log.txt"

typedef enum {
    LOG_DEBUG,   // Most verbose level
    LOG_INFO,    // Default level
    LOG_WARNING,
    LOG_ERROR    // Least verbose level
} LogLevel;

// private function to be used inside logger.c
// only wrappers should be used outside for better readability

// void LogMessage(LogLevel level, const char *message);


void LogMessage(LogLevel level, const char *message);

// Set the minimum log level that will be recorded
void LogSetLevel(LogLevel level);

// Get the current log level
LogLevel LogGetLevel();

void LogDebug(const char *message);
void LogInfo(const char *message);
void LogWarning(const char *message);
void LogError(const char *message);

#endif // LOGGET_H