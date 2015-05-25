#include "value.h"
#include "symtab.h"
#include "env.h"

#define MAXRECURSION 4000

static int EvalCompound(expADT exp);
static void senseRecursion();
static bool controlExpression(char relOp, expADT expL, expADT expR, environmentADT env);

valueADT result,
	temp;
expADT myexp;

valueADT Eval(expADT exp, environmentADT env){

	string funcarg, funcname;
	valueADT value, idexpress;
	environmentADT newEnviron;
	expADT callexpress, callarg, storedBody, funcBody;

	senseRecursion();

	newEnviron = NewClosure(environment);

	switch (ExpType(exp)){

	case FuncExp:
		return Eval(GetFuncBody(exp), env);
		break;

	case IfExp:
		if(controlExpression( GetIfRelOp(exp), GetIfLHSExpression(exp), GetIfRHSExpression(exp), env)) //check to see if control exp is valid
			Eval(GetIfThenPart(exp), env);
		else
			Eval(GetIfElsePart(exp), env);
		break;

	case CallExp:
			

			callexpress = GetCallExp(exp); //  f in f(E)
			callarg = GetCallActualArg(exp);  // E in f(E)

			if (ExpType(callarg) == CompoundExp){
				value = Eval(callarg, env);
				return value;
			}
			else if (ExpType(callarg) == CallExp){
				value = Eval(callarg, env);
				return value;
			}
			else
				funcBody= GetFuncBody(callarg);  //body of E in f(E)

			funcname = ExpIdentifier(GetFuncValueBody(callexpress)); // f

			storedBody = GetIdentifierValue(env, funcname); //recall body of function with name f

			storedBody = GetFuncValueBody(storedBody); //peel off a layer
			funcarg = GetFuncValueFormalArg(storedBody);  //argument of function when defined
			DefineIdentifier(env, funcarg, funcBody, GetFuncValueClosure(storedBody)); //define argument of function in body
			return Eval(storedBody, GetFuncValueClosure(storedBody));
		
		break;

	case ConstExp:
		return NewIntegerValue(ExpInteger(exp));
		break;

	case IdentifierExp:
		printf(" %s ", ExpIdentifier(exp));
		value = GetIdentifierValue( newEnviron, ExpIdentifier(exp));
			
		return Eval(GetFuncValueBody(value), GetFuncValueClosure(value));
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


static void senseRecursion(){

	static int countRecursion = 0;

	countRecursion++;

	if (countRecursion > MAXRECURSION){
		countRecursion = 0;
		Error("\n\nToo deep recursion... cannot continue calculation.\n");
	}

}

static bool controlExpression(char relOp, expADT expL, expADT expR, environmentADT env){

	valueADT leftV, rightV;

	leftV = Eval(expL, env);
	rightV = Eval(expR, env);

	if(ValueType(leftV) == IntValue && ValueType(rightV) == IntValue ){

		switch(relOp){

		case '<':
		return (GetIntValue(leftV) < GetIntValue(rightV));

		case '>':
			return (GetIntValue(leftV) > GetIntValue(rightV));

		case '=':
			return (GetIntValue(leftV) == GetIntValue(rightV));

		default:
			Error("Reloperator %c is not valid.\n", relOp);
			break;
		}
	}
	else
		Error("\nCompared expressions is not Integers\n");

}