#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "bst.h"

#define VALID_ARGS_LEN 6
char validArgs[VALID_ARGS_LEN] = {'I', 'Q', 'D', 'P', 'Q', 'E'};

int validateArgument(char c)
{
	for (int i = 0; i < VALID_ARGS_LEN; i++)
		if (validArgs[i] == c)
			return 1;
	return 0;
}

char getCommand()
{
	char x;
	while (1)
	{
		x = getchar();
		if (isspace(x))
			continue;
		if (validateArgument(x))
			return x;
		printf("Bledne polecenie\n");
	}
	return '\0';
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
		char c = getCommand();
		if (c == 'E')
		{
			deleteTree(bst);
			return 0;
		}
		if (c == 'P')
		{
			printTree(bst, N);
		}
		else
		{
			int x;
			while (scanf("%d", &x) != 1)
				getchar();
			int res;
			switch (c)
			{
			case 'I':
				res = addNode(bst, x);
				printf("%sdalo sie dodac %d do drzewa\n", res ? "U" : "Nie u", x);
				break;
			case 'Q':
				res = findNode(bst, x);
				printf("%snaleziono %d w drzewie.\n", res ? "Z" : "Nie z", x);
				break;
			case 'D':
				res = deleteNode(bst, x);
				printf("%sdalo sie usunac %d do drzewa\n", res ? "U" : "Nie u", x);
				break;
			default:
				deleteTree(bst);
				return 1;
			}
		}
	}

	deleteTree(bst);
	return 0;
}
