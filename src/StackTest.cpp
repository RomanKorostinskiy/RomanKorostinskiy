#include "../include/Stack.h"

#ifdef INT_DATA
int StackTestInt (Stack* stack, int* errors)
{
	FILE* fp = fopen("../test/Test.txt", "w");

	data_t pop_elem;
	data_t push_elem = 0;

    size_t hash = 0;

    int n_of_tests = 10, prblm_sz = 20;

	fprintf(fp, "\n\n\tPush and Pop\n\n");

	for (int i = 0; i < n_of_tests; i++, push_elem += 1) //Пуш и поп n элементов около size = 0
 	{
		fprintf(fp, "\n#%d\n", i + 1);
        hash = StackHash(stack);
		fprintf(fp, "Before Actions | capacity: %d, size: %d, hash: %lu;\n",
			stack->capacity, stack->size, hash );

		StackPush(stack, push_elem, errors);
        hash = StackHash(stack);
		fprintf(fp, "After Push     | capacity: %d, size: %d, hash: %lu;\n",
			stack->capacity, stack->size, hash);

		pop_elem = StackPop(stack, errors);
        hash = StackHash(stack);
		fprintf(fp, "After Pop      | capacity: %d, size: %d, hash: %lu;\n",
			stack->capacity, stack->size, hash);

		fprintf(fp, "Push Elem = %d , Pop Elem = %d\n", push_elem, pop_elem);
	}

    fprintf(fp, "\n\n\tPush elems to problem size\n\n");

 	for (int i = 0; i < prblm_sz; i++, push_elem += 1) //Пуш элементов до problem size
 	{
        fprintf(fp, "\n#%d\n", i + 1);
        hash = StackHash(stack);
        fprintf(fp, "Before Actions | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

        StackPush(stack, push_elem, errors);
        hash = StackHash(stack);
        fprintf(fp, "After Push     | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

        fprintf(fp, "Push Elem = %d\n", push_elem);
 	}

    fprintf(fp, "\n\n\tPush and Pop in problem size\n\n");

  	for (int i = 0; i < n_of_tests; i++, push_elem += 1) //Пуш и поп n элементов около problem size
 	{
		fprintf(fp, "\n#%d\n", i + 1);
        hash = StackHash(stack);
        fprintf(fp, "Before Actions | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

 		StackPush(stack, push_elem, errors);
        hash = StackHash(stack);
        fprintf(fp, "After Push     | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

		pop_elem = StackPop(stack, errors);
        hash = StackHash(stack);
        fprintf(fp, "After Pop      | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

		fprintf(fp, "Push Elem = %d , Pop Elem = %d\n", push_elem, pop_elem);
 	}

   	fprintf(fp, "\n\n\tPop all\n\n");
 	for (int i = 0; i < n_of_tests; i++) //Поп n элементов подряд
	{
		fprintf(fp, "\n#%d\n", i + 1);
        hash = StackHash(stack);
        fprintf(fp, "Before Actions | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

		pop_elem = StackPop(stack, errors);
        hash = StackHash(stack);
        fprintf(fp, "After Pop      | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

		fprintf(fp, "Pop Elem = %d\n", pop_elem);
 	}

	return 0;
}
#endif

#ifdef FLOAT_DATA
int StackTestFloat (Stack* stack, int* errors)
{
    FILE* fp = fopen("../test/Test.txt", "w");

    data_t pop_elem;
    data_t push_elem = 0;

    size_t hash = 0;

    int n_of_tests = 10, prblm_sz = 8;

    fprintf(fp, "\n\n\tPush and Pop\n\n");

    for (int i = 0; i < n_of_tests; i++, push_elem += 1) //Пуш и поп n элементов около size = 0
    {
        fprintf(fp, "\n#%d\n", i + 1);
        hash = StackHash(stack);
        fprintf(fp, "Before Actions | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash );

        StackPush(stack, push_elem, errors);
        hash = StackHash(stack);
        fprintf(fp, "After Push     | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

        pop_elem = StackPop(stack, errors);
        hash = StackHash(stack);
        fprintf(fp, "After Pop      | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

        fprintf(fp, "Push Elem = %lf , Pop Elem = %lf\n", push_elem, pop_elem);
    }

    fprintf(fp, "\n\n\tPush elems to problem size\n\n");

    for (int i = 0; i < prblm_sz; i++, push_elem += 1) //Пуш элементов до problem size
    {
        fprintf(fp, "\n#%d\n", i + 1);
        hash = StackHash(stack);
        fprintf(fp, "Before Actions | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

        StackPush(stack, push_elem, errors);
        hash = StackHash(stack);
        fprintf(fp, "After Push     | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

        fprintf(fp, "Push Elem = %f\n", push_elem);
    }

    fprintf(fp, "\n\n\tPush and Pop in problem size\n\n");

    for (int i = 0; i < n_of_tests; i++, push_elem += 1) //Пуш и поп n элементов около problem size
    {
        fprintf(fp, "\n#%d\n", i + 1);
        hash = StackHash(stack);
        fprintf(fp, "Before Actions | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

        StackPush(stack, push_elem, errors);
        hash = StackHash(stack);
        fprintf(fp, "After Push     | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

        pop_elem = StackPop(stack, errors);
        hash = StackHash(stack);
        fprintf(fp, "After Pop      | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

        fprintf(fp, "Push Elem = %lf , Pop Elem = %lf\n", push_elem, pop_elem);
    }


    fprintf(fp, "\n\n\tPop all\n\n");
    for (int i = 0; i < n_of_tests; i++) //Поп n элементов подряд
    {
        fprintf(fp, "\n#%d\n", i + 1);
        hash = StackHash(stack);
        fprintf(fp, "Before Actions | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

        pop_elem = StackPop(stack, errors);
        hash = StackHash(stack);
        fprintf(fp, "After Pop      | capacity: %d, size: %d, hash: %lu;\n",
                stack->capacity, stack->size, hash);

        fprintf(fp, "Pop Elem = %lf\n", pop_elem);
    }

    return 0;
}
#endif

void StackStructHack (Stack* stack)
{
    stack->data = nullptr;
}

void StackDataHack (Stack* stack)
{
    for (int i = -2; i < 3; i++)
        stack->data[i] = 300;
}