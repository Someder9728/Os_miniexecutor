#ifndef PROCESS_H
#define PROCESS_H

#include "command.h"
#include "monitor.h"

void execute_program(const Command *command, ProcessHistory *history);

#endif
