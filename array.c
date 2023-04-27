//
//  array.c
//  Game
//
//  Created by Thomas Foster on 9/30/22.
//

#include "array.h"
#include <string.h>

#define EL(arr, i) (arr->data + arr->esize * (i))

void * Append(array_t * arr, void * element) {
    return Insert(arr, element, arr->count);
}

void * GetElement(array_t * arr, int i) {
    if ( (unsigned)i >= arr->count )
        return NULL;

    return EL(arr, i);
}

void Clear(array_t * arr) {
    arr->count = 0;
}

void FreeArray(array_t * arr) {
    free(arr->data);
    free(arr);
}

array_t * NewArray(int count, size_t esize) {
    array_t * arr;

    arr = malloc(sizeof *arr);
    arr->data = malloc(count * esize);
    arr->count = count;
    arr->esize = esize;

    return arr;
}

void * Insert(array_t * arr, void * element, int i) {
    if ( (unsigned)i > arr->count ) // inserting at data[count] is valid
        return NULL;

    arr->data = realloc(arr->data, (arr->count + 1) * arr->esize);

    // move the latter part of the array right
    memmove(arr->data + arr->esize * (i + 1),
            arr->data + arr->esize * i,
            arr->esize * (arr->count - i));

    // insert element at i
    memmove(arr->data + arr->esize * i, element, arr->esize);

    ++arr->count;
    return arr->data + (arr->count - 1) * arr->esize;
}

void Remove(array_t * arr, int i) {
    if ( (unsigned)i >= arr->count )
        return;

    // move the latter part of the array left
    memmove(EL(arr, i), EL(arr, i + 1), arr->esize * (arr->count - i - 1));
    --arr->count;
}

void FastRemove(array_t * arr, int i) {
    if ( (unsigned)i >= arr->count )
        return;

    // move the last element to i
    memmove(EL(arr, i), EL(arr, arr->count - 1), arr->esize);
    --arr->count;
}

void Replace(array_t * arr, void * element, int i) {
    if ((unsigned)i >= arr->count)
        return;

    memmove(EL(arr, i), element, arr->esize);
}

void * PopLast(array_t * arr)
{
    return arr->data + (--arr->count * arr->esize);
}

void * GetLastElement(array_t * arr)
{
    if ( arr->count > 0 ) {
        return arr->data + (arr->count - 1) * arr->esize;
    }

    return NULL;
}
