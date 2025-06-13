//
// Created by Nir Hai on 19/05/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

void remove_extra_spaces_to_temp(const char *src_filename) {
    char temp_filename[1024];
    snprintf(temp_filename, sizeof(temp_filename), "%s.t01", src_filename);

    FILE *src = fopen(src_filename, "r");
    if (!src) return;

    FILE *dest = fopen(temp_filename, "w");
    if (!dest) {
        fclose(src);
        return;
    }

    char line[82]; /* 80 chars + '\n' + '\0' */
    while (fgets(line, sizeof(line), src)) {
        char *ptr = line;
        while (isspace((unsigned char)*ptr)) ptr++;
        if (*ptr == ';') {
            fputs(line, dest);
            continue;
        }
        char cleaned[1024];
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
        if (j > 0 && cleaned[j-1] == ' ')
            j--;
        cleaned[j++] = '\n';
        cleaned[j] = '\0';
        fputs(cleaned, dest);
    }
    fclose(src);
    fclose(dest);
}

int main() {
    char filename[] = "test_input.as";

    if (validate_file(filename)) {
        printf("File validated successfully.\n");
        remove_extra_spaces_to_temp(filename);
        printf("Extra spaces removed. Output saved in %s.t01\n", filename);
    } else {
        printf("File validation failed.\n");
    }

    return 0;
}
