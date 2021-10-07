#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

// #define DEBUG //#ifndef #endif

enum Errors
{
	STK_IS_NULL_PTR  = 1,
	STK_OVERFL       = 2,
	STK_UNDERFL      = 3,
	STK_DESTROYED    = 4,
	DATA_IS_NULL_PTR = 5,
};

typedef int data_t;

const int *UNAVAILABLE_ADR = (int*) 1;
const int START_CAPACITY = 1;

typedef struct Stack_t
{
	int capacity = 0;
	data_t* data = nullptr;
	int size = 0;
} Stack;

int StackCtor (Stack* stack, int capacity = START_CAPACITY);

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

int StackCtor (Stack* stack, int capacity) //TODO аргумент по умолчанию
{

	stack->capacity = capacity;

	stack->data = (int*) calloc (capacity, sizeof(int)); //TODO проверка возвращаемого значения calloc

	stack->size = 0;

	return 0;
}

int StackDtor (Stack* stack)
{
	if(stack == NULL)
		return STK_IS_NULL_PTR;

	// if(stack->size == -1)
	// 	return STK_DESTROYED; //проверется в error

	memset (stack->data, 0xF0F0F0F0, stack->capacity);
	free(stack->data);

	stack->size = -1;

	stack->data = (int*) UNAVAILABLE_ADR;

	return 0;
}

int StackPush (Stack* stack, int value)
{
	if(stack == nullptr)
		return STK_IS_NULL_PTR;

	if (stack->size == stack->capacity)
	{
		stack->data = StackResize(stack);
	}

	stack->data[stack->size++] = value;

	return 0;
}

int StackPop (Stack* stack, int* value)
{
	assert(value);

	if(!stack) //
		return STK_IS_NULL_PTR;

	if(stack->size == -1)
		return STK_DESTROYED;

	if(stack->size <= 0)
		return 	STK_UNDERFL;
	
	if (stack->size * 2 == stack->capacity) //TODO придумать константу вместо 2
	{
		stack->data = StackResize(stack);
	}

	*value = stack->data[--stack->size];

	stack->data[stack->size] = 0;

	return 0;
}

int* StackResize (Stack* stack)
{
	//проверки

	int* new_adress = stack->data;

	if (stack->size >= stack->capacity)
	{
		stack->capacity *= 2;

		new_adress = (int*) realloc(stack->data, //TODO проверка возвращаемого значения realloc
			(stack->capacity) * sizeof(int));
	}
	else if (stack->size * 2 == stack->capacity)
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
	int n_of_tests = 10, prblm_sz = 8;

	fprintf(fp, "\n\n\tPush and Pop\n\n");

	for (int i = 0; i < n_of_tests; i++) //Пуш и поп n элементов около size = 0
 	{
		fprintf(fp, "\n#%d\n", i + 1);
		fprintf(fp, "Before Actions | capacity: %d, size: %d;\n", 
			stack->capacity, stack->size);

		StackPush(stack, i);
		fprintf(fp, "After Push     | capacity: %d, size: %d;\n", 
			stack->capacity, stack->size);

		StackPop(stack, &element);
		fprintf(fp, "After Pop      | capacity: %d, size: %d;\n", 
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
 		fprintf(fp, "Before Actions | capacity: %d, size: %d;\n",
  			stack->capacity, stack->size);

 		StackPush(stack, i);
  		fprintf(fp, "After Push     | capacity: %d, size: %d;\n", 
  			stack->capacity, stack->size);

		StackPop(stack, &element);
		fprintf(fp, "After Pop      | capacity: %d, size: %d;\n", 
			stack->capacity, stack->size);

		fprintf(fp, "Push Elem = %d , Pop Elem = %d\n", i, element);
 	}


   	fprintf(fp, "\n\n\tPop all\n\n");
 	for (int i = 0; i < n_of_tests; i++) //Поп n элементов подряд
	{
		fprintf(fp, "\n#%d\n", i + 1);
		fprintf(fp, "Before Actions | capacity: %d, size: %d;\n", 
			stack->capacity, stack->size);

		StackPop(stack, &element);
		fprintf(fp, "After Pop      | capacity: %d, size: %d;\n", 
			stack->capacity, stack->size);

		fprintf(fp, "Pop Elem = %d\n", element);
 	}

	return 0;
}

int StackError (Stack* stack)
{
	if (!stack)
		return STK_IS_NULL_PTR;

	if (!stack->data)
		return DATA_IS_NULL_PTR;

	if (stack->size == -1)
		return STK_DESTROYED;

	if(stack->size < 0)
		return 	STK_UNDERFL;

	if(stack->size > stack->capacity)
		return 	STK_OVERFL;

	return 0;
}