#define _CRT_SECURE_NO_WARNINGS

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

environmentADT environment;

/* Main program */

/*main(){

	string line;

	InitCommandTable();
	while (TRUE) {
		try{
			printf("=> ");
			line = GetLine();
			if (StringEqual(":", CharToString(line[0])))
				ExecuteCommand(line);
		}endtry
	}
}
*/


//Use this main to test parser.
void main(void){

	scannerADT scanner;
	expADT exp;
	string line;
	valueADT value;
	

	printf("Expression interpreter (type \"quit\" to exit)\n\n");

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
				ExecuteCommand(line);
			else{
				SetScannerString(scanner, line);
				exp = ParseExp(scanner);
				PrintExp(exp); printf("\n");
				value = Eval(exp, environment); printf("\n");
				PrintValue(value);
			}
			except(ErrorException)
				printf("Error: %s\n", (string)GetExceptionValue());
		} endtry
	}
}
