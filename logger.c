#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "logger.h"

static FILE *logFile = NULL;
static int loggingEnabled = 0;
static const char *logLevelStrings[] = {
    [LOG_DEBUG] = "DEBUG",
    [LOG_INFO] = "INFO",
    [LOG_WARNING] = "WARNING",
    [LOG_ERROR] = "ERROR"
};

// private function to be used inside logger.c
void LogMessage(LogLevel level, const char *message);

void LogInitialize()
{
    FILE *f = fopen(LOG_FILE, "a");

    // This is basically to log logging error once
    // and not every time something is logged
    if (f == NULL) {
        // maybe prompt the user if he wishes to exit or continue without logging?
        perror("Failed to open log file");
        loggingEnabled = 0;
    }
    else {
        fclose(f);
        loggingEnabled = 1;
    }
}

void LogMessage(LogLevel level, const char *message)
{
    if (!loggingEnabled) {
        return;
    }
    if (level < LOG_DEBUG || level > LOG_ERROR) {
        fprintf(stderr, "Invalid log level: %d\n", level);
        return;
    }
    logFile = fopen(LOG_FILE, "a");
    if(logFile == NULL) {
        perror("Failed to open log file");
        return;
    }

    time_t currentTime;
    time(&currentTime);
    char *timeBuf[64];
    strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));

    fprintf(logFile, "[%s] %s: %s\n", timeBuf, logLevelStrings[level], message);
    fclose(logFile);
    logFile = NULL;
}

void LogDebug(const char *message)
{
    LogMessage(LOG_DEBUG, message);
}

void LogInfo(const char *message)
{
    LogMessage(LOG_INFO, message);
}

void LogWarning(const char *message)
{
    LogMessage(LOG_WARNING, message);
}

void LogError(const char *message)
{
    LogMessage(LOG_ERROR, message);
}