#include "../include/parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* appendChar(char* token, int* size, char c) {
    token = realloc(token, (*size) + 2);
    if (!token) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    token[(*size)++] = c;
    token[*size] = '\0';
    return token;
}

void parseInput(char* input, char*** tokens, int* tokenCount) {
    int capacity = 10;
    *tokens = malloc(sizeof(char*) * capacity);
    if (!*tokens) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    *tokenCount = 0;

    char* ip = input;
    char* currentToken = NULL;
    int currentTokenSize = 0;
    bool insideSingleQuotes = FALSE;
    bool insideDoubleQuotes = FALSE;
    bool escape = FALSE;

    while (*ip != '\0') {
        char c = *ip++;

        if (escape) {
            escape = FALSE;
            currentToken = appendChar(currentToken, &currentTokenSize, c);
            continue;
        }

        if (c == '\\') {
            if (insideSingleQuotes) {
                currentToken = appendChar(currentToken, &currentTokenSize, c);
            } else if (insideDoubleQuotes) {
                char next = *ip;
                if (next == '"' || next == '\\' || next == '$') {
                    escape = TRUE;
                } else {
                    currentToken = appendChar(currentToken, &currentTokenSize, c);
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
                if (*tokenCount >= capacity) {
                    capacity *= 2;
                    *tokens = realloc(*tokens, sizeof(char*) * capacity);
                    if (!*tokens) {
                        perror("realloc");
                        exit(EXIT_FAILURE);
                    }
                }
                (*tokens)[(*tokenCount)++] = strdup(currentToken);
                free(currentToken);
                currentToken = NULL;
                currentTokenSize = 0;
            }
        } else {
            currentToken = appendChar(currentToken, &currentTokenSize, c);
        }
    }

    // last token
    if (currentTokenSize != 0) {
        if (*tokenCount >= capacity) {
            capacity *= 2;
            *tokens = realloc(*tokens, sizeof(char*) * capacity);
            if (!*tokens) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }
        (*tokens)[(*tokenCount)++] = strdup(currentToken);
        free(currentToken);
    }
}

void parseCommand(CommandNode*** commands, char*** tokens, int tokenCount) {
    CommandNode* current = malloc(sizeof(CommandNode));
    current->args = NULL;
    current->outputFile = NULL;
    current->outputFd = STDOUT_FILENO;
    current->appendMode = FALSE;
    current->argsSize = 0;

    for (size_t i = 0; i < tokenCount; i++) {
        char* tok = (*tokens)[i];

        if (strcmp(tok, "|") == 0) {

        } else if (strcmp(tok, ">") == 0 || strcmp(tok, "1>") == 0) {
            if (i + 1 < tokenCount) {
                current->outputFile = (*tokens)[++i];
                current->outputFd = STDOUT_FILENO;
                current->appendMode = false;
            }
        } else if (strcmp(tok, "2>") == 0) {
            if (i + 1 < tokenCount) {
                current->outputFile = (*tokens)[++i];
                current->outputFd = STDERR_FILENO;
                current->appendMode = false;
            }
        } else if (strcmp(tok, ">>") == 0 || strcmp(tok, "1>>") == 0) {
            if (i + 1 < tokenCount) {
                current->outputFile = (*tokens)[++i];
                current->outputFd = STDOUT_FILENO;
                current->appendMode = true;
            }
        } else if (strcmp(tok, "2>>") == 0) {
            if (i + 1 < tokenCount) {
                current->outputFile = (*tokens)[++i];
                current->outputFd = STDERR_FILENO;
                current->appendMode = true;
            }
        } else if (strcmp(tok, "<") == 0) {

        } else {
            current->args = realloc(current->args, (current->argsSize + 1) * sizeof(char*));
            current->args[current->argsSize++] = tok;
        }
    }

    *commands = malloc(sizeof(CommandNode*));
    (*commands)[0] = current;
}