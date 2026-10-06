#include "process.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void format_command(const Command *command, char *buffer, size_t size) {
    int i;
    size_t used = 0;
    buffer[0] = '\0';
    for (i = 0; i < command->argc; i++) {
        int written = snprintf(buffer + used, size - used, "%s%s", i == 0 ? "" : " ", command->argv[i]);
        if (written < 0 || (size_t)written >= size - used) {
            return;
        }
        used += (size_t)written;
    }
}

void execute_program(const Command *command, ProcessHistory *history) {
    pid_t child_pid;
    int status;
    char description[HISTORY_COMMAND_LENGTH];

    format_command(command, description, sizeof(description));
    child_pid = fork();
    if (child_pid < 0) {
        perror("fork");
        return;
    }
    if (child_pid == 0) {
        print_child_info(getpid(), getppid(), description);
        execvp(command->argv[0], command->argv);
        fprintf(stderr, "exec failed for '%s': %s\n", command->argv[0], strerror(errno));
        _exit(127);
    }

    print_parent_info(getpid(), child_pid);
    if (waitpid(child_pid, &status, 0) < 0) {
        perror("waitpid");
        return;
    }
    history_add(history, child_pid, getpid(), status, description);
    if (WIFEXITED(status)) {
        printf("[Parent process] Child %ld finished with exit status %d.\n",
               (long)child_pid, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("[Parent process] Child %ld ended by signal %d.\n",
               (long)child_pid, WTERMSIG(status));
    }
}
