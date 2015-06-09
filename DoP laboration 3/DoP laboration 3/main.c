#define _CRT_SECURE_NO_WARNINGS

/*
* File: main.c
* ------------
* MFL Interpretator av S142066 Henrik Wistbacka, S142015 Tobias Ånhed, S131469 Oscar Persson
*/

#include <stdio.h>
#include <ctype.h>

#include "genlib.h"
#include "strlib.h"
#include "simpio.h"

#include "cmdTable.h"
#include "exception.h"
#include "scanadt.h"
#include "exp.h"
#include "print.h"
#include "eval.h"
#include "parser.h"
#include "fileHandler.h"

/* Main program */


//Use this main to test parser.
void main(void){

	scannerADT scanner;
	expADT exp;
	string line;
	valueADT value;

	environmentADT environment;
	
	printf("\n\n\nMFL Interpretator by S142066 Henrik Wistbacka, S142015 Tobias Anhed, S131469 Oscar Persson\n\n:h for help\n");

	InitCommandTable();
	scanner = NewScanner();
	SetScannerSpaceOption(scanner, IgnoreSpaces);
	environment = NewEnvironment();
	


	while (TRUE) {
		try {
			printf("=> ");
			line = GetLine();
			if (StringEqual(line, ":quit") || StringEqual(line, ":q")){
				FreeScanner(scanner);
				exit(0);
			}
			if (StringEqual(":", CharToString(line[0])))
				ExecuteCommand(line, environment);
			else{
				SetScannerString(scanner, line);
				exp = ParseExp(scanner);
				PrintExp(exp); printf("\n");
				value = Eval(exp, NewClosure(environment)); printf("\n");
				PrintValue(value);
			}
			except(ErrorException)
				printf("Error: %s\n", (string)GetExceptionValue());
		} endtry
	}
}
