#ifndef PRE_ASSEMBLER_DS_H
#define PRE_ASSEMBLER_DS_H

#define MAX_LINE_LENGTH 82

typedef struct MacroLine {
    char *text;
    struct MacroLine *next;
} MacroLine;

typedef struct Macro {
    char name[MAX_LINE_LENGTH];
    MacroLine *lines_head;
    MacroLine *lines_tail;
    struct Macro *next;
} Macro;

#endif