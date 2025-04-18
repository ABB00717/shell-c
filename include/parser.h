#ifndef PARSER_H
#define PARSER_H

#include <unistd.h>
#include "utils.h"

typedef struct {
    int argsSize;
    char** args;
    char* outputFile;

    int outputFd;
    bool appendMode;
} CommandNode;

char* appendChar(char* token, int* size, char c);
void parseInput(char *input, char ***tokens, int *tokenCount);
void parseCommand(CommandNode*** commands, char*** tokens, int tokenCount);

#endif