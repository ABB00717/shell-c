#include "../include/command_executer.h"
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
        if (args && args[0] && strcmp(args[0], "0") == 0) {
            free(args);
            exit(0);
        }
    } else if (strcmp(command, "echo") == 0) {
        while (*args != NULL) {
            printf("%s ", *args++);
        }
        printf("\n");
    } else if (strcmp(command, "type") == 0 && args && args[0]) {
        const char **cp = builtinCommands;
        for (; *cp != NULL; cp++) {
            if (strcmp(*cp, args[0]) == 0) {
                printf("%s is a shell builtin\n", *cp);\
                break;
            }
        }

        if (*cp == NULL)
            printf("%s: command not found\n", args[0]);
    } else {
        printf("%s: command not found\n", command);
    }
}