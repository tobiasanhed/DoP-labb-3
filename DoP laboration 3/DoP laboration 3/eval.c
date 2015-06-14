#include "value.h"
#include "symtab.h"
#include "env.h"
#include "eval.h"

static int EvalCompound(expADT exp, environmentADT env);
static bool controlExpression(char relOp, expADT expL, expADT expR, environmentADT env);

valueADT result,
	temp;
expADT myexp;

valueADT Eval(expADT exp, environmentADT env){

	string funcarg, funcname;
	valueADT value, valueCallExpress, valueCallArg, valueBody;
	environmentADT newEnviron;
	expADT callexpress;

	senseRecursion(FALSE);


	switch (ExpType(exp)){

	case FuncExp:
		return NewFuncValue(GetFuncFormalArg(exp), GetFuncBody(exp), NewClosure(env));
		break;

	case IfExp:
		if (controlExpression(GetIfRelOp(exp), GetIfLHSExpression(exp), GetIfRHSExpression(exp), NewClosure(env))) //check to see if control exp is valid
			return Eval(GetIfThenPart(exp), NewClosure(env));
		else
			return Eval(GetIfElsePart(exp), NewClosure(env));
		break;
	
	case CallExp:
		callexpress = GetCallExp(exp);

		valueCallArg = GetCallActualArg(exp); // argument for f
		valueCallExpress = Eval(callexpress, env);

		if (ValueType(valueCallExpress) == FuncValue){
			//behöver inte skapa ny miljö utan ist hämta den som finns i funcvaluet.
			newEnviron = GetFuncValueClosure(valueCallExpress);

			funcarg = GetFuncValueFormalArg(valueCallExpress);

			DefineIdentifier(newEnviron, funcarg, valueCallArg, env); //define argument of function in body
			
			return Eval(GetFuncValueBody(valueCallExpress), newEnviron);
		}
		
		return valueCallExpress;
		
		break;

	case ConstExp:
		return NewIntegerValue(ExpInteger(exp));
		break;

	case IdentifierExp:
		//printf(" %s ", ExpIdentifier(exp));
		value = GetIdentifierValue(env, ExpIdentifier(exp));
		return Eval(GetFuncValueBody(value), GetFuncValueClosure(value));
		break;

	case CompoundExp:
		return NewIntegerValue(EvalCompound(exp, env));
		break;

	default:
		break;
	}
}



static int EvalCompound(expADT exp, environmentADT env){
    char op;
    int lhs, rhs;
	valueADT lValue, rValue;

    op = ExpOperator(exp);

	lValue = Eval(ExpLHS(exp), NewClosure(env));
    rValue = Eval(ExpRHS(exp), NewClosure(env));
	
	lhs = GetIntValue(lValue);
	rhs = GetIntValue(rValue);
		


    switch (op) {
      case '+': return (lhs + rhs);
      case '-': return (lhs - rhs);
      case '*': return (lhs * rhs);
	  case '/': if (rhs == 0) Error("Division by zero\n"); else return (lhs / rhs);
      default:  Error("Illegal operator");
    }
}


void senseRecursion(bool resetCounter){

	static int countRecursion = 0;

	countRecursion++;

	if (resetCounter == TRUE)
		countRecursion = 0;

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