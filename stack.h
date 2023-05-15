//
//  stack.h
//  Game
//
//  Created by Thomas Foster on 10/17/22.
//

#ifndef stack_h
#define stack_h

#include <stdlib.h>
#include <stdbool.h>

typedef struct stack Stack;

Stack * NewStack(size_t element_size);
void    FreeStack(Stack * stack);

bool    StackIsEmpty(const Stack * stack);
void *  StackPeek(const Stack * stack);
void    StackPush(Stack * stack, void * data);
bool    StackPop(Stack * stack, void * out);

#endif /* stack_h */
