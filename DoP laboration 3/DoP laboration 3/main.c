
#include <stdio.h>
#include <ctype.h>

#include "genlib.h"
#include "strlib.h"
#include "simpio.h"

#include "cmdTable.h"
#include "exception.h"

/* Main program */

main(){

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

/*void interprator(){

	scannerADT scanner;
	expressionADT exp;
	string line;
	int value;

	printf("Expression interpreter (type \"quit\" to exit)\n\n");

	InitVariableTable();
	scanner = NewScanner();
	SetScannerSpaceOption(scanner, IgnoreSpaces);



	while (TRUE) {
		try {
			printf("=> ");
			line = GetLine();
			if (StringEqual(line, "quit")) exit(0);
			SetScannerString(scanner, line);
			exp = ParseExp(scanner);
			value = EvalExp(exp);
			printf("%d\n", value);
			except(ErrorException)
				printf("Error: %s\n", (string)GetExceptionValue());
		} endtry
	}
}*/