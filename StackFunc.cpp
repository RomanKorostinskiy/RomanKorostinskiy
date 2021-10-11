#include "Stack.h"

const int *UNAVAILABLE_ADR = (int*) 1;
const int START_CAPACITY = 1;
const int CAPACITY_STEP = 2;

int StackCtor (Stack* stack, int* errors, int capacity) //START_CAPACITY //TODO аргумент по умолчанию
{
	STACK_CTOR_CHECK

	stack->capacity = capacity;

	stack->data = (data_t*) calloc (capacity, sizeof(data_t)); //TODO проверка возвращаемого значения calloc

	stack->size = 0;

	return 0;
}

int StackDtor (Stack* stack, int* errors)
{
	STACK_ERROR_CHECK

	memset (stack->data, 0xF0F0F0F0, stack->capacity);
	free(stack->data);

	stack->size = -1;

	stack->capacity = 0;

	stack->data = (data_t*) UNAVAILABLE_ADR;

	return 0;
}

int StackPush (Stack* stack, data_t value)
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

data_t StackPop (Stack* stack)
{
	if(stack->size <= 0)
		return 	STK_UNDERFL;

	int value;
	
	if (stack->size * CAPACITY_STEP == stack->capacity) //TODO придумать константу вместо 2
	{
		stack->data = StackResize(stack);
	}

	value = stack->data[--stack->size];

	stack->data[stack->size] = 0;

	return value;
}

data_t* StackResize (Stack* stack)
{
	//проверки

	data_t* new_adress = stack->data;

	if (stack->size >= stack->capacity)
	{
		stack->capacity *= CAPACITY_STEP;

		new_adress = (data_t*) realloc(stack->data, //TODO проверка возвращаемого значения realloc
			(stack->capacity) * sizeof(data_t));
	}
	else if (stack->size * CAPACITY_STEP == stack->capacity)
	{
		stack->capacity /= CAPACITY_STEP;

		new_adress = (data_t*) realloc(stack->data, 
			(stack->capacity) * sizeof(data_t));
	}

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

void StackDump (int errors, const char* current_file, const char* current_function)
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

	fclose(dump_file);
}

int StackTestInt (Stack* stack)
{
	if(stack == nullptr)
		return STK_IS_NULL_PTR;

	FILE* fp = fopen("Test.txt", "w");

	data_t pop_elem = 0;
	data_t push_elem = 0;
	int n_of_tests = 5, prblm_sz = 8;

	fprintf(fp, "\n\n\tPush and Pop\n\n");

	for (int i = 0; i < n_of_tests; i++, push_elem += 1) //Пуш и поп n элементов около size = 0
 	{
		fprintf(fp, "\n#%d\n", i + 1);
		fprintf(fp, "Before Actions | capacity: %d, size: %d;\n", 
			stack->capacity, stack->size);

		StackPush(stack, push_elem);
		fprintf(fp, "After Push     | capacity: %d, size: %d;\n", 
			stack->capacity, stack->size);

		pop_elem = StackPop(stack);
		fprintf(fp, "After Pop      | capacity: %d, size: %d;\n", 
			stack->capacity, stack->size);

		fprintf(fp, "Push Elem = %d , Pop Elem = %d\n", push_elem, pop_elem);
	}

  	fprintf(fp, "\n\n\tPush and Pop in problem size\n\n");

 	for (int i = 0; i < prblm_sz; i++, push_elem += 1) //Пуш элементов до problem size
 	{
 		StackPush(stack, push_elem);	
 	}

  	for (int i = 0; i < n_of_tests; i++, push_elem += 1) //Пуш и поп n элементов около problem size
 	{
		fprintf(fp, "\n#%d\n", i + 1);
 		fprintf(fp, "Before Actions | capacity: %d, size: %d;\n",
  			stack->capacity, stack->size);

 		StackPush(stack, push_elem);
  		fprintf(fp, "After Push     | capacity: %d, size: %d;\n", 
  			stack->capacity, stack->size);

		pop_elem = StackPop(stack);
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

		pop_elem = StackPop(stack);
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