#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>

#include "bst.h"
#include "commands.h"
#include "argsHandler.h"

int main(int argc, char **argv)
{
	unsigned int n = parseArgs(argc, argv);
	BST_t *bst = createTree();
	while (1)
		if (readAndExecuteCommand(bst, n))
			break;
	deleteTree(bst);
	return 0;
}
