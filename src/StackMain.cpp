#include "../include/Stack.h"

int main ()
{
    Stack stack = {};

    StackCtor(&stack);

    StackCtor(&stack);

//    StackStructHack(&stack);
//
//    StackDataHack(&stack);

#ifdef INT_DATA
    StackTestInt(&stack);
#endif

#ifdef FLOAT_DATA
    StackTestFloat(&stack);
#endif

    StackDtor(&stack);

    return 0;
}