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


/*
* Type: commandFnT
* ----------------
* This type defines the class of command functions.
*/

typedef void(*commandFnT)(void);

/*
* Type: commandEntryT
* -------------------
* This type consists of a structure containing only a command
* function.  This extra level of structure is required because
* function pointers are not compatible with void * in ANSI C.
*/

typedef struct {
	commandFnT fn;
} *commandEntryT;

/*
* Private variable: commandTable
* ------------------------------
* The entries in this table are used to hold the commands and
* their corresponding actions.
*/

static symtabADT commandTable;

/* Local function declarations */

static void defineCommand(string cmd, commandFnT fn);
static void loadCmd(scannerADT scanner);
static void defineValueCmd(scannerADT scanner);
static void helpCmd(void);
static void quitCmd(void);

/* Main test program */

void InitCommandTable(void)
{
	commandTable = NewSymbolTable();
	defineCommand(":load", loadCmd);
	defineCommand(":l", loadCmd);
	defineCommand(":define", defineValueCmd);
	defineCommand(":d", defineValueCmd);
	defineCommand(":help", helpCmd);
	defineCommand(":h", helpCmd);
	defineCommand(":quit", quitCmd);
	defineCommand(":q", quitCmd);
}

static void defineCommand(string cmd, commandFnT fn)
{
	commandEntryT entry;

	entry = New(commandEntryT);
	entry->fn = fn;
	Enter(commandTable, cmd, entry);
}

void ExecuteCommand(string cmd){
	
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
	if (StringEqual(token, ":l") || StringEqual(token, ":load") || StringEqual(token, ":d") || StringEqual(token, ":define"))
		entry->fn(scanner);
	else
		entry->fn();

	FreeScanner(scanner);
}

/* Command dispatch functions */

static void loadCmd(string cmd)
{
	printf("Command = load %s\n", cmd);
}

static void defineValueCmd(scannerADT scanner)
{
	expADT body, newFunc;
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
		DefineIdentifier( environment, variable, NewFuncExp(variable,GetFuncBody(ParseExp(scanner))), NewClosure(environment));
	}
	else{
		SaveToken(scanner, value);
		body =  GetFuncBody(ParseExp(scanner));
		newFunc = NewFuncExp("",body);
		DefineIdentifier( environment, variable, newFunc, NewClosure(environment));
	}
	//printf("Command = define value %s\n", cmd);
}

static void helpCmd(void)
{
	printf("Command = help\n");
}

static void quitCmd(void)
{
	printf("Command = quit\n");
	exit(0);
}
