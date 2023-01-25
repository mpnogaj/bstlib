#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "bst.h"

#define VALID_ARGS_LEN 6
char validArgs[VALID_ARGS_LEN] = {'i', 'q', 'd', 'p', 'q', 'e'};

typedef enum
{
	EMPTY,
	OK,
	UNKNOWN_COMMAND,
	INVALID_ARGS
} GetCommandResult;

int validateCommand(char c)
{
	for (int i = 0; i < VALID_ARGS_LEN; i++)
		if (validArgs[i] == c)
			return 1;
	return 0;
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

GetCommandResult getCommand(char* cmd, int* arg)
{
	int ch = 0;
	while (1)
	{
		ch = getchar();
		if(!isspace(ch))
		{
			*cmd = tolower(ch);
			break;
		}
		else if(ch == EOF || ch == '\n') return EMPTY;
	}
	if(!validateCommand(*cmd)) return UNKNOWN_COMMAND;
	//don't need argument
	if(*cmd == 'e' || *cmd == 'p') return OK;
	if(scanf("%d", arg) != 1) return INVALID_ARGS;
	return OK;
}

void printHelp()
{
	fprintf(stderr, "Program nalezy obowiazkowo wywolac z dokladnie jedym parametrem N - dodatnia liczba naturalna bedaca szerokoscia wyjscia, podana w znakach\nPrzykladowe wywolanie: ./bst 200\n");
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printHelp();
		return 1;
	}


	errno = 0;
	char *end;
	int N = strtoll(argv[1], &end, 10);

	if (end == argv[1] || errno == ERANGE || N <= 0)
	{
		printHelp();
		return 1;
	}


	BST_t *bst = createTree();
	while (1)
	{
		char cmd = '\0';
		int arg = -1;
		GetCommandResult getCmdRes = getCommand(&cmd, &arg);
		if(getCmdRes == EMPTY) continue;
		readTillEndOrNewLine();
		if(getCmdRes != OK)
		{
			if(getCmdRes == UNKNOWN_COMMAND) fprintf(stderr, "Nieznane polecenie\n");
			else if(getCmdRes == INVALID_ARGS) fprintf(stderr, "Niepoprawne argumenty\n");
			continue;
		}
		int cmdRes = 0;
		switch (cmd)
		{
		case 'e':
			deleteTree(bst);
			return 0;
			break;
		case 'p':
			printTree(bst, N);
			break;
		case 'i':
			cmdRes = addNode(bst, arg);
			printf("%sdalo sie dodac %d do drzewa\n", cmdRes ? "U" : "Nie u", arg);
			break;
		case 'd':
			deleteNode(bst, arg);
			printf("%sdalo sie usunac %d z drzewa\n", cmdRes ? "U" : "Nie u", arg);
			break;
		case 'q':
			findNode(bst, arg);
			printf("%snaleziono %d w drzewa\n", cmdRes ? "Z" : "Nie z", arg);
			break;
		}
	}

	deleteTree(bst);
	return 0;
}
