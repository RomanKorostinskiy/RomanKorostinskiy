#include "Stack.h"

int main ()
{
	Stack stack = {};

	StackCtor(&stack);

	StackTest__(&stack);

	StackDtor(&stack);

	return 0;
}