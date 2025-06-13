//
// Created by Nir Hai on 19/05/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pre_assembler.h"
#include "error_handling.h"

int validate_file(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        print_error_by_code(1, filename, 0);  /* File not found */
        return 0;
    }
    fclose(fp);
    return 1;
}

void remove_extra_spaces (char *filename)
{
  FILE *fp_in, *fp_out;
  char post_rm_space[FILENAME_MAX];
  char line[MAX_LINE_LENGTH];
  int i, j, in_word;

  /* Create a new file named: filename.t01 */
  strcpy(post_rm_space, filename);
  strcat(post_rm_space, ".t01");

  fp_in = fopen(filename, "r"); /* the file was validated*/
  fp_out = fopen(post_rm_space, "w");
  if (fp_out == NULL) {
    print_error_by_code(7, post_rm_space, 0);
    fclose(fp_in);
    return;
  }
 while (fgets(line, MAX_LINE_LENGTH, fp_in)) {
    if (line[0] == ';') {
        fputc('\n', fp_out);
        continue;
    }

    i = 0;
    j = 0;

    /* Skip leading spaces/tabs */
    while (line[i] == ' ' || line[i] == '\t') i++;

    in_word = 0;

    while (line[i] != '\n' && line[i] != '\0') {
        if (line[i] == ' ' || line[i] == '\t') {
            if (in_word) {
                line[j++] = ' ';
                in_word = 0;
            }
        } else {
            line[j++] = line[i];
            in_word = 1;
        }
        i++;
    }

    /* Remove trailing space if needed */
    if (j > 0 && line[j - 1] == ' ')
        j--;

    line[j++] = '\n';
    line[j] = '\0';

    fputs(line, fp_out);
}
fclose(fp_in);
fclose(fp_out);
}

int main() {
    char filename[] = "test_input.as";

    if (validate_file(filename)) {
        printf("File validated successfully.\n");
        remove_extra_spaces(filename);
        printf("Extra spaces removed. Output saved in %s.t01\n", filename);
    } else {
        printf("File validation failed.\n");
    }

    return 0;
}
