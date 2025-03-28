#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/utils.h"
#include "../include/executor.h"

int main(int argc, char* argv[]) {
    // Flush after every printf
    setbuf(stdout, NULL);

    // Wait for user input
    char** tokens;
    char* input = NULL;
    int tokenCount = 0;
    while (inputCommand(&input) == 0) {
        tokenCount = 0;
        parseInput(input, &tokens, &tokenCount);
        
        executeCommand(tokens, tokenCount);
    }

    // Free all memories
    free(input);
    for (int i = 0; i < tokenCount; i++) {
        free(tokens[i]);
    }
    return 0;
}
