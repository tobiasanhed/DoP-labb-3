/*
* File: cmddisp.c
* ---------------
* This program implements a command dispatch mechanism that
* stores function pointers in a symbol table.
*/

#include <stdio.h>
#include <ctype.h>

#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "symtab.h"
#include "scanadt.h"
#include "eval.h"
#include "value.h"
#include "env.h"
#include "parser.h"
#include "fileHandler.h"
#include "cmdTable.h"

typedef enum{
	noArg,
	oneArg,
	twoArg
}countArgsT;


/*
* Type: commandFnT
* ----------------
* This type defines the class of command functions.
*/

typedef void(*commandFnT)(void);

typedef void (*command2argsFnT)(scannerADT , environmentADT);

typedef void(*command1argsFnT)(scannerADT);

/*
* Type: commandEntryT
* -------------------
* This type consists of a structure containing only a command
* function.  This extra level of structure is required because
* function pointers are not compatible with void * in ANSI C.
*/

typedef struct {
	union {
		commandFnT fn;
		command1argsFnT fn1;
		command2argsFnT fn2;
	}fnTypeT;
} *commandEntryT;

/*
* Private variable: commandTable
* ------------------------------
* The entries in this table are used to hold the commands and
* their corresponding actions.
*/

static symtabADT commandTable;

/* Local function declarations */

static void defineCommand(string cmd, void *fn, countArgsT type);
static void loadCmd(scannerADT scanner, environmentADT environment);
static void defineValueCmd(scannerADT scanner, environmentADT environment);
static void helpCmd(void);
static void quitCmd(void);

/* Main test program */

void InitCommandTable(void)
{
	commandTable = NewSymbolTable();
	defineCommand(":load", loadCmd, twoArg);
	defineCommand(":l", loadCmd, twoArg);
	defineCommand(":define", defineValueCmd, twoArg);
	defineCommand(":d", defineValueCmd, twoArg);
	defineCommand(":help", helpCmd, noArg);
	defineCommand(":h", helpCmd, noArg);
	defineCommand(":quit", quitCmd, noArg);
	defineCommand(":q", quitCmd, noArg);
}

static void defineCommand(string cmd, void *fn, countArgsT type)
{
	commandEntryT entry;

	entry = New(commandEntryT);

	switch (type){

	case noArg:
		entry->fnTypeT.fn = (commandFnT)fn;
		break;
	case oneArg:
		entry->fnTypeT.fn1 = (command1argsFnT)fn;
		break;
	case twoArg:
		entry->fnTypeT.fn2 = (command2argsFnT)fn;
		break;
	default:
		Error("defineCommand with invalid parameters\n");
		break;
	}

	Enter(commandTable, cmd, entry);
}

void ExecuteCommand(string cmd, environmentADT env){
	
	commandEntryT entry;
	scannerADT scanner;
	string token;

	scanner = NewScanner();
	SetScannerString(scanner, cmd);
	SetScannerSpaceOption(scanner, IgnoreSpaces);

	token = ReadToken(scanner);
	token = Concat(token, ReadToken(scanner));

	entry = Lookup(commandTable, token);
	if (entry == UNDEFINED) {
		printf("Undefined command: %s\n", cmd);
		return;
	}
	if (StringEqual(token, ":l") || StringEqual(token, ":load"))
		entry->fnTypeT.fn2(scanner, env);
	else if (StringEqual(token, ":d") || StringEqual(token, ":define"))
		entry->fnTypeT.fn2(scanner, env);
	else
		entry->fnTypeT.fn();

	FreeScanner(scanner);
}

/* Command dispatch functions */

static void loadCmd(scannerADT scanner, environmentADT environment ){

	loadFromFile(Concat(ReadToken(scanner), ".mfl"), environment);
}

static void defineValueCmd(scannerADT scanner, environmentADT environment)
{
	expADT  newFunc, parseBody;
	string variable,
		value,
		token;

	variable = ReadToken(scanner);
	token = ReadToken(scanner);
	if(!StringEqual(token, "="))
		Error("No valid definition.");
	value = ReadToken(scanner);

	if(StringEqual(value, "func")){
		SaveToken(scanner, value);
		DefineIdentifier(environment, variable, ParseExp(scanner), NewClosure(environment));
	}
	else{
		SaveToken(scanner, value);
		parseBody = ParseExp(scanner);
		// Behöver inte göra en ny func här heller..
		DefineIdentifier(environment, variable, parseBody, NewClosure(environment));
	}
}

static void helpCmd(void)
{
	printf(":d or :define to define a variable or function.\n");
	printf(":d x = 42\n");
	printf(":d pow = func (x) { x * x }\n");
	printf(":h or :help for this menu of help\n");
	printf(":l or :load to load a file\n");
	printf(":l filename\n");
	printf(":q or :quit to end this program\n");
}

static void quitCmd(void)
{
	printf("Command = quit\n");
	exit(0);
}
