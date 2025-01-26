#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Function to check if the ISO file exists and is accessible
int iso_exists(const char *iso_path) {
    struct stat buffer;
    return (stat(iso_path, &buffer) == 0);
}

// Function to validate ISO file (check if it's a valid file and exists)
int validate_iso(const char *iso_path) {
    if (iso_path == NULL || strlen(iso_path) == 0) {
        fprintf(stderr, "ISO path is invalid.\n");
        return 0;
    }

    // Check if the ISO exists and is accessible
    if (!iso_exists(iso_path)) {
        fprintf(stderr, "ISO file not found at path: %s\n", iso_path);
        return 0;
    }

    // Here you can add more specific validation if required (e.g., check file extension)
    if (strstr(iso_path, ".iso") == NULL) {
        fprintf(stderr, "Invalid ISO file format: %s\n", iso_path);
        return 0;
    }

    return 1; // ISO is valid and accessible
}

