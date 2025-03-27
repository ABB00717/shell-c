#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    // Flush after every printf
    setbuf(stdout, NULL);

    // Wait for user input
    char input[100];
    printf("$ ");
    while (fgets(input, 100, stdin) != 0) {
        input[strlen(input) - 1] = '\0';
        if (strcmp(input, "exit 0") == 0)
            break;

        printf("%s: command not found\n", input);
        printf("$ ");
    }

    return 0;
}
