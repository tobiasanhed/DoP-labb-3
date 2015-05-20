#include "value.h"

valueADT Eval(expADT exp, environmentADT env){
	
	switch (ExpType(exp)){

	case FuncExp:
		printf(" func ");
		printf("(%s) ", GetFuncFormalArg(exp));
		printf(" { ");
		Eval(GetFuncBody(exp), env);
		printf(" } ");
		break;

	case IfExp:
		//printf("IF-expression.\n");
		printf("if ");
		Eval(GetIfLHSExpression(exp), env);
		printf(" %c ", GetIfRelOp(exp));
		Eval(GetIfRHSExpression(exp), env);
		printf(" then ");
		Eval(GetIfThenPart(exp), env);
		printf(" else ");
		Eval(GetIfElsePart(exp), env);
		break;

	case CallExp:
		Eval(GetCallExp(exp), env);
		printf(" ( ");
		Eval(GetCallActualArg(exp),env);
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
		Eval(ExpLHS(exp), env);
		printf(" %c ", ExpOperator(exp));
		Eval(ExpRHS(exp), env);
		
		break;

	default:
		;
	}
}