#include "../include/utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum BOOL {
    FALSE = 0,
    TRUE = 1
};

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

    char c;
    char *ip = input;
    char *currentToken = NULL;
    int currentTokenSize = 0;
    bool insideSingleQuotes = FALSE;
    bool insideDoubleQuotes = FALSE;
    bool escape = FALSE;

    while (*ip != 0) {
        c = *ip++;

        if (escape) {
            escape = FALSE;
            currentToken = realloc(currentToken, currentTokenSize + 2);
            currentToken[currentTokenSize++] = c;
            currentToken[currentTokenSize] = '\0';
            continue;
        }

        if (c == '\\') {
            if (insideSingleQuotes) {
                currentToken = realloc(currentToken, currentTokenSize + 2);
                currentToken[currentTokenSize++] = c;
                currentToken[currentTokenSize] = '\0';
            } else if (insideDoubleQuotes) {
                char next = *ip;
                if (next == '"' || next == '\\' || next == '$') {
                    escape = TRUE;
                } else {
                    currentToken = realloc(currentToken, currentTokenSize + 2);
                    currentToken[currentTokenSize++] = c;
                    currentToken[currentTokenSize] = '\0';
                }
            } else {
                escape = TRUE;
            }
        } else if (c == '\'' && !insideDoubleQuotes) {
            insideSingleQuotes = !insideSingleQuotes;
        } else if (c == '"' && !insideSingleQuotes) {
            insideDoubleQuotes = !insideDoubleQuotes;
        } else if (isspace(c) && !insideSingleQuotes && !insideDoubleQuotes) {
            if (currentTokenSize != 0) {
                currentToken = realloc(currentToken, currentTokenSize + 1);
                currentToken[currentTokenSize] = '\0';

                if (*tokenCount >= capacity) {
                    capacity *= 2;
                    *tokens = realloc(*tokens, sizeof(char*) * capacity);
                }

                (*tokens)[(*tokenCount)++] = strdup(currentToken);
                free(currentToken);
                currentToken = NULL;
                currentTokenSize = 0;
            }
        } else {
            currentToken = realloc(currentToken, currentTokenSize + 2);
            currentToken[currentTokenSize++] = c;
            currentToken[currentTokenSize] = '\0';
        }
    }

    // last token
    if (currentTokenSize != 0) {
        currentToken = realloc(currentToken, currentTokenSize + 1);
        currentToken[currentTokenSize] = '\0';
        if (*tokenCount >= capacity) {
            capacity *= 2;
            *tokens = realloc(*tokens, sizeof(char*) * capacity);
        }
        (*tokens)[(*tokenCount)++] = strdup(currentToken);
        free(currentToken);
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

        token = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}