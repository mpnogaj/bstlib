#include "argsHandler.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "commands.h"

typedef struct option_t
{
	char *longName;
	char shortName;
	char *description;
} option_t;

#define CREATE_OPTION(LNAME, SNAME, DESC) (option_t){.longName = LNAME, .shortName = SNAME, .description = DESC}

#define OPTIONS_CNT 2
option_t options[OPTIONS_CNT] = {
	CREATE_OPTION("help", 'h', "Wypisz instrukcje uzycia i zakoncz program"),
	CREATE_OPTION("commands", 'c', "Wypisz dostepne polecenia i zakoncz program")
};

void printHelp(char *programName);
void printOptionDescription(option_t *option);
void printWrongUsage(char *programName);
void printCommands();

void executeOption(int id, char *programName)
{
	switch (id)
	{
	case 0: //help
		printHelp(programName);
		exit(0);
	case 1: //commands
		printCommands();
		exit(0);
	default: //invalid option
		printWrongUsage(programName);
		exit(1);
	}
}

unsigned int parseArgs(int argc, char *argv[])
{
	//Get program name. If not specified get fallback name
	char* programName = argc < 1 || argv[0] == NULL || argv[0][0] == '\0' ? "bst" : argv[0];
	bool valueRead = false;
	unsigned int n;
	for(int i = 1; i < argc; i++)
	{
		//option
		if(argv[i][0] == '\0') continue;
		if(argv[i][0] == '-')
		{
			bool longOption = argv[i][1] == '-';
			int j = 0;
			for(; j < OPTIONS_CNT; j++)
			{
				option_t *option = &options[j];
				if(longOption && strcmp(argv[i] + 2, option->longName) == 0) break;
				else if(argv[i][1] == option->shortName) break;
			}
			executeOption(j, programName);
		}
		else
		{
			errno = 0;
			char *end;
			n = strtoll(argv[1], &end, 10);

			if (end == argv[1] || errno == ERANGE || n <= 0)
			{
				printWrongUsage(programName);
				exit(1);
			}
			valueRead = 1;
			break;
		}
	}

	if(!valueRead)
	{
		printWrongUsage(programName);
		exit(1);
	}

	return n;
}

void printHelp(char *programName)
{
	printf("Uruchamianie:\n  %s [OPCJE] <SZEROKOSC_TERMINALA>\n\n", programName);
	printf("Opcje:\n");
	for(int i = 0; i < OPTIONS_CNT; i++)
		printOptionDescription(&options[i]);
	printf("\nProgram dziala w interaktywnej sesji. Dostepne polecenia mozna zobaczyc uruchamiajac program z odpowiednia opcja (patrz wyzej)\n");
}

void printOptionDescription(option_t *option)
{
	printf("  --%-8s -%-1c: %s\n", option->longName, option->shortName, option->description);
}

void printWrongUsage(char *programName)
{
	fprintf(stderr, "Nieprawidlowe argumenty wywolania programu.\n");
	fprintf(stderr, "Uzyj '%s --help' zeby zobaczyc wiecej informacji\n", programName);
}