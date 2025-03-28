#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

extern const char* builtinCommands[];

char* getLongestCommonPrefix(char** list, int count);
void listMatches(char** matches, int count);
int startsWith(const char* str, const char* prefix);

unsigned inputCommand(char** input) {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    static int multiTabCount = 0;
    static char lastPrefix[MAX_INPUT] = {0};
    char buffer[MAX_INPUT] = {0};
    int len = 0;

    printf("$ ");
    fflush(stdout);

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == '\n') {
            buffer[len] = '\0';
            printf("\n");
            break;
        } else if (c == '\t') {
            char* completed = tabComplete(buffer);
            if (completed) {
                int complen = strlen(completed);
                if (complen > len) {
                    printf("%s", completed + len);
                    fflush(stdout);
                }
                strcpy(buffer, completed);
                len = complen;
                free(completed);
            }
        } else if (c == 127 || c == '\b') {
            if (len > 0) {
                len--;
                buffer[len] = '\0';
                printf("\b \b");
                fflush(stdout);
            }
        } else {
            if (len < MAX_INPUT - 1) {
                buffer[len++] = c;
                buffer[len] = '\0';
                printf("%c", c);
                fflush(stdout);
            }
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    *input = strdup(buffer);
    return len > 0 ? 0 : 1;
}

char* tabComplete(const char* input) {
    static int multiTabCount = 0;
    static char lastPrefix[MAX_INPUT] = {0};
    char* candidates[MAX_CANDIDATES];
    int count = 0;

    // Builtins
    for (int i = 0; builtinCommands[i]; ++i) {
        if (startsWith(builtinCommands[i], input)) {
            candidates[count++] = strdup(builtinCommands[i]);
        }
    }

    // PATH executables
    char* path = getenv("PATH");
    char* pathCopy = strdup(path);
    char* dir = strtok(pathCopy, ":");

    while (dir && count < MAX_CANDIDATES) {
        DIR* dp = opendir(dir);
        if (dp) {
            struct dirent* entry;
            while ((entry = readdir(dp)) && count < MAX_CANDIDATES) {
                if (startsWith(entry->d_name, input)) {
                    candidates[count++] = strdup(entry->d_name);
                }
            }
            closedir(dp);
        }
        dir = strtok(NULL, ":");
    }

    free(pathCopy);

    if (count == 0) {
        printf("\a");
        multiTabCount = 0;
        lastPrefix[0] = '\0';
        return NULL;
    }

    if (count == 1) {
        multiTabCount = 0;
        lastPrefix[0] = '\0';
        char* result = malloc(strlen(candidates[0]) + 2);
        sprintf(result, "%s ", candidates[0]);
        for (int i = 0; i < count; ++i) free(candidates[i]);
        return result;
    }

    char* common = getLongestCommonPrefix(candidates, count);
    if (strlen(common) > strlen(input)) {
        multiTabCount = 0;
        lastPrefix[0] = '\0';
        for (int i = 0; i < count; ++i) free(candidates[i]);
        return common;
    }

    if (strcmp(lastPrefix, input) != 0) {
        strcpy(lastPrefix, input);
        multiTabCount = 1;
        printf("\a");
    } else {
        multiTabCount++;
    }

    if (multiTabCount == 2) {
        printf("\n");
        listMatches(candidates, count);
        printf("\n$ %s", input);
        fflush(stdout);
        multiTabCount = 0;
    }

    for (int i = 0; i < count; ++i) free(candidates[i]);
    return strdup(input);
}

int startsWith(const char* str, const char* prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

void listMatches(char** matches, int count) {
    for (int i = 0; i < count; ++i) {
        printf("%s  ", matches[i]);
    }
}

char* getLongestCommonPrefix(char** list, int count) {
    if (count == 0) return strdup("");

    int i = 0;
    while (1) {
        char c = list[0][i];
        if (c == '\0') break;

        for (int j = 1; j < count; ++j) {
            if (list[j][i] != c) {
                goto end;
            }
        }
        i++;
    }

end:
    char* prefix = malloc(i + 1);
    strncpy(prefix, list[0], i);
    prefix[i] = '\0';
    return prefix;
}

char* getPath(const char* command) {
    char* path_env = getenv("PATH");
    if (!path_env)
        return NULL;

    char* path_copy = strdup(path_env);
    char* token = strtok(path_copy, ":");
    while (token != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", token, command);

        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return strdup(full_path);
        }

        token = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}