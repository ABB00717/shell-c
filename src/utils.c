#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

char* getPath(const char* command) {
    char* path_env = getenv("PATH");
    if (!path_env)
        return NULL;

    char* path_copy = strdup(path_env);
    char* token = strtok(path_copy, ":");
    while (token != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", token, command);

        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return strdup(full_path);
        }

        strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}