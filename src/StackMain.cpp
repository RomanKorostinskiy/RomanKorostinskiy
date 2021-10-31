#include "../include/Stack.h"

int main ()
{
    Stack stack = {};

    StackCtor(&stack);

//    StackStructHack(&stack);
//
//    StackDataHack(&stack);

#ifdef INT_DATA
    StackTestInt(&stack);
#endif

#ifdef FLOAT_DATA
    StackTestFloat(&stack, &error_code);
#endif

    StackDtor(&stack);

    return 0;
}