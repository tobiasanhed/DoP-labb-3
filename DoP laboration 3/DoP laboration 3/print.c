#include "value.h"

void PrintExp(expADT exp){

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
