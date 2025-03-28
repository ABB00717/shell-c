#ifndef UTILS_H
#define UTILS_H

enum BOOL {
    FALSE = 0,
    TRUE = 1
};

unsigned inputCommand(char** input);
char* getPath(const char* command);

#endif