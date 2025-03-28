#ifndef UTILS_H
#define UTILS_H

enum BOOL {
    FALSE = 0,
    TRUE = 1
};

#define MAX_INPUT 1024
#define MAX_CANDIDATES 512

unsigned inputCommand(char** input);
char* getPath(const char* command);
char* getLongestCommonPrefix(char** list, int count);
void listMatches(char** matches, int count);
int startsWith(const char* str, const char* prefix);
char* tabComplete(const char* input);

#endif