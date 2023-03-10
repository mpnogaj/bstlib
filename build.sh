#!/bin/bash
if [ $# != "0" ] && [ $1 = "debug" ]; then
	echo "Debug build"
	gcc bst.c bstPrinter.c argsHandler.c commands.c main.c  -o bst -Werror -Wall -Wshadow -Wextra -g -fsanitize=address -lm
else
	echo "Release build"
	gcc bst.c bstPrinter.c argsHandler.c commands.c main.c  -o bst -Werror -Wall -Wshadow -Wextra -lm
fi