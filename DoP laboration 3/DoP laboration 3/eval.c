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
	expADT callexpress, ifCallExp;

	senseRecursion(FALSE);

	//newEnviron = NewClosure(environment);

	switch (ExpType(exp)){

	case FuncExp:
		return NewFuncValue("", GetFuncBody(exp), NewClosure(env));
		//return Eval(GetFuncBody(exp), env);
		break;

	case IfExp:
		if (controlExpression(GetIfRelOp(exp), GetIfLHSExpression(exp), GetIfRHSExpression(exp), NewClosure(env))) //check to see if control exp is valid
			return Eval(GetIfThenPart(exp), NewClosure(env));
		else
			return Eval(GetIfElsePart(exp), NewClosure(env));
		break;
		/*if (controlExpression(GetIfRelOp(exp), GetIfLHSExpression(exp), GetIfRHSExpression(exp), env)) //check to see if control exp is valid
			return Eval(GetIfThenPart(exp), env);
		else
			return Eval(GetIfElsePart(exp), env);
		break;
		*/
	case CallExp:
		newEnviron = NewClosure(env);
		callexpress = GetCallExp(exp);
		valueCallExpress = Eval(callexpress, env);
		valueCallArg = Eval(GetCallActualArg(exp), env); // argument for f
		funcname = ExpIdentifier(GetFuncValueBody(valueCallExpress)); //f
		
		valueBody = GetIdentifierValue(GetFuncValueClosure(valueCallExpress), funcname); //recall body of function with name f
		funcarg = GetFuncFormalArg(GetFuncValueBody(valueBody));
		
		if (1){//ValueType(valueCallArg) == IntValue){

			while(ValueType(valueCallArg) != IntValue)
				valueCallArg = Eval(GetFuncValueBody(valueCallArg), newEnviron);

			DefineIdentifier(env, funcarg, NewFuncExp("", NewIntegerExp(GetIntValue(valueCallArg))), newEnviron);
			printf("%s: %d\n", __FUNCTION__, GetIntValue(valueCallArg));
			valueCallExpress = Eval(GetFuncValueBody(valueBody), newEnviron);
			//Eval(GetFuncValueBody(valueCallExpress), GetFuncValueClosure(valueCallExpress));
			
			if(ExpType(ifCallExp = GetFuncValueBody(valueCallExpress)) == IfExp){
				if (controlExpression(GetIfRelOp(ifCallExp), GetIfLHSExpression(ifCallExp), GetIfRHSExpression(ifCallExp), newEnviron)){
					value = Eval(GetIfThenPart(ifCallExp), newEnviron);
					DefineIdentifier(env, funcarg, GetFuncValueBody(value), newEnviron);
					return value;
				}
			}
		}
		else{
			DefineIdentifier(env, funcarg, GetFuncValueBody(valueCallArg), newEnviron);//GetFuncValueClosure(valueCallArg)); //define argument of function in body
			valueCallExpress = Eval(GetFuncValueBody(valueBody), newEnviron);//GetFuncValueClosure(valueBody));
		}

		
		return Eval(GetFuncValueBody(valueCallExpress), GetFuncValueClosure(valueCallExpress));//GetFuncValueClosure(valueCallExpress));
		//funcarg = GetFuncValueFormalArg(Eval(GetFuncValueBody(valueBody), GetFuncValueClosure(valueBody)));  //argument of function when defined
		
		//return Eval(GetFuncValueBody(valueCallExpress), GetFuncValueClosure(valueCallExpress));
			/*callexpress = GetCallExp(exp); //  f in f(E)
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
			*/
		break;

	case ConstExp:
		return NewIntegerValue(ExpInteger(exp));
		break;

	case IdentifierExp:
		printf(" %s ", ExpIdentifier(exp));
		//value = GetIdentifierValue( newEnviron, ExpIdentifier(exp));
		value = GetIdentifierValue(env, ExpIdentifier(exp));
	    //return value;
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

	//lhs = GetIntValue(Eval(ExpLHS(exp), env));
	//rhs = GetIntValue(Eval(ExpRHS(exp), env));
	lValue = Eval(ExpLHS(exp), env);
    rValue = Eval(ExpRHS(exp), env);

	while (ValueType(lValue) == FuncValue)
		lValue = Eval(GetFuncValueBody(lValue), GetFuncValueClosure(lValue));
	
	lhs = GetIntValue(lValue);

	while (ValueType(rValue) == FuncValue)
		rValue = Eval(GetFuncValueBody(rValue), GetFuncValueClosure(rValue));
		
	rhs = GetIntValue(rValue);
		

	/*if (ValueType(lValue) == IntValue)
		lhs = GetIntValue(lValue);
	else
		Eval(GetFuncValueBody(lValue), GetFuncValueClosure(lValue));

	if (ValueType(rValue) == IntValue)
		rhs = GetIntValue(rValue);
	else
		Eval(GetFuncValueBody(rValue), GetFuncValueClosure(rValue));
		*/
	//lhs = GetIntValue(lfuncValue);
	//rhs = GetIntValue(rfuncValue);
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

	while (ValueType(leftV) == FuncValue)
		leftV = Eval(GetFuncValueBody(leftV), GetFuncValueClosure(leftV));

	while (ValueType(rightV) == FuncValue)
		rightV = Eval(GetFuncValueBody(rightV), GetFuncValueClosure(rightV));

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