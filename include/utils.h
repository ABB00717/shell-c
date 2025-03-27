#ifndef UTILS_H
#define UTILS_H

#define MAX_TOKEN_COUNT 100

unsigned inputCommand(char **input);
void parseInput(char *input, char *tokens[], int *tokenCount);

#endif