#define _CRT_SECURE_NO_WARNINGS

/*
* File: fileHandler.c
* -----------
* Implementation av filhantering
*/

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRICATE

#define NBuckets 101

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
#include "env.h"
#include "symtab.h"
#include "scanadt.h"

void loadFromFile(string fileName){
	FILE *infile;
	string line;
	int i;
	

	infile = fopen(fileName, "r");
	if (infile == NULL) Error("Cant open %s", fileName);
	line = ReadLine(infile);
	while (line != NULL){
		if (line[0] != '#' && line[0] != '\0'){
			line = Concat(":d ", line);
			i = StringLength(line);
			if (line[i-1] == '{'){
				line = Concat(line, ReadLine(infile));
				while (line[i-1] != '}'){
					i = StringLength(line);
					line = Concat(line, ReadLine(infile));
				}
			}		
			ExecuteCommand(line);
		}
		line = ReadLine(infile);
	}	
	printf("%s loaded\n", fileName);
	fclose(infile);
}