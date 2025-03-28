#include "../include/executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void executeCommand(char* tokens[], int tokenCount) {
    if (tokens == NULL || tokens[0] == NULL)
        return;

    char *command = tokens[0];
    char **args = NULL;

    if (tokenCount > 1) {
        args = malloc(sizeof(char*) * tokenCount);
        for (int i = 1; i < tokenCount; i++) {
            args[i-1] = tokens[i];
        }
        args[tokenCount-1] = NULL;
    }

    if (strcmp(command, "exit") == 0) {
        handleExit((const char**)args);
    } else if (strcmp(command, "echo") == 0) {
        handleEcho((const char**)args);
    } else if (strcmp(command, "type") == 0) {
        handleType((const char**)args);
    } else {
        commandNotFound(command);
    }
}