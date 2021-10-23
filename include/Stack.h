#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

// #define DEBUG //#ifndef #endif

enum Stack_errors
{
	STK_IS_NULL_PTR  = 1,
	DATA_IS_NULL_PTR = 2,
	STK_DESTROYED    = 4,
	STK_OVERFL       = 8,
	STK_UNDERFL      = 16,
	STK_DOUBLE_CTED  = 32,
    STRCT_CANARY_BAD = 64,
    DATA_CANARY_BAD  = 128,
    HASH_BAD         = 256,
};

#define INT_DATA

#ifdef FLOAT_DATA
typedef double data_t;
#endif

#ifdef INT_DATA
typedef int data_t;
#endif

typedef size_t canary_t;
typedef size_t hash_t;


extern const int    START_CAPACITY;
extern const int    CAPACITY_STEP;
extern const size_t CANARY_CONSTANT;
extern const data_t*   UNAVAILABLE_ADR;
extern const size_t DATA_SHIFT;

typedef struct Stack_t
{
    canary_t canary_left = 0;

	int      capacity = 0;
	data_t*  data = nullptr;
	int      size = 0;

    hash_t   hash = 0;

    canary_t canary_right = 0;
} Stack;

#define STACK_CTOR_CHECK                                \
    *errors = 0;							            \
                                                        \
	StackCtorCheck(stack, errors);				        \
												        \
	StackDump(stack, *errors, __FILE__, __FUNCTION__);	\
												        \
	if (*errors != 0)							        \
	{											        \
		return 1;								        \
	}											        \

#define STACK_ERROR_CHECK                               \
    *errors = 0;                                        \
                                                        \
	StackErrorCheck(stack, errors);				        \
												        \
	StackDump(stack, *errors, __FILE__, __FUNCTION__);	\
												        \
	if (*errors != 0)							        \
	{											        \
		return 1;					                    \
	}										            \

#define STACK_RESIZE_ERROR_CHECK                        \
    *errors = 0;                                        \
                                                        \
	StackErrorCheck(stack, errors);				        \
												        \
	StackDump(stack, *errors, __FILE__, __FUNCTION__);	\
												        \
	if (*errors != 0)							        \
	{											        \
		return nullptr;					                \
	}                                                   \

#define STACK_POP_ERROR_CHECK                           \
    *errors = 0;                                        \
                                                        \
	StackPopCheck(stack, errors);				        \
												        \
	StackDump(stack, *errors, __FILE__, __FUNCTION__);	\
												        \
	if (*errors != 0)							        \
	{											        \
		return 0xBEDABEDA;					            \
	}                                                   \

#define STACK_DTOR_ERROR_CHECK                          \
    *errors = 0;                                        \
                                                        \
	StackDtorCheck(stack, errors);				        \
												        \
	StackDump(stack, *errors, __FILE__, __FUNCTION__);	\
                                                        \
	if (*errors != 0)							        \
	{											        \
		return 1;					                    \
	}                                                   \

int StackCtor (Stack* stack, int* errors, int capacity = START_CAPACITY);

int StackDtor (Stack* stack, int* errors);

int StackPush (Stack* stack, data_t value, int* errors);

data_t StackPop (Stack* stack, int* errors);

data_t* StackResize (Stack* stack, int* errors);

size_t StackHash (Stack* stack);

int StackErrorCheck (Stack* stack, int* errors);

int StackCtorCheck(Stack* stack, int* errors);

int StackDtorCheck (Stack* stack, int* errors);

int StackPopCheck (Stack* stack, int* errors);

void StackDump (Stack* stack, int errors, const char* current_file, const char* current_function);

int StackTestInt (Stack* stack, int* errors);

int StackTestFloat (Stack* stack, int* errors);

void StackStructHack (Stack* stack);

void StackDataHack (Stack* stack);