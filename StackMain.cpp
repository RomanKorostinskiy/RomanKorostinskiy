#include "Stack.h"

int main ()
{
	Stack stack = {};

	int error_code = 0;

	StackCtor(&stack, &error_code);

	StackTestInt(&stack, &error_code);

	StackDtor(&stack, &error_code);

	return 0;
}