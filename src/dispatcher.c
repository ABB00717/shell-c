#include "../include/dispatcher.h"
#include "../include/executor.h"
#include <unistd.h> // for STDOUT_FILENO
#include <fcntl.h>  // for open()
#include <stdio.h>

void dispatchCommands(CommandNode** commands) {
    if (commands == NULL)
        return;

    CommandNode* cmd = commands[0];
    int savedStdout = -1;
    int outputFd = -1;

    if (cmd->outputFile != NULL) {
        int flags = O_WRONLY | O_CREAT;
        if (cmd->appendMode) {
            flags |= O_APPEND;
        } else {
            flags |= O_TRUNC;
        }
        
        outputFd = open(cmd->outputFile, flags, 0644);
        if (outputFd < 0) {
            printf("open outputfile");
            return;
        }

        savedStdout = dup(cmd->outputFd);

        dup2(outputFd, cmd->outputFd);
        close(outputFd);
    }

    executeCommand(cmd->args, cmd->argsSize);

    if (savedStdout != -1) {
        dup2(savedStdout, cmd->outputFd);
        close(savedStdout);
    }
}