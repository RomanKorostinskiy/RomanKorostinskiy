#include "../include/Stack.h"

const int    START_CAPACITY  = 1;
const int    CAPACITY_STEP   = 2;
const size_t CANARY_CONSTANT = 0xB00BA555;
const data_t*   UNAVAILABLE_ADR = (data_t*) 1;
const size_t DATA_SHIFT = sizeof(canary_t) / sizeof (data_t);

#define CANARY_DEFF
#define HASH_DEFF

int StackCtor (Stack* stack, int* errors, int capacity)
{
	STACK_CTOR_CHECK

    stack->capacity = capacity;

#ifdef CANARY_DEFF
    stack->data = (data_t*) calloc(stack->capacity*sizeof(data_t) + 2*sizeof(canary_t), //TODO проверка возвращаемого значения calloc
                                   sizeof(char));

    stack->canary_left  = CANARY_CONSTANT;
    stack->canary_right = CANARY_CONSTANT;

    ((canary_t*)stack->data)[0] = CANARY_CONSTANT;
    (stack->data) += DATA_SHIFT;
    *(canary_t*)(stack->data + stack->capacity) = CANARY_CONSTANT;
#endif

#ifndef CANARY_DEFF
    stack->data = (data_t*) calloc (stack->capacity, sizeof(data_t)); //TODO проверка возвращаемого значения calloc
    stack->size = 0;
#endif

#ifdef HASH_DEFF
    stack->hash = StackHash(stack);
#endif

    STACK_ERROR_CHECK

	return 0;
}

int StackDtor (Stack* stack, int* errors)
{
    STACK_DTOR_ERROR_CHECK

    memset (stack->data, (int)0xF0F0F0F0, stack->capacity);

#ifdef CANARY_DEFF
    stack->data -= DATA_SHIFT;
#endif

    free(stack->data);

	stack->size     = -1;
	stack->capacity = 0;
	stack->data     = (data_t*) UNAVAILABLE_ADR;

#ifdef CANARY_DEFF
    stack->canary_left  = 0;
    stack->canary_right = 0;
#endif

	return 0;
}

int StackPush (Stack* stack, data_t value, int* errors)
{
    STACK_ERROR_CHECK

    if (stack->size == stack->capacity)
    {
        stack->data = StackResize(stack, errors);
    }

    stack->data[stack->size++] = value;

#ifdef HASH_DEFF
    stack->hash = StackHash(stack);
#endif

    STACK_ERROR_CHECK

	return 0;
}

data_t StackPop (Stack* stack, int* errors)
{
    STACK_POP_ERROR_CHECK

	data_t value;
	
	if (stack->size * CAPACITY_STEP == stack->capacity)
	{
		stack->data = StackResize(stack, errors);
	}

	value = stack->data[--stack->size];

	stack->data[stack->size] = 0;

#ifdef HASH_DEFF
    stack->hash = StackHash(stack);
#endif

    STACK_ERROR_CHECK

	return value;
}

data_t* StackResize (Stack* stack, int* errors)
{
    STACK_RESIZE_ERROR_CHECK

	data_t* new_adress = stack->data;

	if (stack->size >= stack->capacity)
	{
		stack->capacity *= CAPACITY_STEP;

#ifdef CANARY_DEFF

        (stack->data) -= DATA_SHIFT;

        new_adress = (data_t*) realloc(stack->data,
                                       stack->capacity * sizeof(data_t) + 2*sizeof(canary_t));//TODO проверка возвращаемого значения realloc

        ((canary_t*)new_adress)[0] = CANARY_CONSTANT;
        new_adress += DATA_SHIFT;
        *(canary_t*)(new_adress + stack->capacity) = CANARY_CONSTANT;
#endif

#ifndef CANARY_DEFF
        new_adress = (data_t*) realloc(stack->data,
                                       (stack->capacity) * sizeof(data_t));//TODO проверка возвращаемого значения realloc
#endif
	}
	else if (stack->size * CAPACITY_STEP == stack->capacity)
	{
		stack->capacity /= CAPACITY_STEP;

#ifdef CANARY_DEFF

        (stack->data) -= DATA_SHIFT;

        new_adress = (data_t*) realloc(stack->data,
                                       stack->capacity * sizeof(data_t) + 2*sizeof(canary_t));//TODO проверка возвращаемого значения realloc

        ((canary_t*)new_adress)[0] = CANARY_CONSTANT;
        new_adress += DATA_SHIFT;
        *(canary_t*)(new_adress + stack->capacity) = CANARY_CONSTANT;
#endif

#ifndef CANARY_DEFF
        new_adress = (data_t*) realloc(stack->data,
                                       (stack->capacity) * sizeof(data_t));//TODO проверка возвращаемого значения realloc
#endif
    }

	return new_adress;
}

size_t StackHash (Stack* stack)
{
    if (stack->data == nullptr && stack->data < (data_t*)4000)
    {
        return 0;
    }

    size_t hash = 2139062143;

    for (int i = 0; i < stack->capacity; i++) //TODO vallgrind ругается на uninitialised value (если использовать capacity)
    {
        hash = 37 * hash + (size_t)stack->data[i];
    }

    hash = 37 * hash + (size_t)stack->capacity;
    hash = 37 * hash + (size_t)stack->data;
    hash = 37 * hash + (size_t)stack->size;

    return hash;
}

int StackErrorCheck (Stack* stack, int* errors)
{
	if (stack == nullptr)
		*errors |= STK_IS_NULL_PTR;

	if (stack->data == nullptr)
		*errors |= DATA_IS_NULL_PTR;

	if (stack->data == UNAVAILABLE_ADR)
		*errors |= STK_DESTROYED;

    if ((*errors & STK_IS_NULL_PTR) == STK_IS_NULL_PTR)
        return *errors;

    if ((*errors & STK_IS_NULL_PTR)  == STK_IS_NULL_PTR ||
        (*errors & DATA_IS_NULL_PTR) == DATA_IS_NULL_PTR ||
        (*errors & STK_DESTROYED)    == STK_DESTROYED)
        return *errors;

	if (stack->size < 0)
		*errors |= STK_UNDERFL;

	if (stack->size > stack->capacity)
		*errors |= STK_OVERFL;

#ifdef CANARY_DEFF
    if (stack->canary_left != CANARY_CONSTANT || stack->canary_right != CANARY_CONSTANT)
        *errors |= STRCT_CANARY_BAD;

    if (((canary_t*)stack->data)[-1] != CANARY_CONSTANT ||
    *(canary_t*)(stack->data + stack->capacity) != CANARY_CONSTANT)
        *errors |= DATA_CANARY_BAD;
#endif

#ifdef HASH_DEFF
    if (stack->hash != StackHash(stack))
        *errors |= HASH_BAD;
#endif
	if (*errors != 0)
		return *errors;

	return 0;
}

int StackPopCheck (Stack* stack, int* errors)
{
    if (stack == nullptr)
        *errors |= STK_IS_NULL_PTR;

    if (stack->data == nullptr)
        *errors |= DATA_IS_NULL_PTR;

    if (stack->data == UNAVAILABLE_ADR)
        *errors |= STK_DESTROYED;

    if ((*errors & STK_IS_NULL_PTR)  == STK_IS_NULL_PTR ||
        (*errors & DATA_IS_NULL_PTR) == DATA_IS_NULL_PTR ||
        (*errors & STK_DESTROYED)    == STK_DESTROYED)
        return *errors;

    if (stack->size <= 0)
        *errors |= STK_UNDERFL;

    if (stack->size > stack->capacity)
        *errors |= STK_OVERFL;

#ifdef CANARY_DEFF
    if (stack->canary_left != CANARY_CONSTANT || stack->canary_right != CANARY_CONSTANT)
        *errors |= STRCT_CANARY_BAD;

    if (((canary_t*)stack->data)[-1] != CANARY_CONSTANT ||
        *(canary_t*)(stack->data + stack->capacity) != CANARY_CONSTANT)
        *errors |= DATA_CANARY_BAD;
#endif

#ifdef HASH_DEFF
    if (stack->hash != StackHash(stack))
        *errors |= HASH_BAD;
#endif

    if (*errors != 0)
        return *errors;

    return 0;
}

int StackCtorCheck (Stack* stack, int* errors)
{
	if (stack->data != nullptr && stack->data != UNAVAILABLE_ADR)
	{
		*errors |= STK_DOUBLE_CTED;
		return *errors;
	}
	else
		return 0;
}

int StackDtorCheck (Stack* stack, int* errors)
{
    if (stack == nullptr)
        *errors |= STK_IS_NULL_PTR;

    if (stack->data == nullptr)
        *errors |= DATA_IS_NULL_PTR;

    if (stack->data == UNAVAILABLE_ADR)
        *errors |= STK_DESTROYED;

    if ((*errors & STK_DESTROYED) == STK_DESTROYED)
        return *errors;

    if (*errors != 0)
        return *errors;

    return 0;
}

void StackDump (Stack* stack, int errors, const char* current_file, const char* current_function)
{
	FILE* dump_file = fopen("../test/Dump.txt", "a"); //TODO сделать чтобы каждый раз создавался новый файл

    if (errors != 0)
    {
        fprintf(dump_file, "\n______________________________\n");
    }

	if ((errors & STK_IS_NULL_PTR) == STK_IS_NULL_PTR)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack is nullptr: %d\n",
			current_file, current_function, errors & STK_IS_NULL_PTR);
	}

	if ((errors & DATA_IS_NULL_PTR) == DATA_IS_NULL_PTR)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack data is nullptr: %d\n",
			current_file, current_function, errors & DATA_IS_NULL_PTR);
	}

	if ((errors & STK_DESTROYED) == STK_DESTROYED)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack destroyed: %d\n",
			current_file, current_function, errors & STK_DESTROYED);
	}

	if ((errors & STK_UNDERFL) == STK_UNDERFL)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack underflowed: %d\n",
			current_file, current_function, errors & STK_UNDERFL);
	}

	if ((errors & STK_OVERFL) == STK_OVERFL)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack overflowed: %d\n",
			current_file, current_function, errors & STK_OVERFL);
	}

	if ((errors & STK_DOUBLE_CTED) == STK_DOUBLE_CTED)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack already constructed: %d\n",
			current_file, current_function, errors & STK_DOUBLE_CTED);
	}

    if ((errors & STRCT_CANARY_BAD) == STRCT_CANARY_BAD)
    {
        fprintf(dump_file, "\n-In file: %s; In function: %s\n Canary of Structure \"Stack\" has been changed: "
                           "Error code: %d\n", current_file, current_function, errors & STRCT_CANARY_BAD);
    }

    if ((errors & DATA_CANARY_BAD) == DATA_CANARY_BAD)
    {
        fprintf(dump_file, "\n-In file: %s; In function: %s\n Data canary has been changed illegally : Error code: %d\n",
                current_file, current_function, errors & DATA_CANARY_BAD);
    }

    if ((errors & HASH_BAD) == HASH_BAD)
    {
        fprintf(dump_file, "\n-In file: %s; In function: %s\n Data hash has been changed illegally. Error code: %d\n",
                current_file, current_function, errors & HASH_BAD);
    }

    if (errors != 0)
    {
        fprintf(dump_file, "\n Stack: \n");
        for (int i = 0; i < stack->size; i++)
#ifdef FLOAT_DATA
            fprintf(dump_file, "[%d]: %f\n", i, stack->data[i]);
#endif
#ifdef INT_DATA
            fprintf(dump_file, "[%d]: %d\n", i, stack->data[i]);
#endif
    }

	fclose(dump_file);
}