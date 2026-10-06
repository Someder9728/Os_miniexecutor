#include "command.h"
#include "monitor.h"
#include "process.h"

#include <stdio.h>

int main(void) {
    Command command;
    ProcessHistory history;
    int keep_reading = 1;

    history_init(&history);
    puts("Mini Command Executor");
    puts("Type 'help' to see available commands.\n");

    while (keep_reading && read_command(&command)) {
        switch (command.type) {
            case COMMAND_RUN:
                execute_program(&command, &history);
                break;
            case COMMAND_HELP:
                print_help();
                break;
            case COMMAND_HISTORY:
                history_print(&history);
                break;
            case COMMAND_EXIT:
                keep_reading = 0;
                break;
            case COMMAND_EMPTY:
            case COMMAND_INVALID:
                break;
        }
        free_command(&command);
    }
    puts("Goodbye.");
    return 0;
}
