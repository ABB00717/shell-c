#include "../include/parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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