#ifndef COMMANDS_H
#define COMMANDS_H

#include "utils.h"

extern const char* builtinCommands[];

void commandNotFound(const char* command);
void handleEcho(const char* args[]);
void handleExit(const char* args[]);
void handleType(const char* args[]);
void handleExternalProgram(const char* command, const char* args[]);
void handlePwd();
void handleCd(const char* args[]);

#endif