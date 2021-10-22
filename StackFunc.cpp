#include "Stack.h"

const int    START_CAPACITY  = 1;
const int    CAPACITY_STEP   = 2;
const size_t CANARY_CONSTANT = 0xB00BA555;
const int*   UNAVAILABLE_ADR = (int*) 1;

#define CANARY_DEF

int StackCtor (Stack* stack, int* errors, int capacity)
{
	STACK_CTOR_CHECK

    stack->capacity = capacity;

#ifdef CANARY_DEF
    stack->data = (data_t*) calloc(stack->capacity*sizeof(data_t) + 2*sizeof(canary_t),
                                   sizeof(char));

    stack->canary_left = CANARY_CONSTANT;
    stack->canary_right = CANARY_CONSTANT;

    ((canary_t*)stack->data)[0] = CANARY_CONSTANT;
    (stack->data) += sizeof(canary_t) / sizeof (data_t);
    *(canary_t*)(stack->data + stack->capacity) = CANARY_CONSTANT;
#endif

#ifndef CANARY_DEF
    stack->data = (data_t*) calloc (stack->capacity, sizeof(data_t)); //TODO проверка возвращаемого значения calloc
    stack->size = 0;
#endif

    STACK_ERROR_CHECK

	return 0;
}

int StackDtor (Stack* stack, int* errors)
{
	STACK_ERROR_CHECK

    (stack->data) -= sizeof(canary_t) / sizeof (data_t);

	memset (stack->data, 0xF0F0F0F0, stack->capacity);
	free(stack->data);

	stack->size = -1;
	stack->capacity = 0;
	stack->data = (data_t*) UNAVAILABLE_ADR;

#ifdef CANARY_DEF
    stack->canary_left = 0;
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

    STACK_ERROR_CHECK

	return 0;
}

data_t StackPop (Stack* stack, int* errors)
{
    STACK_POP_ERROR_CHECK

	int value;
	
	if (stack->size * CAPACITY_STEP == stack->capacity)
	{
		stack->data = StackResize(stack, errors);
	}

	value = stack->data[--stack->size];

	stack->data[stack->size] = 0;

    STACK_POP_ERROR_CHECK

	return value;
}

data_t* StackResize (Stack* stack, int* errors)
{
    STACK_RESIZE_ERROR_CHECK

	data_t* new_adress = stack->data;

	if (stack->size >= stack->capacity)
	{
		stack->capacity *= CAPACITY_STEP;

#ifdef CANARY_DEF

        (stack->data) -= sizeof(canary_t) / sizeof (data_t);

        new_adress = (data_t*) realloc(stack->data,
                                       stack->capacity * sizeof(data_t) + 2*sizeof(canary_t));//TODO проверка возвращаемого значения realloc

        ((canary_t*)new_adress)[0] = CANARY_CONSTANT;
        new_adress += sizeof(canary_t) / sizeof (data_t);
        *(canary_t*)(new_adress + stack->capacity) = CANARY_CONSTANT;
#endif

#ifndef CANARY_DEF
        new_adress = (data_t*) realloc(stack->data,
                                       (stack->capacity) * sizeof(data_t));//TODO проверка возвращаемого значения realloc
#endif
	}
	else if (stack->size * CAPACITY_STEP == stack->capacity)
	{
		stack->capacity /= CAPACITY_STEP;

#ifdef CANARY_DEF

        (stack->data) -= sizeof(canary_t) / sizeof (data_t);

        new_adress = (data_t*) realloc(stack->data,
                                       stack->capacity * sizeof(data_t) + 2*sizeof(canary_t));//TODO проверка возвращаемого значения realloc

        ((canary_t*)new_adress)[0] = CANARY_CONSTANT;
        new_adress += sizeof(canary_t) / sizeof (data_t);
        *(canary_t*)(new_adress + stack->capacity) = CANARY_CONSTANT;
#endif

#ifndef CANARY_DEF
        new_adress = (data_t*) realloc(stack->data,
                                       (stack->capacity) * sizeof(data_t));//TODO проверка возвращаемого значения realloc
#endif
    }

    //Здесь проверки не работают

	return new_adress;
}

int StackErrorCheck (Stack* stack, int* errors)
{
	if (!stack)
		*errors |= STK_IS_NULL_PTR;

	if (!stack->data)
		*errors |= DATA_IS_NULL_PTR;

	if (stack->data == UNAVAILABLE_ADR)
		*errors |= STK_DESTROYED;

	if (stack->size < 0)
		*errors |= STK_UNDERFL;

	if (stack->size > stack->capacity)
		*errors |= STK_OVERFL;

    if (stack->canary_left != CANARY_CONSTANT || stack->canary_right != CANARY_CONSTANT)
        *errors |= STRCT_CANARY_BAD;

    if (((canary_t*)stack->data)[-1] != CANARY_CONSTANT ||
    *(canary_t*)(stack->data + stack->capacity) != CANARY_CONSTANT)
    {
        *errors |= DATA_CANARY_BAD;
    }

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

int StackCanaryCheck (Stack* stack, int* errors)
{
    if (stack->data != nullptr && stack->data != UNAVAILABLE_ADR)
    {
        *errors |= STK_DOUBLE_CTED;
        return *errors;
    }
    else
        return 0;
}

void StackDump (Stack* stack, int errors, const char* current_file, const char* current_function)
{
	FILE* dump_file = fopen("Dump.txt", "a");

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
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack is already destroyed: %d\n", 
			current_file, current_function, errors & STK_DESTROYED);
	}

	if ((errors & STK_UNDERFL) == STK_UNDERFL)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack is underflowed: %d\n", 
			current_file, current_function, errors & STK_UNDERFL);
	}

	if ((errors & STK_OVERFL) == STK_OVERFL)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack is overflowed: %d\n", 
			current_file, current_function, errors & STK_OVERFL);
	}

	if ((errors & STK_DOUBLE_CTED) == STK_DOUBLE_CTED)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack is constructed twice: %d\n", 
			current_file, current_function, errors & STK_DOUBLE_CTED);
	}

    if ((errors & STRCT_CANARY_BAD) == STRCT_CANARY_BAD)
    {
        fprintf(dump_file, "\n-In file: %s; In function: %s\n Canary of Structure \"Stack\" has been changed: %d\n",
                current_file, current_function, errors & STRCT_CANARY_BAD);
    }

    if ((errors & DATA_CANARY_BAD) == DATA_CANARY_BAD)
    {
        fprintf(dump_file, "\n-In file: %s; In function: %s\n Data canary has been changed: %d\n",
                current_file, current_function, errors & DATA_CANARY_BAD);
    }

    if (errors != 0)
    {
        fprintf(dump_file, "\n Stack: \n");
        for (int i = 0; i < stack->size; i++)
            fprintf(dump_file, "[%d]: %d\n", i, stack->data[i]);
    }

	fclose(dump_file);
}

int StackTestInt (Stack* stack, int* errors)
{
	FILE* fp = fopen("Test.txt", "w");

	data_t pop_elem;
	data_t push_elem = 0;
	int n_of_tests = 10, prblm_sz = 8;

	fprintf(fp, "\n\n\tPush and Pop\n\n");

	for (int i = 0; i < n_of_tests; i++, push_elem += 1) //Пуш и поп n элементов около size = 0
 	{
		fprintf(fp, "\n#%d\n", i + 1);
		fprintf(fp, "Before Actions | capacity: %d, size: %d;\n", 
			stack->capacity, stack->size);

		StackPush(stack, push_elem, errors);
		fprintf(fp, "After Push     | capacity: %d, size: %d;\n", 
			stack->capacity, stack->size);

		pop_elem = StackPop(stack, errors);
		fprintf(fp, "After Pop      | capacity: %d, size: %d;\n", 
			stack->capacity, stack->size);

		fprintf(fp, "Push Elem = %d , Pop Elem = %d\n", push_elem, pop_elem);
	}

    fprintf(fp, "\n\n\tPush elems to problem size\n\n");

 	for (int i = 0; i < prblm_sz; i++, push_elem += 1) //Пуш элементов до problem size
 	{
        fprintf(fp, "\n#%d\n", i + 1);
        fprintf(fp, "Before Actions | capacity: %d, size: %d;\n",
                stack->capacity, stack->size);

        StackPush(stack, push_elem, errors);
        fprintf(fp, "After Push     | capacity: %d, size: %d;\n",
                stack->capacity, stack->size);

        fprintf(fp, "Push Elem = %d\n", push_elem);
 	}

    fprintf(fp, "\n\n\tPush and Pop in problem size\n\n");

  	for (int i = 0; i < n_of_tests; i++, push_elem += 1) //Пуш и поп n элементов около problem size
 	{
		fprintf(fp, "\n#%d\n", i + 1);
 		fprintf(fp, "Before Actions | capacity: %d, size: %d;\n",
  			stack->capacity, stack->size);

 		StackPush(stack, push_elem, errors);
  		fprintf(fp, "After Push     | capacity: %d, size: %d;\n", 
  			stack->capacity, stack->size);

		pop_elem = StackPop(stack, errors);
		fprintf(fp, "After Pop      | capacity: %d, size: %d;\n", 
			stack->capacity, stack->size);

		fprintf(fp, "Push Elem = %d , Pop Elem = %d\n", push_elem, pop_elem);
 	}


   	fprintf(fp, "\n\n\tPop all\n\n");
 	for (int i = 0; i < n_of_tests; i++) //Поп n элементов подряд
	{
		fprintf(fp, "\n#%d\n", i + 1);
		fprintf(fp, "Before Actions | capacity: %d, size: %d;\n", 
			stack->capacity, stack->size);

		pop_elem = StackPop(stack, errors);
		fprintf(fp, "After Pop      | capacity: %d, size: %d;\n", 
			stack->capacity, stack->size);

		fprintf(fp, "Pop Elem = %d\n", pop_elem);
 	}

	return 0;
}

// int StackTestFloat (Stack *stack)
// {
// 	if(stack == nullptr)
// 		return STK_IS_NULL_PTR;

// 	FILE* fp = fopen("Test.txt", "w");

// 	data_t pop_elem = 0;
// 	data_t push_elem = 0;
// 	int n_of_tests = 5, prblm_sz = 8;

// 	fprintf(fp, "\n\n\tPush and Pop\n\n");

// 	for (int i = 0; i < n_of_tests; i++, push_elem += 1) //Пуш и поп n элементов около size = 0
//  	{
// 		fprintf(fp, "\n#%d\n", i + 1);
// 		fprintf(fp, "Before Actions | capacity: %d, size: %d;\n", 
// 			stack->capacity, stack->size);

// 		StackPush(stack, push_elem);
// 		fprintf(fp, "After Push     | capacity: %d, size: %d;\n", 
// 			stack->capacity, stack->size);

// 		pop_elem = StackPop(stack);
// 		fprintf(fp, "After Pop      | capacity: %d, size: %d;\n", 
// 			stack->capacity, stack->size);

// 		fprintf(fp, "Push Elem = %f , Pop Elem = %f\n", push_elem, pop_elem);
// 	}

//   	fprintf(fp, "\n\n\tPush and Pop in problem size\n\n");

//  	for (int i = 0; i < prblm_sz; i++, push_elem += 1) //Пуш элементов до problem size
//  	{
//  		StackPush(stack, push_elem);	
//  	}

//   	for (int i = 0; i < n_of_tests; i++, push_elem += 1) //Пуш и поп n элементов около problem size
//  	{
// 		fprintf(fp, "\n#%d\n", i + 1);
//  		fprintf(fp, "Before Actions | capacity: %d, size: %d;\n",
//   			stack->capacity, stack->size);

//  		StackPush(stack, push_elem);
//   		fprintf(fp, "After Push     | capacity: %d, size: %d;\n", 
//   			stack->capacity, stack->size);

// 		pop_elem = StackPop(stack);
// 		fprintf(fp, "After Pop      | capacity: %d, size: %d;\n", 
// 			stack->capacity, stack->size);

// 		fprintf(fp, "Push Elem = %f , Pop Elem = %f\n", push_elem, pop_elem);
//  	}


//    	fprintf(fp, "\n\n\tPop all\n\n");
//  	for (int i = 0; i < n_of_tests; i++) //Поп n элементов подряд
// 	{
// 		fprintf(fp, "\n#%d\n", i + 1);
// 		fprintf(fp, "Before Actions | capacity: %d, size: %d;\n", 
// 			stack->capacity, stack->size);

// 		pop_elem = StackPop(stack);
// 		fprintf(fp, "After Pop      | capacity: %d, size: %d;\n", 
// 			stack->capacity, stack->size);

// 		fprintf(fp, "Pop Elem = %f\n", pop_elem);
//  	}

// 	return 0;
// }