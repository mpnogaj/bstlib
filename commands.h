#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>
#include <stdio.h>

#include "bst.h"

typedef struct command_t
{
	char command;
	bool requireArgument;
	char *description;
} command_t;

#define AVAILABLE_COMMANDS_CNT 6
extern command_t availableCommands[AVAILABLE_COMMANDS_CNT];

/**
 * @brief Reads and executes command from stdin
 * 
 * @return true if program should finish
 * @return false if program should continue
 */
bool readAndExecuteCommand(BST_t *tree, unsigned int terminalWidth);
void printCommands();

#endif // COMMANDS_H
