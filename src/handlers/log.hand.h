#ifndef LOG_HANDLER_H
#define LOG_HANDLER_H

// Log levels
#define INFO    1
#define WARNING 2
#define ERROR   3

// Function to log messages
void log_message(int level, const char *message, ...);

// Function to initialize the log (optional for setup)
void initialize_log();

// Function to clean up log system (optional if needed)
void cleanup_log();

#endif
