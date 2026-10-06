#ifndef COMMAND_H
#define COMMAND_H

#define MAX_LINE_LENGTH 1024
#define MAX_ARGUMENTS 64

typedef enum {
    COMMAND_EMPTY,
    COMMAND_RUN,
    COMMAND_HELP,
    COMMAND_HISTORY,
    COMMAND_EXIT,
    COMMAND_INVALID
} CommandType;

typedef struct {
    CommandType type;
    int argc;
    char *argv[MAX_ARGUMENTS];
    char input[MAX_LINE_LENGTH];
} Command;

int read_command(Command *command);
void free_command(Command *command);
void print_help(void);

#endif
