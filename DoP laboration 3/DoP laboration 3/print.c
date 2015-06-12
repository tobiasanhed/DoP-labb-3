/*
* File: print.c
* ------------
* This file implements the print.h interface. 
*/


#include "value.h"
#include "exp.h"
#include "eval.h"

void PrintExp(expADT exp){
	
	switch (ExpType(exp)){

	case FuncExp:
		printf(" func ");
		printf("(%s) ", GetFuncFormalArg(exp));
		printf(" { ");
		PrintExp(GetFuncBody(exp));
		printf(" } ");
		break;

	case IfExp:
		printf("if ");
		PrintExp(GetIfLHSExpression(exp));
		printf(" %c ", GetIfRelOp(exp));
		PrintExp(GetIfRHSExpression(exp));
		printf(" then ");
		PrintExp(GetIfThenPart(exp));
		printf(" else ");
		PrintExp(GetIfElsePart(exp));
		break;

	case CallExp:
		PrintExp(GetCallExp(exp));
		printf(" ( ");
		PrintExp(GetCallActualArg(exp));
		printf(" ) ");
		break;

	case ConstExp:
		printf(" %d ", ExpInteger(exp));
		break;

	case IdentifierExp:
		printf(" %s ", ExpIdentifier(exp));
		break;

	case CompoundExp:
		PrintExp(ExpLHS(exp));
		printf(" %c ", ExpOperator(exp));
		PrintExp(ExpRHS(exp));
		break;

	default:
		Error("Expression could not be printed - unknown type\n");
	}
}

void PrintValue(valueADT value){
	
	if (ValueType(value) == IntValue)
		printf("Int value: %d\n", GetIntValue(value));
	else{
		printf("Function with one argument\n");
	}
}



