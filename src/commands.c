#include "../include/commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

const char* builtinCommands[] = {"echo", "type", "exit", NULL};

void commandNotFound(const char* command) {
    printf("%s: command not found\n", command);
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

        if (*cp != NULL)
            return;

        char* path = getPath(args[0]);
        if (path != NULL) {
            printf("%s is %s\n", args[0], path);
            free(path);
        } else {
            printf("%s not found\n", args[0]);
        }
    }
}

void handleExternalProgram(const char *command, char* const args[]) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    }
    
    if (pid == 0) {
        execvp(command, args);
        perror("execvp failed");
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}