#include "monitor.h"

#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

void history_init(ProcessHistory *history) {
    history->count = 0;
}

void print_parent_info(pid_t parent_pid, pid_t child_pid) {
    printf("\n[Parent process]\nPID: %ld | Waiting for child PID: %ld\n",
           (long)parent_pid, (long)child_pid);
}

void print_child_info(pid_t child_pid, pid_t parent_pid, const char *command) {
    printf("\n[Child process]\nPID: %ld | PPID: %ld | Command: %s\n",
           (long)child_pid, (long)parent_pid, command);
    fflush(stdout);
}

void history_add(ProcessHistory *history, pid_t pid, pid_t ppid, int status, const char *command) {
    ProcessRecord *record;
    if (history->count == HISTORY_CAPACITY) {
        memmove(&history->records[0], &history->records[1],
                sizeof(history->records[0]) * (HISTORY_CAPACITY - 1));
        history->count--;
    }
    record = &history->records[history->count++];
    record->pid = pid;
    record->ppid = ppid;
    record->status = status;
    snprintf(record->command, sizeof(record->command), "%s", command);
}

void history_print(const ProcessHistory *history) {
    int i;
    if (history->count == 0) {
        puts("\nProcess history is empty.");
        return;
    }
    puts("\nProcess History");
    puts("PID       PPID      RESULT             COMMAND");
    puts("---------------------------------------------------------------");
    for (i = 0; i < history->count; i++) {
        const ProcessRecord *record = &history->records[i];
        if (WIFEXITED(record->status)) {
            printf("%-9ld %-9ld exit %-10d %s\n", (long)record->pid, (long)record->ppid,
                   WEXITSTATUS(record->status), record->command);
        } else if (WIFSIGNALED(record->status)) {
            printf("%-9ld %-9ld signal %-8d %s\n", (long)record->pid, (long)record->ppid,
                   WTERMSIG(record->status), record->command);
        } else {
            printf("%-9ld %-9ld unknown            %s\n", (long)record->pid,
                   (long)record->ppid, record->command);
        }
    }
}
