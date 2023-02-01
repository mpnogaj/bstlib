#include "commands.h"

#include <ctype.h>

#include "bst.h"


typedef enum GetCommandResult
{
	EMPTY,
	OK,
	UNKNOWN_COMMAND,
	INVALID_ARGS,
	ERROR
} GetCommandResult;

/**
 * @brief Reads all chars from stdin untill EOF or \\n is reached
 */
void readTillEndOrNewLine();

/**
 * @brief Reads command from stdin and argument if needed
 * 
 * @param cmd 
 * @param arg 
 * @return GetCommandResult function result. See @ref GetCommandResult "for more information"
 */
GetCommandResult readCommand(command_t **cmd, int *arg);
void printCommand(command_t *command);

#define CREATE_COMMAND(CMD, REQ, DESC) (command_t){.command = CMD, .requireArgument = REQ, .description = DESC}
#define AVAILABLE_COMMANDS_CNT 6
command_t availableCommands[AVAILABLE_COMMANDS_CNT] = {
	CREATE_COMMAND('I', true, "Wstaw wartosc do drzewa"),
	CREATE_COMMAND('Q', true, "Sprawdz czy wartosc istnieje w drzewie"),
	CREATE_COMMAND('D', true, "Usun wartosc z drzewa (jezeli istnieje)"),
	CREATE_COMMAND('P', false, "Wypisz drzewo"),
	CREATE_COMMAND('E', false, "Wyjdz z programu"),
	CREATE_COMMAND('H', false, "Wypisz liste dostepnych polecen")
};

/* Public definitions */
void printCommands()
{
	printf("Dostepne polecenia:\n");
	for(int i = 0; i < AVAILABLE_COMMANDS_CNT; i++)
		printCommand(&availableCommands[i]);
}

bool readAndExecuteCommand(BST_t *tree, unsigned int terminalWidth)
{
	printf(">>> ");
	command_t *cmd;
	int arg = -1;
	GetCommandResult getCmdRes = readCommand(&cmd, &arg);
	if(getCmdRes == EMPTY) return false;
	readTillEndOrNewLine();
	if(getCmdRes != OK)
	{
		if(getCmdRes == UNKNOWN_COMMAND) fprintf(stderr, "Nieznane polecenie\n");
		else if (getCmdRes == INVALID_ARGS) fprintf(stderr, "Niepoprawne argumenty\n");
		fprintf(stderr, "Uzyj polecenia h/H aby wyswietlic dostepne polecenia i argumenty\n");
		return false;
	}
	bool cmdRes = 0;
	switch (cmd->command)
	{
	case 'E':
		return true;
	case 'P':
		printTree(tree, terminalWidth);
		break;
	case 'H':
		printCommands();
		break;
	case 'I':
		cmdRes = addNode(tree, arg);
		printf("%sdalo sie dodac %d do drzewa\n", cmdRes ? "U" : "Nie u", arg);
		break;
	case 'D':
		cmdRes = deleteNode(tree, arg);
		printf("%sdalo sie usunac %d z drzewa\n", cmdRes ? "U" : "Nie u", arg);
		break;
	case 'Q':
		cmdRes = findNode(tree, arg);
		printf("%snaleziono %d w drzewa\n", cmdRes ? "Z" : "Nie z", arg);
		break;
	}
	return false;
}

/* Private definitions */
void printCommand(command_t *command)
{
	if(command == NULL) return;
	fprintf(stdout, "  %c/%c%-13s%s\n", tolower(command->command), command->command, 
		command->requireArgument ? " <ARGUMENT>:" : ":", command->description);
}

command_t *getCommand(char c)
{
	for (int i = 0; i < AVAILABLE_COMMANDS_CNT; i++)
		if (availableCommands[i].command == toupper(c))
			return &availableCommands[i];
	return NULL;
}

void readTillEndOrNewLine()
{
	//read untill \n or EOF
	int ch = 0;
	while(1)
	{
		ch = getchar();
		if(ch == EOF || ch == '\n') break;
	}
}

GetCommandResult readCommand(command_t **cmd, int *arg)
{
	if(cmd == NULL || arg == NULL) return ERROR;
	int ch = 0;
	while (1)
	{
		ch = getchar();
		if(!isspace(ch))
		{
			*cmd = getCommand(ch);
			break;
		}
		else if(ch == EOF || ch == '\n') return EMPTY;
	}
	if(*cmd == NULL) return UNKNOWN_COMMAND;
	if(!((*cmd)->requireArgument)) return OK;
	if(scanf("%d", arg) != 1) return INVALID_ARGS;
	return OK;
}