#ifndef MONITOR_H
#define MONITOR_H

#include <sys/types.h>

#define HISTORY_CAPACITY 100
#define HISTORY_COMMAND_LENGTH 512

typedef struct {
    pid_t pid;
    pid_t ppid;
    int status;
    char command[HISTORY_COMMAND_LENGTH];
} ProcessRecord;

typedef struct {
    ProcessRecord records[HISTORY_CAPACITY];
    int count;
} ProcessHistory;

void history_init(ProcessHistory *history);
void print_parent_info(pid_t parent_pid, pid_t child_pid);
void print_child_info(pid_t child_pid, pid_t parent_pid, const char *command);
void history_add(ProcessHistory *history, pid_t pid, pid_t ppid, int status, const char *command);
void history_print(const ProcessHistory *history);

#endif
