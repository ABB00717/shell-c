#ifndef COMMAND_EXECUTER_H
#define COMMAND_EXECUTER_H

#include <stdio.h>

static const char *builtinCommands[] = {"echo", "type", "exit", NULL};

void executeCommand(char *tokens[], int tokenCount);

#endif