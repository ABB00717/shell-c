#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/dispatcher.h"
#include "../include/parser.h"
#include "../include/utils.h"

int main(int argc, char* argv[]) {
    // Flush after every printf
    setbuf(stdout, NULL);

    // Wait for user input
    char** tokens;
    CommandNode** commands;
    char* input = NULL;
    int tokenCount = 0;
    while (inputCommand(&input) == 0) {
        tokenCount = 0;
        parseInput(input, &tokens, &tokenCount);
        parseCommand(&commands, &tokens, tokenCount);

        // executeCommand(tokens, tokenCount);
        dispatchCommands(commands);
        // Free all memories
        free(input);
        for (int i = 0; i < tokenCount; i++) {
            free(tokens[i]);
        }
        for (int i = 0; commands[i] != NULL; i++) {
            if (commands[i]->args != NULL)
                free(commands[i]->args);
            free(commands[i]);
        }
        free(commands);
    }

    return 0;
}
