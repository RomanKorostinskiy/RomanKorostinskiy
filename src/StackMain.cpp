#include "../include/Stack.h"

int main ()
{
    Stack stack = {};

    int error_code = 0;

    StackCtor(&stack, &error_code);

//    StackStructHack(&stack);

//    StackDataHack(&stack);

#ifdef INT_DATA
    StackTestInt(&stack, &error_code);
#endif

#ifdef FLOAT_DATA
    StackTestFloat(&stack, &error_code);
#endif

    StackDtor(&stack, &error_code);

    return 0;
}