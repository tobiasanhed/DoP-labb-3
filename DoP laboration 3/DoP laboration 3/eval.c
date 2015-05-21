#include "value.h"
#include "symtab.h"
#include "env.h"

static int EvalCompound(expADT exp);

//static symtabADT variableTable;


valueADT result,
	temp;
expADT myexp;

valueADT Eval(expADT exp, environmentADT env){
	string funcarg;

	switch (ExpType(exp)){

	case FuncExp:
		//GetIdentifierValueE();
		return NewFuncValue(GetFuncFormalArg(exp), GetFuncBody(exp), env);
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
		
		//myexp = NewCallExp(GetCallExp(exp), GetCallActualArg(exp));
		
		if (ExpType(GetCallActualArg(exp)) == CompoundExp){
			result = NewIntegerValue(EvalCompound(GetCallActualArg(exp), env));
			funcarg = GetFuncFormalArg(exp);
			//temp = Eval(GetFuncBody(exp), env);
		}
		else{
			result = Eval(GetCallActualArg(exp), env);
			funcarg = GetFuncFormalArg(exp);
			//temp = Eval(GetFuncBody(exp), env);
		}
		break;

	case ConstExp:
		return NewIntegerValue(ExpInteger(exp));
		break;

	case IdentifierExp:
		printf(" %s ", ExpIdentifier(exp));
		return (GetIdentifierValue(environment, ExpIdentifier(exp)));
		break;

	case CompoundExp:
		return NewIntegerValue(EvalCompound(exp, env));
		break;

	default:
		break;
	}
}



static int EvalCompound(expADT exp, environmentADT env)
{
    char op;
    int lhs, rhs;

    op = ExpOperator(exp);
    /*if (op == '=') {
        rhs = GetIntValue(Eval(ExpRHS(exp), env));
        DefineValue(ExpIdentifier(ExpLHS(exp)), rhs);
        return (rhs);
    }*/
    lhs = GetIntValue(Eval(ExpLHS(exp), env));
    rhs = GetIntValue(Eval(ExpRHS(exp), env));
    switch (op) {
      case '+': return (lhs + rhs);
      case '-': return (lhs - rhs);
      case '*': return (lhs * rhs);
      case '/': return (lhs / rhs);
      default:  Error("Illegal operator");
    }
}