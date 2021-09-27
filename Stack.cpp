#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Errors
{
	STK_IS_NULL_PTR,
	STK_OVERFL,
};

const int *UNAVAILABLE_ADR = (int*) 1;
const size_t START_CAPACITY = 4;

typedef struct Stack
{
	size_t capacity;
	int* data;
	size_t size;
} Stack;

int StackCtor (Stack* stack);

int StackDtor (Stack* stack);

int StackPuch (Stack* stack, int value);

int StackPop (Stack* stack, int* value);

int PrintStack__ (Stack* stack);

int StackTest__ (Stack* stack);

int main ()
{
	Stack stack = {};

	StackCtor(&stack);

	StackTest__(&stack);

	PrintStack__(&stack);

	StackDtor(&stack);

	return 0;
}

int StackCtor (Stack* stack)
{
	if(stack == NULL)
		return STK_IS_NULL_PTR;

	stack->capacity = START_CAPACITY;

	stack->data = (int*) calloc (stack->capacity, sizeof(int));

	stack->size = 0;

	return 0;
}

int StackDtor (Stack* stack)
{
	if(stack == NULL)
		return STK_IS_NULL_PTR;

	memset (stack->data, 0xF0F0F0F0, stack->capacity);
	free(stack->data);

	stack->size = -1;

	stack->data = (int*) UNAVAILABLE_ADR;

	return 0;
}

int StackPuch (Stack* stack, int value)
{
	if(stack == NULL)
		return STK_IS_NULL_PTR;

	if(stack->size >= stack->capacity)
		return STK_OVERFL;

	stack->data[stack->size++] = value;

	return 0;
}

int StackPop (Stack* stack, int* value)
{
	if(stack == NULL)
		return STK_IS_NULL_PTR;

	*value = stack->data[--stack->size];

	stack->data[stack->size] = 0;

	return 0;
}

int PrintStack__ (Stack* stack)
{
	if(stack == NULL)
		return STK_IS_NULL_PTR;

	printf("capacity = %ld, size = %ld\n\n", stack->capacity, stack->size);

	for(size_t i = 0; i < stack->size; i++)
		printf("%d ", stack->data[i]);
	printf("\n\n");

	return 0;
}

int StackTest__ (Stack *stack)
{
	if(stack == NULL)
		return STK_IS_NULL_PTR;

 	int a, b, c, d;

	StackPuch(stack, 2); //2 5 10 3
	StackPop(stack, &a);
	StackPuch(stack, 3);
	StackPuch(stack, 5);
	StackPop(stack, &b);
	StackPuch(stack, 10);
	StackPop(stack, &c);
	StackPop(stack, &d);
	StackPuch(stack, 5);

	printf("%d, %d, %d, %d\n", a, b, c, d);

	return 0;
}
