#ifndef Stack_h
#define Stack_h

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
};

typedef int    data_t;
typedef size_t canary_t;
typedef size_t hash_t;


extern const int    START_CAPACITY;
extern const int    CAPACITY_STEP;
extern const size_t CANARY_CONSTANT;
extern const int*   UNAVAILABLE_ADR;

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
	StackCtorCheck(stack, errors);				        \
												        \
	StackDump(stack, *errors, __FILE__, __FUNCTION__);	\
												        \
	if (*errors != 0)							        \
	{											        \
		*errors = 0;							        \
		return 1;								        \
	}											        \

#define STACK_ERROR_CHECK                               \
	StackErrorCheck(stack, errors);				        \
												        \
	StackDump(stack, *errors, __FILE__, __FUNCTION__);	\
												        \
	if (*errors != 0)							        \
	{											        \
		*errors = 0;							        \
		return 1;					                    \
	}										            \

#define STACK_RESIZE_ERROR_CHECK                        \
	StackErrorCheck(stack, errors);				        \
												        \
	StackDump(stack, *errors, __FILE__, __FUNCTION__);	\
												        \
	if (*errors != 0)							        \
	{											        \
		*errors = 0;							        \
		return nullptr;					                \
	}                                                   \

#define STACK_POP_ERROR_CHECK                           \
	StackErrorCheck(stack, errors);				        \
												        \
	StackDump(stack, *errors, __FILE__, __FUNCTION__);	\
												        \
	if (*errors != 0)							        \
	{											        \
		*errors = 0;							        \
		return 0xBEDABEDA;					            \
	}                                                   \

int StackCtor (Stack* stack, int* errors, int capacity = START_CAPACITY);

int StackDtor (Stack* stack, int* errors);

int StackPush (Stack* stack, data_t value, int* errors);

data_t StackPop (Stack* stack, int* errors);

data_t* StackResize (Stack* stack, int* errors);

int StackErrorCheck (Stack* stack, int* errors);

int StackCtorCheck(Stack* stack, int* errors);

void StackDump (Stack* stack, int errors, const char* current_file, const char* current_function);

int StackTestInt (Stack* stack, int* errors);

int StackTestFloat (Stack* stack);

#endif