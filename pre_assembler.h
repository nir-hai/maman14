/*
// Created by Nir Hai on 19/05/2025.
*/

#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include "pre_assembler_ds.h"

void remove_extra_spaces_to_temp(const char *src_filename);
int is_macro_name(const char *word);
Macro *get_macro(const char *name);
void insert_macro(const char *name, MacroLine *lines_head);
void write_modified_source(const char *filename, char **lines, int line_count);
void free_macro_table(void);

#endif /*PRE_ASSEMBLER_H */
