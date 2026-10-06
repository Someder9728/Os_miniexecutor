#include "command.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *skip_spaces(char *text) {
    while (*text != '\0' && isspace((unsigned char)*text)) {
        text++;
    }
    return text;
}

static int parse_arguments(char *text, Command *command) {
    char *cursor = skip_spaces(text);

    while (*cursor != '\0') {
        char quote = '\0';
        char *start;

        if (command->argc >= MAX_ARGUMENTS - 1) {
            fprintf(stderr, "Too many arguments (maximum: %d).\n", MAX_ARGUMENTS - 1);
            return -1;
        }

        if (*cursor == '\"' || *cursor == '\'') {
            quote = *cursor++;
        }
        start = cursor;

        while (*cursor != '\0' && (quote ? *cursor != quote : !isspace((unsigned char)*cursor))) {
            cursor++;
        }
        if (quote && *cursor != quote) {
            fprintf(stderr, "Unclosed quote in command.\n");
            return -1;
        }
        if (cursor == start) {
            fprintf(stderr, "Empty argument is not supported.\n");
            return -1;
        }

        command->argv[command->argc] = malloc((size_t)(cursor - start) + 1U);
        if (command->argv[command->argc] == NULL) {
            perror("malloc");
            return -1;
        }
        memcpy(command->argv[command->argc], start, (size_t)(cursor - start));
        command->argv[command->argc][cursor - start] = '\0';
        command->argc++;

        if (quote) {
            cursor++;
        }
        cursor = skip_spaces(cursor);
    }
    command->argv[command->argc] = NULL;
    return 0;
}

int read_command(Command *command) {
    char *line;
    char *name;
    char *after_name;

    memset(command, 0, sizeof(*command));
    command->type = COMMAND_EMPTY;

    printf("mce> ");
    fflush(stdout);
    if (fgets(command->input, sizeof(command->input), stdin) == NULL) {
        return 0;
    }
    command->input[strcspn(command->input, "\n")] = '\0';
    line = skip_spaces(command->input);
    if (*line == '\0') {
        return 1;
    }

    name = line;
    while (*line != '\0' && !isspace((unsigned char)*line)) {
        line++;
    }
    if (*line != '\0') {
        *line++ = '\0';
    }
    after_name = line;

    if (strcmp(name, "run") == 0) {
        command->type = COMMAND_RUN;
        if (parse_arguments(line, command) != 0 || command->argc == 0) {
            if (command->argc == 0) {
                fprintf(stderr, "Usage: run <program> [arguments...]\n");
            }
            free_command(command);
            command->type = COMMAND_INVALID;
        }
    } else if (strcmp(name, "help") == 0 && *skip_spaces(after_name) == '\0') {
        command->type = COMMAND_HELP;
    } else if (strcmp(name, "history") == 0 && *skip_spaces(after_name) == '\0') {
        command->type = COMMAND_HISTORY;
    } else if (strcmp(name, "exit") == 0 && *skip_spaces(after_name) == '\0') {
        command->type = COMMAND_EXIT;
    } else {
        command->type = COMMAND_INVALID;
        fprintf(stderr, "Unknown or malformed command. Type 'help' for usage.\n");
    }
    return 1;
}

void free_command(Command *command) {
    int i;
    for (i = 0; i < command->argc; i++) {
        free(command->argv[i]);
        command->argv[i] = NULL;
    }
    command->argc = 0;
}

void print_help(void) {
    puts("\nMini Command Executor commands:");
    puts("  run <program> [arguments...]  Execute a program in a child process");
    puts("  history                       Show processes executed in this session");
    puts("  help                          Show this message");
    puts("  exit                          Close the program");
    puts("\nExamples:");
    puts("  run ls -la");
    puts("  run echo \"hello process\"");
}
