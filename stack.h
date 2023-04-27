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

typedef struct mystack mystack_t;

mystack_t * NewStack(size_t element_size);
void        FreeStack(mystack_t * stack);
bool        IsStackEmpty(mystack_t * stack);
void *      Peek(mystack_t * stack);
void *      Push(mystack_t * stack, void * data);
void *      Pop(mystack_t * stack);

#endif /* stack_h */
