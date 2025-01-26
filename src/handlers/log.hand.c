#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include "log.hand.h"

#define LOG_FILE_PATH "../logs/app.log"  

// Function to get the current timestamp
const char* get_current_time() {
    static char buffer[20];
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return buffer;
}

// Log message function
void log_message(int level, const char *message, ...) {
    const char *level_str;

    // Determine log level
    switch (level) {
        case INFO:
            level_str = "INFO";
            break;
        case WARNING:
            level_str = "WARNING";
            break;
        case ERROR:
            level_str = "ERROR";
            break;
        default:
            level_str = "UNKNOWN";
            break;
    }

    // Format the message
    va_list args;
    va_start(args, message);
    char formatted_message[1024];
    vsnprintf(formatted_message, sizeof(formatted_message), message, args);
    va_end(args);

    // Prepare log entry with timestamp
    const char *current_time = get_current_time();
    FILE *logfile = fopen(LOG_FILE_PATH, "a");

    if (logfile) {
        // Write log to the file
        fprintf(logfile, "[%s] [%s] %s\n", current_time, level_str, formatted_message);
        fclose(logfile);
    } else {
        // If file can't be opened, print to console
        printf("[%s] [%s] %s\n", current_time, level_str, formatted_message);
    }
}

// Optional: Initialize log system (e.g., open files)
void initialize_log() {
    log_message(INFO, "Logging system initialized.");
}

// Optional: Cleanup log system (e.g., close files)
void cleanup_log() {
    log_message(INFO, "Logging system cleaned up.");
}
