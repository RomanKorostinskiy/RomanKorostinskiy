#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Errors
{
	STK_IS_NULL_PTR,
	STK_OVERFL,
	STK_UNDERFL,
	STK_DESTROYED,
};

const int *UNAVAILABLE_ADR = (int*) 1;
const size_t START_CAPACITY = 1;

typedef struct Stack
{
	size_t capacity;
	int* data;
	size_t size;
} Stack;

int StackCtor (Stack* stack);

int StackDtor (Stack* stack);

int StackPush (Stack* stack, int value);

int StackPop (Stack* stack, int* value);

int* StackResize (Stack* stack);

int StackTest__ (Stack* stack);

int StackError (Stack* stack);

int main ()
{
	Stack stack = {};

	StackCtor(&stack);

	StackTest__(&stack);

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

	if(stack->size == -1)
		return STK_DESTROYED;

	memset (stack->data, 0xF0F0F0F0, stack->capacity);
	free(stack->data);

	stack->size = -1;

	stack->data = (int*) UNAVAILABLE_ADR;

	return 0;
}

int StackPush (Stack* stack, int value)
{
	if(stack == NULL)
		return STK_IS_NULL_PTR;

	if(stack->size == -1)
		return STK_DESTROYED;

	if (stack->size >= stack->capacity)
	{
		stack->data = StackResize(stack);
	}

	stack->data[stack->size++] = value;

	return 0;
}

int StackPop (Stack* stack, int* value)
{
	if(stack == NULL)
		return STK_IS_NULL_PTR;

	if(stack->size == -1)
		return STK_DESTROYED;

	if(stack->size <= 0)
		return 	STK_UNDERFL;
	
	if (stack->size * 2 == stack->capacity)
	{
		stack->data = StackResize(stack);
	}

	*value = stack->data[--stack->size];

	stack->data[stack->size] = 0;

	return 0;
}

int* StackResize (Stack* stack)
{
	// if(stack == NULL)
	// 	return STK_IS_NULL_PTR;

	// if(stack->size == -1)
	// 	return STK_DESTROYED;

	int* new_adress = stack->data;

	if (stack->size >= stack->capacity)
	{
		stack->capacity *= 2;

		new_adress = (int*) realloc(stack->data, 
			(stack->capacity) * sizeof(int));
	}
	else if (stack->size * 2 == stack->capacity) //TODO проблемка
	{
		stack->capacity /= 2;

		new_adress = (int*) realloc(stack->data, 
			(stack->capacity) * sizeof(int));
	}

	return new_adress;
}

int StackTest__ (Stack *stack)
{
	if(stack == NULL)
		return STK_IS_NULL_PTR;

	FILE* fp = fopen("Test.txt", "w");

	int element = 0;
	int n_of_tests = 10, prblm_sz = 5;

	fprintf(fp, "\n\n\tPush and Pop\n\n");

	for (int i = 0; i < n_of_tests; i++) //Пуш и поп n элементов около size = 0
 	{
		fprintf(fp, "\n#%d\n", i + 1);
		fprintf(fp, "Before Actions | capacity: %ld, size: %ld;\n", 
			stack->capacity, stack->size);

		StackPush(stack, i);
		fprintf(fp, "After Push     | capacity: %ld, size: %ld;\n", 
			stack->capacity, stack->size);

		StackPop(stack, &element);
		fprintf(fp, "After Pop      | capacity: %ld, size: %ld;\n", 
			stack->capacity, stack->size);

		fprintf(fp, "Push Elem = %d , Pop Elem = %d\n", i, element);
	}

  	fprintf(fp, "\n\n\tPush and Pop in problem size\n\n");

 	for (int i = 0; i < prblm_sz; i++) //Пуш элементов до problem size
 	{
 		StackPush(stack, i);	
 	}

  	for (int i = 0; i < n_of_tests; i++) //Пуш и поп n элементов около problem size
 	{
		fprintf(fp, "\n#%d\n", i + 1);
 		fprintf(fp, "Before Actions | capacity: %ld, size: %ld;\n",
  			stack->capacity, stack->size);

 		StackPush(stack, i);
  		fprintf(fp, "After Push     | capacity: %ld, size: %ld;\n", 
  			stack->capacity, stack->size);

		StackPop(stack, &element);
		fprintf(fp, "After Pop      | capacity: %ld, size: %ld;\n", 
			stack->capacity, stack->size);

		fprintf(fp, "#%d = %d\n", i + 1, element);
 	}


   	fprintf(fp, "\n\n\tPop all\n\n");
 	for (int i = 0; i < n_of_tests; i++) //Поп n элементов подряд
	{
		fprintf(fp, "\n#%d\n", i + 1);
		fprintf(fp, "Before Actions | capacity: %ld, size: %ld;\n", 
			stack->capacity, stack->size);

		StackPop(stack, &element);
		fprintf(fp, "After Pop      | capacity: %ld, size: %ld;\n", 
			stack->capacity, stack->size);

		fprintf(fp, "capacity: %ld, size: %ld; ", stack->capacity, stack->size);
		fprintf(fp, "#%d = %d\n", n_of_tests - i, element);
 	}

	return 0;
}

int StackError (Stack* stack)
{
	return 0;
}