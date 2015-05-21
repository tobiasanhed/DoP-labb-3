#include "value.h"
#include "symtab.h"
#include "env.h"

static int EvalCompound(expADT exp);

//static symtabADT variableTable;


valueADT result,
	temp;
expADT myexp;

valueADT Eval(expADT exp, environmentADT env){
	string funcarg, funcname;
	valueADT value, idexpress;
	environmentADT newEnviron;
	expADT callexpress, callarg, storedBody, funcBody;

	switch (ExpType(exp)){

	case FuncExp:
		//GetIdentifierValueE();
		return Eval(GetFuncBody(exp), env);//NewIntegerValue(EvalCompound(exp, env));//NewFuncValue(GetFuncFormalArg(exp), GetFuncBody(exp), env);
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
		
		if (ExpType(GetCallActualArg(exp)) == CompoundExp){
			result = NewIntegerValue(EvalCompound(GetCallActualArg(exp), env));
		}
		else{
			newEnviron = NewClosure(env);

			callexpress = GetCallExp(exp); //  f in f(E)
			callarg = GetCallActualArg(exp);  // E in f(E)

			if (ExpType(callarg) == CallExp){
				value = Eval(callarg, env);
				funcBody = NewFuncExp("", NewIntegerExp( GetIntValue(value)));
			}
			else
				funcBody= GetFuncBody(callarg);  //body of E in f(E)

			funcname = ExpIdentifier(callexpress); // f

			storedBody = (expADT)GetIdentifierValue(env, funcname); //recall body of function with name f

			storedBody = GetFuncValueBody(storedBody); //peel off a layer
			funcarg = GetFuncValueFormalArg(storedBody);  //argument of function when defined
			DefineIdentifier(env, funcarg, funcBody, newEnviron); //define argument of function in body
			return Eval(storedBody, env);
		}
		break;

	case ConstExp:
		return NewIntegerValue(ExpInteger(exp));
		break;

	case IdentifierExp:
		printf(" %s ", ExpIdentifier(exp));
		value = GetIdentifierValue(environment, ExpIdentifier(exp));
			
		return Eval(GetFuncValueBody(value), env);
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