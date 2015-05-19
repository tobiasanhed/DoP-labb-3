#include "value.h"
#include "exp.h"

void PrintExp(expADT exp){
	
	
	switch (ExpType(exp)){

	case FuncExp:
		printf("Function with one argument.\n");
		break;

	case IfExp:
		printf("IF-expression.\n");
		break;

	case CallExp:
		printf("Function call.\n");
		break;

	case ConstExp:
		printf("Integer.\n");
		break;

	case IdentifierExp:
		printf("Variable.\n");
		break;

	case CompoundExp:
		printf("Compound expression.\n");
		break;

	default:
		;
	}
}
void PrintValue(valueADT value){
	
	if (ValueType(value) == IntValue)
		printf("Int value: &d\n", GetIntValue(value));
	else{
		//environmentADT GetFuncValueClosure(valueADT value);
		printf("%s\n", GetFuncValueFormalArg(value));
		//expADT         GetFuncValueBody(valueADT value); print exp
	}
}



