#include <stdio.h>  
#include <ctype.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "exp.h"
#include "scanadt.h"

/*Private functions*/
static expADT readE(scannerADT scanner);
static expADT readT(scannerADT scanner);
static expADT readC(scannerADT scanner);
static expADT readF(scannerADT scanner);
static bool  isRelOperator(char op);

/*Helper functions*/
static expADT ifClause(scannerADT scanner);
static expADT functionWithArgument(scannerADT scanner);

static expADT equalSignE(scannerADT scanner);



expADT ParseExp(scannerADT scanner){


	expADT exp = NULL;

	//exp = equalSignE(scanner);
	exp = readE(scanner);

	if (MoreTokensExist(scanner))
		Error("ParseExp: %s unexpected tokens left in scannner.\n", ReadToken(scanner));

	return exp;
}

/*
* Function: readE
* Usage: exp = readE(scanner);
* -------------------------------
* This function parses a MFL expression by reading tokens from
* the scanner. It primarily handles expressions that 
* is separated by a + or - sign. All other expressions are 
* delegated down to the next function.
*/
static expADT readE(scannerADT scanner){
	//       
	expADT exp = NULL;
	string token;
	char op;
	expADT lhs, rhs;

	exp = readT(scanner);			//  E ::= T


	token = ReadToken(scanner);

	if (StringEqual(token, "+") || StringEqual(token, "-")){     	//  E ::= T + E   
		op = token[0];                							    //  E ::= T – E  
		lhs = exp;
		rhs = readE(scanner);

		exp = NewCompoundExp(op, lhs, rhs);
	}
	else
		SaveToken(scanner, token);  //  Not an expression with + or -

	return exp;
}

/*
* Function: readT
* Usage: exp = readT(scanner);
* -------------------------------
* This function parses a MFL expression by reading tokens from
* the scanner. It primarily handles expressions that
* is separated by a * or / sign. All other expressions are
* delegated down to the next function.
*/
static expADT readT(scannerADT scanner){

	expADT exp = NULL;
	string token;
	char op;
	expADT lhs, rhs;

	exp = readC(scanner);			//  T ::= C 


	token = ReadToken(scanner);

	if (StringEqual(token, "*") || StringEqual(token, "/")){     	//  T ::= C * T   
		op = token[0];                								 //  T ::= C / T 
		lhs = exp;												  
		rhs = readT(scanner);

		exp = NewCompoundExp(op, lhs, rhs);
	}
	else
		SaveToken(scanner, token);  //  Not an expression with * or /

	return exp;
}

/*
* Function: readC
* Usage: exp = readC(scanner);
* -------------------------------
* This function parses a MFL expression by reading tokens from
* the scanner. It primarily handles expressions that
* is a call to a function. All other expressions are
* delegated down to the next function.
*/
static expADT readC(scannerADT scanner){
	
	expADT exp = NULL;
	string token;

	expADT func, arg;

	exp = readF(scanner);				//C ::= F 


	token = ReadToken(scanner);

	if (StringEqual(token, "(")){     //C :: = F(E)    // Call to function.
		arg = readE(scanner);

		if (!StringEqual(ReadToken(scanner), ")"))
			Error("Call to function - parentheses do not match.\n");
		else {
			func = exp;
			exp = NewCallExp(func, arg);
		}
	}
	else
		SaveToken(scanner, token); //We have no call to function. Restore scanner.

	return exp;
}

/*
* Function: readF
* Usage: exp = readF(scanner);
* -------------------------------
* This function parses a MFL expression by reading tokens from
* the scanner. It primarily handles expressions that
* will define an if-clause or a function. If all other expressions are
* basic variables or integers, the function is done. If there is an 
* if-clause or a function, the function delegates the work down to 
* the next function.
*/
static expADT readF(scannerADT scanner){

	expADT exp = NULL;
	string token;

	token = ReadToken(scanner);

	if (StringEqual(token, "(")){     // F :: = (E)
		exp = readE(scanner);

		if (!StringEqual(ReadToken(scanner), ")"))
			Error("Parentheses do not match.\n");
	}
	else if (StringEqual(token, "if")){   //F :: = if E RelOp E then E else E // If expression 
		exp = ifClause(scanner);
	}
	else if (StringEqual(token, "func")){  //F ::= func (identifier) { E } // Function with argument
		exp = functionWithArgument(scanner);
	}
	else if (isdigit(token[0])){           //F ::= integer
		exp = NewIntegerExp(StringToInteger(token));
		//exp = NewFuncExp("", NewIntegerExp(StringToInteger(token)));
	}
	else if (isalpha(token[0])){            //F ::= identifier  
		//exp = NewFuncExp("", NewIdentifierExp(token)); //test
		exp = NewIdentifierExp(token); //Behöver inte göra en ny func, räcker med ny identifierare
		}
	else {
		Error("Illegal term or empty expression.\n");
	}
	return exp;
}

/*
* Function: ifClause
* Usage: exp = ifClause(scanner);
* -------------------------------
* This function parses a MFL expression by reading tokens from
* the scanner. It primarily handles expressions that
* will define an if-clause.  It is a helper function
* to readF().
*/
static expADT ifClause(scannerADT scanner){

	expADT  exp = NULL;
	string token;

	expADT lhs, rhs, ifPart, elsePart;
	char relOp;
	lhs = readE(scanner);  //E before RelOp
	token = ReadToken(scanner);
	if (isRelOperator(relOp = token[0])){  //F :: = if E RelOp E then E else E // If expression 
		rhs = readE(scanner);  //E after RelOp

		if (StringEqual(ReadToken(scanner), "then")){
			ifPart = readE(scanner);  //E after then
			if (StringEqual(ReadToken(scanner), "else")){
				elsePart = readE(scanner);  // E after else
				exp = NewIfExp(lhs, relOp, rhs, ifPart, elsePart);
			}
		}
		else
			Error("then is missing from if-expression.\n");
	}
	else {
		Error("Relative token is not valid in if-expression.\n");
	}
	return exp;
}

/*
* Function: functionWithArgument
* Usage: exp = functionWithArgument(scanner);
* -------------------------------
* This function parses a MFL expression by reading tokens from
* the scanner. It primarily handles expressions that
* will define a function with one argument. It is a helper function
* to readF().
*/
static expADT functionWithArgument(scannerADT scanner){

	expADT exp = NULL;

	string arg = NULL;
	expADT body = NULL;
	if (StringEqual(ReadToken(scanner), "("))  //F ::= func (identifier) { E } // Function with argument
		arg = ReadToken(scanner);
	else
		Error("Function start parethesis malformed.\n");
	if (StringEqual(ReadToken(scanner), ")"))
		;
	else
		Error("Function end parenthesis malformed.\n");
	if (StringEqual(ReadToken(scanner), "{"))
		body = readE(scanner);
	else
		Error("Function body start character not valid.\n");
	if (StringEqual(ReadToken(scanner), "}"))
		exp = NewFuncExp(arg, body);
	else
		Error("Function body end character not valid.\n");
	return exp;
}

/*
* Function: isRelOperator
* Usage: answer = isRelOperator(op);
* -------------------------------
* This function checks if an operator is one of the 
* expected - see code - and returns TRUE if it is. 
* Otherwise it returns FALSE.
*/
static bool  isRelOperator(char op){

	if (op == '<' || op == '>' || op == '=') //RelOp :: = =
		                                     //RelOp :: = <
	                                         //RelOp :: = >
		return TRUE;

	return FALSE;
}
