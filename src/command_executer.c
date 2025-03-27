#include "../include/command_executer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void executeCommand(char* tokens[]) {
    if (tokens == NULL || tokens[0] == NULL)
        return;

    char *command = tokens[0];
    char **args = NULL;

    if (tokens[1] != NULL)
        args = &tokens[1];
        
    if (strcmp(command, "exit") == 0) {
        if (args && args[0] && strcmp(args[0], "0") == 0)
            exit(0);
    } else {
        printf("%s: command not found\n", command);
    }
}