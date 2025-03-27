#include "../include/utils.h"
#include <stdio.h>
#include <string.h>

unsigned inputCommand(char **input) {
    size_t bufsize = 0;
    ssize_t linelen;
    
    printf("$ ");
    linelen = getline(input, &bufsize, stdin);
    (*input)[strcspn(*input, "\n")] = 0;

    return linelen != -1 ? 0 : 1;
}

void parseInput(char *input, char *tokens[], int *tokenCount) {    
    char *token = strtok(input, " ");
    
    while (token != NULL && *tokenCount < MAX_TOKEN_COUNT) {
        tokens[(*tokenCount)++] = strdup(token);
        token = strtok(NULL, " ");
    }
}