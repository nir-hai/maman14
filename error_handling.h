/**
* @file error_handling.h
* @brief Contains declarations for error reporting utilities and the global error message array.
*/

#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H
#include "pre_assembler_ds.h"

/**
* @struct Error
* @brief Represents an error found in source code.
* Here we define a struct of an error, which is represented by error number and a
* corresponding message
 */
typedef struct Error {
    int error_num;     /* a number repreresting the error*/
    char *error_message;  /* a message corresponding to the error number */
} Error;


/* 'errors' is a list of error that is defined in error_handling.c */
extern Error errors[];

/**
 * @brief Prints a predefined error message based on the error number.
 *
 * @param error_num The numeric error code (must exist in errors[]).
 * @param file_name The name of the file where the error occurred.
 * @param line_num The line number where the error occurred.
 */
void print_error_by_code(int error_num, const char *file_name, int line_num);
void remove_extra_spaces_to_temp(const char *src_filename);
int is_macro_name(const char *word);
Macro *get_macro(const char *name);
void insert_macro(const char *name, MacroLine *lines_head);
void write_modified_source(const char *filename, char **lines, int line_count);
void free_macro_table(void);

#endif //ERROR_HANDLING_H
