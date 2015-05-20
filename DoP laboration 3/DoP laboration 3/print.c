#include "value.h"
#include "exp.h"

void PrintExp(expADT exp){
	
	switch (ExpType(exp)){

	case FuncExp:
		printf(" func ");
		printf("(%s) ", GetFuncFormalArg(exp));
		printf(" { ");
		PrintExp(GetFuncBody(exp));
		printf(" } ");
 		//printf("Function with one argument.\n");
		break;

	case IfExp:
		//printf("IF-expression.\n");
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
		//printf("Function call.\n");
		break;

	case ConstExp:
		printf(" %d ", ExpInteger(exp));
		//printf("Integer.\n");
		break;

	case IdentifierExp:
		printf(" %s ", ExpIdentifier(exp));
		//printf("Variable.\n");
		break;

	case CompoundExp:
		//printf("Compound expression.\n");
		PrintExp(ExpLHS(exp));
		printf(" %c ", ExpOperator(exp));
		PrintExp(ExpRHS(exp));
		
		break;

	default:
		;
	}
}

void PrintValue(valueADT value){
	
	if (ValueType(value) == IntValue)
		printf("Int value: %d\n", GetIntValue(value));
	else{
		//environmentADT GetFuncValueClosure(valueADT value);
		printf("%s\n", GetFuncValueFormalArg(value));
		//expADT         GetFuncValueBody(valueADT value); print exp
	}
}



