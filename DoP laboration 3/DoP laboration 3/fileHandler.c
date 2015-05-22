/*
* File: fileHandler.c
* -----------
* Implementation av filhantering
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
#include "env.h"
#include "symtab.h"
#include "scanadt.h"

void saveToFile(string fileName, environmentADT environment, int NBuckets){
	 FILE *outfile;
	 fopen(fileName, "w");
	 int i;
	 

	 for (i = 0; i < NBuckets; i++){
		 fprintf(outfile, "%s\n", environment->definitions->buckets[i].key);
	 }
	 
	 fclose(outfile);
}

void loadFromFile(string fileName, int NBuckets){
	FILE *infile;
	string line;
	int i;
	

	fopen(fileName, "r");
	if (infile == NULL) Error("Cant open %s", fileName);

	for (i = 0; i < NBuckets; i++){
		line = ReadLine(infile);
		ExecuteCommand(line);
	}	

	fclose(infile);
	return;
}