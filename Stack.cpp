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

int PrintStack__ (Stack* stack);

int StackTest__ (Stack* stack);

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

	stack->data = StackResize(stack);

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

	stack->data = StackResize(stack);

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

// int PrintStack__ (Stack* stack)
// {
// 	if(stack == NULL)
// 		return STK_IS_NULL_PTR;

// 	printf("capacity = %ld, size = %ld\n\n", stack->capacity, stack->size);

// 	for(size_t i = 0; i < stack->size; i++)
// 		printf("%d ", stack->data[i]);
// 	printf("\n\n");

// 	return 0;
// }

int StackTest__ (Stack *stack)
{
	if(stack == NULL)
		return STK_IS_NULL_PTR;

	FILE* fp = fopen("Test.txt", "w");

 	int a = 0, n = 10;

 	fprintf(fp, "\npush and pop\n");

 	for (int i = 0; i < n; i++) //Пуш и поп n элементов
 	{
 		StackPush(stack, i);
 		StackPop(stack, &a);
 		fprintf(fp, "capacity: %ld, size: %ld; ", stack->capacity, stack->size);
 		fprintf(fp, "#%d = %d\n", i + 1, a);
 	}

  	fprintf(fp, "\npush and pop in problem size\n");

 	for (int i = 0; i < n; i++) //Пуш n элементов подряд
 	{
 		StackPush(stack, i);
 		fprintf(fp, "capacity: %ld, size: %ld;\n", stack->capacity, stack->size);		
 	}

  	for (int i = 0; i < n; i++) //Пуш и поп n элементов
 	{
 		StackPush(stack, i);
 		StackPop(stack, &a);
 		fprintf(fp, "capacity: %ld, size: %ld; ", stack->capacity, stack->size);
 		fprintf(fp, "#%d = %d\n", i + 1, a);
 	}


   	fprintf(fp, "\npop\n");
 	for (int i = 0; i < n; i++) //Поп n элементов подряд
 	{
 		StackPop(stack, &a);
 		fprintf(fp, "capacity: %ld, size: %ld; ", stack->capacity, stack->size);
 		fprintf(fp, "#%d = %d\n", n - i, a);
 	}

	// StackPush(stack, 2); //2 5 10 3
	// StackPop(stack, &a);
	// StackPush(stack, 3);
	// StackPush(stack, 5);
	// StackPop(stack, &b);
	// StackPush(stack, 10);
	// StackPop(stack, &c);
	// StackPop(stack, &d);
	// StackPush(stack, 5);
	// StackPush(stack, 5);

	// printf("%d, %d, %d, %d\n", a, b, c, d);

	return 0;
}
