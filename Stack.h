#ifndef Stack_h
#define Stack_h

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

typedef double data_t;

extern const int *UNAVAILABLE_ADR;
extern const int START_CAPACITY;

typedef struct Stack_t
{
	int capacity = 0;
	data_t* data = nullptr;
	int size = 0;
} Stack;

int StackCtor (Stack* stack, int capacity = START_CAPACITY);

int StackDtor (Stack* stack);

int StackPush (Stack* stack, int value);

int StackPop (Stack* stack);

data_t* StackResize (Stack* stack);

int StackTest__ (Stack* stack);

int StackError (Stack* stack);

#endif