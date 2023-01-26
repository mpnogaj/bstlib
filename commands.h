#ifndef COMMANDS
#define COMMANDS

#include <stdbool.h>
#include <stdio.h>

#include "bst.h"

/**
 * @brief Struct representing available commands
 */
typedef struct command_t
{
	char command;
	bool requireArgument;
	char *description;
} command_t;

/**
 * @brief Reads and executes command, from stdin
 * 
 * @param tree pointer to tree on which commands will be executed
 * @param terminalWidth width of the available drawing space
 * @return true if program should finish
 * @return false if program should continue
 */
bool readAndExecuteCommand(BST_t *tree, unsigned int terminalWidth);

/**
 * @brief Print desciption of available commands
 */
void printCommands();

#endif // COMMANDS
