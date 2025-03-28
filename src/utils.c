#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned inputCommand(char **input) {
    size_t bufsize = 0;
    ssize_t linelen;
    
    printf("$ ");
    linelen = getline(input, &bufsize, stdin);
    (*input)[strcspn(*input, "\n")] = 0;

    return linelen != -1 ? 0 : 1;
}

void parseInput(char *input, char ***tokens, int *tokenCount) {    
    int capacity = 10;
    *tokens = malloc(sizeof(char*) * capacity);
    *tokenCount = 0;
    
    char *token = strtok(input, " ");
    while (token != NULL) {
        if (*tokenCount >= capacity) {
            capacity *= 2;
            *tokens = realloc(*tokens, sizeof(char*) * capacity);
        }
        
        (*tokens)[(*tokenCount)++] = strdup(token);
        token = strtok(NULL, " ");
    }
}