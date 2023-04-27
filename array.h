//
//  array.h
//  Game
//
//  Created by Thomas Foster on 9/30/22.
//

#ifndef array_h
#define array_h

#include <stdlib.h>

typedef struct
{
    void *  data;
    size_t  esize;
    int     count; // current num of elements
} array_t;

array_t *   NewArray(int count, size_t esize);
void        FreeArray(array_t *);
void        Clear(array_t *);
void *      Append(array_t *, void * element); // Push at end
void *      GetElement(array_t *, int index);
void *      Insert(array_t *, void * element, int index);
void        Replace(array_t *, void * element, int index);
void        Remove(array_t *, int index);
void        FastRemove(array_t * arr, int index);
void *      PopLast(array_t * arr);
void *      GetLastElement(array_t * arr);

#endif /* array_h */
