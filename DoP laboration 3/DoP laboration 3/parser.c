#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "exp.h"
#include "scanadt.h"

static expADT readE(scannerADT scanner);
static expADT readT(scannerADT scanner);
static expADT readC(scannerADT scanner);
static expADT readF(scannerADT scanner);
static bool  isRelOperator(char op);

static expADT ifClause(scannerADT scanner);
static expADT functionWithArgument(scannerADT scanner);

expADT ParseExp(scannerADT scanner){

}


static expADT readE(scannerADT scanner){


}

static expADT readT(scannerADT scanner){


}

static expADT readC(scannerADT scanner){


}

static expADT readF(scannerADT scanner){

	expADT exp;
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
	}
	else if (isalpha(token[0])){            //F ::= identifier  
		exp = NewIdentifierExp(token);
	}
	else {
		Error("Illegal term in expression");
	}

	return exp;
}

static expADT ifClause(scannerADT scanner){

	expADT exp;
	string token;

	expADT lhs, rhs, ifPart, elsePart;
	char relOp;
	lhs = readE(scanner);
	token = ReadToken(scanner);
	if (isRelOperator(relOp = token[0])){
		rhs = readE(scanner);
		ifPart = readE(scanner);
		elsePart = readE(scanner);
		exp = NewIfExp(lhs, relOp, rhs, ifPart, elsePart);
	}
	else {
		Error("Relative token is not valid in IF-expression.\n");
	}

	return exp;
}

static expADT functionWithArgument(scannerADT scanner){

	expADT exp;
	string token;

	string arg;
	expADT body;
	if (StringEqual(ReadToken(scanner), "("))
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

static bool  isRelOperator(char op){

	if (op == '<' || op == '>' || op == '=') //RelOp :: = =
		//RelOp :: = <
		//RelOp :: = >
		return TRUE;

	return FALSE;
}