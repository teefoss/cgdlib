//
//  stack.c
//  Game
//
//  Created by Thomas Foster on 10/17/22.
//

#include "stack.h"
#include "genlib.h"
#include "shorttypes.h"

struct mystack {
    void * data;
    size_t esize;
    int top;
};

mystack_t * NewStack(size_t esize)
{
    mystack_t * stack = malloc(sizeof(*stack));
    if ( stack == NULL ) {
        Error("could not allocate stack");
    }

    stack->data = NULL;
    stack->top = -1;
    stack->esize = esize;

    return stack;
}

void FreeStack(mystack_t * stack)
{
    if ( stack->data ) {
        free(stack->data);
    }

    free(stack);
}

bool IsStackEmpty(mystack_t * stack)
{
    return stack->top == -1;
}

void * Peek(mystack_t * stack)
{
    if ( stack->top == -1 ) {
        return NULL;
    }

    return (u8 *)stack->data + stack->top * stack->esize;
}

void * Push(mystack_t * stack, void * data)
{
    ++stack->top;

    void * temp = realloc(stack->data, (stack->top + 1) * stack->esize);
    if ( temp == NULL ) {
        Error("failed to reallocate stack");
    }
    stack->data = temp;

    void * top = Peek(stack);
    memmove(top, data, stack->esize);

    return top;
}

void * Pop(mystack_t * stack)
{
    if ( IsStackEmpty(stack) ) {
        Error("tried to pop from empty stack");
    }

    void * top = Peek(stack);
    --stack->top;

    return top;
}



#if 0

#include <stdio.h>

void PrintIntStack(mystack_t * stack)
{
    printf("--------------------\n");
    printf("stack top: %d\n", stack->top);
    for ( int i = 0; i <= stack->top; i++ ) {
        printf("stack index %2d: %3d\n",
               i,
               *(int *)(stack->data + i * stack->esize));
    }
}

void PushTest(mystack_t * stack, int test_value)
{
    puts("\npush:");
    Push(stack, &test_value);
    PrintIntStack(stack);
}

void PopTest(mystack_t * stack)
{
    puts("\npop:");
    Pop(stack);
    PrintIntStack(stack);
}

void TestStack(void)
{
    mystack_t * stack = NewStack(sizeof(int));
    puts("- begin stack test -");
    puts("created stack");

    if ( IsStackEmpty(stack) ) {
        puts("stack is empty");
    }

    PushTest(stack, 10);
    PushTest(stack, 15);
    PushTest(stack, 20);

    PopTest(stack);
    PopTest(stack);
    PopTest(stack);

    FreeStack(stack);

    puts("\n- end stack test -\n\n");
}

#endif
