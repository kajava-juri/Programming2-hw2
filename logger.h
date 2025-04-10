#ifndef LOGGER_H
#define LOGGER_H

#define LOG_FILE "log.txt"

typedef enum {
    LOG_INFO,
    LOG_ERROR,
    LOG_WARNING,
    LOG_DEBUG
} LogLevel;

// private function to be used inside logger.c
// only wrappers should be used outside for better readability

// void LogMessage(LogLevel level, const char *message);


void LogInitialize();

void LogDebug(const char *message);
void LogInfo(const char *message);
void LogWarning(const char *message);
void LogError(const char *message);

#endif // LOGGET_H