/**
 * @file error_handling.c
 * @brief Implements error reporting functions and defines the global error array.
 *
 * This file defines how errors are printed and contains a list of predefined error messages.
 */
#include "error_handling.h"
#include <stdio.h>
/**
 * @brief Array of predefined error messages.
 *
 * Each entry maps an error number to a corresponding user-friendly message.
 */
Error errors[] = {
    {1, "File not found"},
    {2, "Line too long"},
    {3, "Undefined macro used"},
    {0, NULL} /* End of array marker */
};

/**
 * @brief Prints an error message with a custom message.
 */
void print_error(int error_num, const char *message, const char *file_name, int line_num) {
    printf("Error %d in file \"%s\", line %d: %s\n", error_num, file_name, line_num, message);
}

/**
 * @brief Searches for an error message by code and prints it with context.
 */
void print_error_by_code(int error_num, const char *file_name, int line_num) {
    int i;
    for (i = 0; errors[i].error_message != NULL; i++) {
        if (errors[i].error_num == error_num) {
            print_error(error_num, errors[i].error_message, file_name, line_num);
            return;
        }
    }
    /* Fallback if error number not found */
    print_error(error_num, "Unknown error", file_name, line_num);
}
