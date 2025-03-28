#include "../include/executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void executeCommand(char* tokens[], int tokensCount) {
    if (tokens == NULL || tokens[0] == NULL)
        return;

    char *command = tokens[0];
    char **args = NULL;

    if (tokensCount > 1) {
        args = malloc(sizeof(char*) * tokensCount);
        for (int i = 1; i < tokensCount; i++) {
            args[i-1] = tokens[i];
        }
        args[tokensCount-1] = NULL;
    }

    if (strcmp(command, "exit") == 0) {
        handleExit((const char**)args);
    } else if (strcmp(command, "echo") == 0) {
        handleEcho((const char**)args);
    } else if (strcmp(command, "type") == 0) {
        handleType((const char**)args);
    } else if (strcmp(command, "pwd") == 0) {
        handlePwd();
    } else if (strcmp(command, "cd") == 0) {
        handleCd((const char**)args, tokensCount-1);
    } else {
        char *path = getPath(command);
        if (path == NULL) {
            commandNotFound(command);
        }else {
            handleExternalProgram(command, args, tokensCount-1);
        }
    }
}