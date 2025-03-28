#include "../include/commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* builtinCommands[] = {"echo", "type", "exit", NULL};

void commandNotFound(const char* command) {
    printf("%s: command not found", command);
}

void handleExit(const char** args) {
    if (args) {
        free(args);
        exit(0);
    }
}

void handleEcho(const char** args) {
    while (*args != NULL) {
        printf("%s ", *args++);
    }
    printf("\n");
}

void handleType(const char** args) {
    for (; *args != NULL; args++) {
        const char** cp = builtinCommands;
        for (; *cp != NULL; cp++) {
            if (strcmp(*cp, args[0]) == 0) {
                printf("%s is a shell builtin\n", *cp);
                break;
            }
        }

        if (*cp == NULL)
            printf("%s: not found\n", args[0]);
    }
}