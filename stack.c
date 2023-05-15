//
//  stack.c
//  Game
//
//  Created by Thomas Foster on 10/17/22.
//

#include "stack.h"
#include "genlib.h"
#include "shorttypes.h"

typedef struct stack_node StackNode;
struct stack_node {
    void * data;
    StackNode * next;
};

struct stack {
    StackNode * top;
    size_t esize;
};

static StackNode * NewNode(size_t esize)
{
    StackNode * node = malloc(sizeof(*node));
    ASSERT(node != NULL);

    if ( node ) {
        node->data = malloc(esize);
        ASSERT(node->data != NULL);
    }

    return node;
}

Stack * NewStack(size_t esize)
{
    Stack * stack = malloc(sizeof(*stack));
    ASSERT(stack != NULL);

    if ( stack ) {
        stack->top = NULL;
        stack->esize = esize;
    }

    return stack;
}

void FreeStack(Stack * stack)
{
    ASSERT(stack != NULL);

    StackNode * node = stack->top;
    while ( node ) {
        free(node->data);
        StackNode * temp = node;
        node = node->next;
        free(temp);
    }

    free(stack);
}

bool StackIsEmpty(const Stack * stack)
{
    return stack->top == NULL;
}

void * StackPeek(const Stack * stack)
{
    return stack->top;
}

void StackPush(Stack * stack, void * data)
{
    StackNode * node = NewNode(stack->esize);
    ASSERT(node != NULL);

    memcpy(node->data, data, stack->esize);
    node->next = stack->top;
    stack->top = node;
}

bool StackPop(Stack * stack, void * out)
{
    if ( StackIsEmpty(stack) ) {
        return false;
    }

    memcpy(out, stack->top->data, stack->esize);
    StackNode * temp = stack->top;
    stack->top = stack->top->next;
    free(temp);

    return true;
}
