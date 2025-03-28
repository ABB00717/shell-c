#ifndef COMMANDS_H
#define COMMANDS_H

extern const char* builtinCommands[];

void commandNotFound(const char* command);
void handleEcho(const char* args[]);
void handleExit(const char* args[]);
void handleType(const char* args[]);

#endif