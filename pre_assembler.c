//
// Created by Nir Hai on 19/05/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pre_assembler_ds.h" 
#include "pre_assembler.h"

Macro *macro_table = NULL; // Head of the macro table linked list

/**
 * Removes extra spaces from each line of the source file (except comment lines)
 * and writes the cleaned lines to a temporary file with ".t1" appended.
 */
void remove_extra_spaces_to_temp(const char *src_filename) {
    char temp_filename[256];
    snprintf(temp_filename, sizeof(temp_filename), "%s.t1", src_filename);

    FILE *src = fopen(src_filename, "r");
    if (!src) return;

    FILE *dest = fopen(temp_filename, "w");
    if (!dest) {
        fclose(src);
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), src)) {
        char *ptr = line;
        // Skip leading spaces
        while (isspace((unsigned char)*ptr)) ptr++;
        // If the line is a comment, write as is
        if (*ptr == ';') {
            fputs(line, dest);
            continue;
        }
        // Remove extra spaces between words
        char cleaned[MAX_LINE_LENGTH];
        int i = 0, j = 0, in_space = 0;
        while (isspace((unsigned char)line[i])) i++;
        for (; line[i] != '\0' && line[i] != '\n'; i++) {
            if (isspace((unsigned char)line[i])) {
                if (!in_space) {
                    cleaned[j++] = ' ';
                    in_space = 1;
                }
            } else {
                cleaned[j++] = line[i];
                in_space = 0;
            }
        }
        // Remove trailing space
        if (j > 0 && cleaned[j-1] == ' ')
            j--;
        cleaned[j++] = '\n';
        cleaned[j] = '\0';
        fputs(cleaned, dest);
    }
    fclose(src);
    fclose(dest);
}

/**
 * Checks if the given word is a macro name in the macro table.
 * Returns 1 if found, 0 otherwise.
 */
int is_macro_name(const char *word) {
    Macro *curr = macro_table;
    while (curr) {
        if (strcmp(curr->name, word) == 0)
            return 1;
        curr = curr->next;
    }
    return 0;
}

/**
 * Returns a pointer to the macro struct with the given name, or NULL if not found.
 */
Macro *get_macro(const char *name) {
    Macro *curr = macro_table;
    while (curr) {
        if (strcmp(curr->name, name) == 0)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

/**
 * Inserts a new macro with the given name and lines into the macro table.
 */
void insert_macro(const char *name, MacroLine *lines_head) {
    Macro *new_macro = (Macro *)malloc(sizeof(Macro));
    strncpy(new_macro->name, name, MAX_LINE_LENGTH);
    new_macro->lines_head = lines_head;
    new_macro->lines_tail = NULL;
    new_macro->next = NULL;

    // Find the tail of the macro lines
    MacroLine *curr = lines_head;
    while (curr && curr->next) curr = curr->next;
    new_macro->lines_tail = curr;

    // Insert the new macro at the head of the macro table
    new_macro->next = macro_table;
    macro_table = new_macro;
}

/**
 * Writes the processed output lines to the given filename.
 */
void write_modified_source(const char *filename, char **lines, int line_count) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    for (int i = 0; i < line_count; i++) {
        fputs(lines[i], f);
    }
    fclose(f);
}

/**
 * Frees all memory used by the macro table.
 */
void free_macro_table(void) {
    Macro *curr = macro_table;
    while (curr) {
        MacroLine *line = curr->lines_head;
        while (line) {
            MacroLine *tmp = line;
            line = line->next;
            free(tmp->text);
            free(tmp);
        }
        Macro *tmpm = curr;
        curr = curr->next;
        free(tmpm);
    }
}

/**
 * Main function: orchestrates the pre-assembler process.
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    char temp_filename[256];
    snprintf(temp_filename, sizeof(temp_filename), "%s.t1", argv[1]);

    // Step 1: Remove extra spaces and write to temp file
    remove_extra_spaces_to_temp(argv[1]);

    // Step 2: Open the cleaned temp file for reading
    FILE *src = fopen(temp_filename, "r");
    if (!src) {
        printf("Error opening temp file.\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    char first_word[MAX_LINE_LENGTH];
    int is_macro = 0; // Flag: are we inside a macro definition?
    char macro_name[MAX_LINE_LENGTH];
    MacroLine *macro_lines_head = NULL, *macro_lines_tail = NULL;
    char *output_lines[10000]; // Stores all output lines
    int output_line_count = 0;

    // Step 3: Process each line in the cleaned file
    while (fgets(line, sizeof(line), src)) {
        // Extract the first word from the line
        int i = 0, j = 0;
        while (isspace((unsigned char)line[i])) i++;
        while (line[i] && !isspace((unsigned char)line[i]) && line[i] != '\n')
            first_word[j++] = line[i++];
        first_word[j] = '\0';

        // Step 4: If the first word is a macro name, expand it
        if (!is_macro && is_macro_name(first_word)) {
            Macro *macro = get_macro(first_word);
            if (macro) {
                MacroLine *ml = macro->lines_head;
                while (ml) {
                    output_lines[output_line_count++] = strdup(ml->text);
                    ml = ml->next;
                }
            }
            continue; // Go to next line
        }

        // Step 5: If the first word is "mcro", start macro definition
        if (!is_macro && strcmp(first_word, "mcro") == 0) {
            is_macro = 1;
            
            // Skip whitespace after "mcro"
            while (isspace((unsigned char)line[i])) i++;
            
            // Extract macro name
            j = 0;
            while (line[i] && !isspace((unsigned char)line[i]) && line[i] != '\n')
                macro_name[j++] = line[i++];
            macro_name[j] = '\0';
            
            // Check if there's anything else on the line after the macro name
            while (isspace((unsigned char)line[i])) i++;
            if (line[i] != '\0' && line[i] != '\n' && line[i] != '\r') {
                // Error: Extra characters after macro name
                printf("Error: Extra characters after macro name on line: %s", line);
                // Call your error handling function here
                // report_error(line_num, INVALID_MACRO_DEF);
                return 1;
            }
            
            macro_lines_head = macro_lines_tail = NULL;
            continue; // Go to next line
        }

        // Step 6: If inside macro definition, collect lines until "mcroend"
        if (is_macro) {
            if (strcmp(first_word, "mcroend") == 0) {
                // Check if there's anything after "mcroend"
                while (isspace((unsigned char)line[i])) i++;
                if (line[i] != '\0' && line[i] != '\n' && line[i] != '\r') {
                    // Error: Extra characters after mcroend
                    printf("Error: Extra characters after mcroend on line: %s", line);
                    // Call your error handling function here
                    // report_error(line_num, INVALID_MACRO_END);
                    return 1;
                }
                
                insert_macro(macro_name, macro_lines_head); // Save macro
                is_macro = 0; // End macro definition
                macro_lines_head = macro_lines_tail = NULL;
                continue; // Go to next line
            } else {
                // Add line to macro's lines
                MacroLine *new_line = (MacroLine *)malloc(sizeof(MacroLine));
                new_line->text = strdup(line);
                new_line->next = NULL;
                if (!macro_lines_head)
                    macro_lines_head = macro_lines_tail = new_line;
                else {
                    macro_lines_tail->next = new_line;
                    macro_lines_tail = new_line;
                }
                continue; // Go to next line
            }
        }

        // Step 7: Normal line, add to output
        output_lines[output_line_count++] = strdup(line);
    }

    fclose(src);

    // Step 8: Write the final output to the temp file (overwriting it)
    write_modified_source(temp_filename, output_lines, output_line_count);

    // Step 9: Free all allocated memory
    for (int i = 0; i < output_line_count; i++) {
        free(output_lines[i]);
    }
    free_macro_table();

    return 0;
}
