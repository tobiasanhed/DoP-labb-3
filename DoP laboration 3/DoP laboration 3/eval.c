#include "value.h"
#include "symtab.h"

static int EvalCompound(expADT exp);

static symtabADT variableTable;

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
		return NewIntegerValue(ExpInteger(exp));
		//printf(" %d ", ExpInteger(exp));
		//printf("Integer.\n");
		break;

	case IdentifierExp:
		printf(" %s ", ExpIdentifier(exp));
		return NewIntegerValue(GetIdentifierValueE(ExpIdentifier(exp)));
		//printf("Variable.\n");
		break;

	case CompoundExp:
		//printf("Compound expression.\n");
		//Eval(ExpLHS(exp), env);
		//printf(" %d ", EvalCompound(exp, env));
		return NewIntegerValue(EvalCompound(exp, env));
		//printf(" %c ", ExpOperator(exp));
		//Eval(ExpRHS(exp), env);
		
		break;

	default:
		;
	}
}

void InitVariableTable(void)
{
    variableTable = NewSymbolTable();
}

int GetIdentifierValueE(string name)
{
    int *ip;

    ip = Lookup(variableTable, name);
    if (ip == UNDEFINED)  Error("%s is undefined", name);
    return (*ip);
}

void SetIdentifierValue(string name, int value)
{
    int *ip;

    ip = New(int *);
    *ip = value;
    Enter(variableTable, name, ip);
}

static int EvalCompound(expADT exp, environmentADT env)
{
    char op;
    int lhs, rhs;

    op = ExpOperator(exp);
    if (op == '=') {
        rhs = GetIntValue(Eval(ExpRHS(exp), env));
        SetIdentifierValue(ExpIdentifier(ExpLHS(exp)), rhs);
        return (rhs);
    }
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