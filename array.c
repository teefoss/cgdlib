//
//  array.c
//  Game
//
//  Created by Thomas Foster on 9/30/22.
//

#include "array.h"
#include "shorttypes.h"
#include "genlib.h"
#include <string.h>

void * Push(Array * arr, void * element) {
    return Insert(arr, element, arr->count);
}

void * Get(Array * arr, int i) {
    ASSERT((unsigned)i < arr->count);

    return (u8 *)arr->data + arr->esize * i;
}

void Clear(Array * arr) {
    arr->count = 0;
}

Array * NewArray(int slots, size_t esize, int resize)
{
    ASSERT(slots > 0 || resize != 0);
    ASSERT(esize > 0);

    Array * arr = malloc(sizeof(*arr));
    ASSERT(arr != NULL);

    if ( slots != 0 ) {
        arr->data = malloc(slots * esize);
    }
    arr->slots = slots;
    arr->count = 0;
    arr->esize = esize;
    arr->resize = resize;

    return arr;
}

void FreeArray(Array * arr) {
    ASSERT(arr->data);
    ASSERT(arr);

    free(arr->data);
    free(arr);
}

static void Grow(Array * arr)
{
    if ( arr->resize == 0 ) {
        return;
    }

    if ( arr->resize == ARRAY_DOUBLE ) {
        arr->slots *= 2;
    } else {
        arr->slots += arr->resize;
    }

    arr->data = realloc(arr->data, arr->slots * arr->esize);
    ASSERT(arr->data != NULL);
}

void Resize(Array * arr, int slots)
{
    ASSERT(slots >= 0);

    arr->slots = slots;

    if ( slots < arr->count ) {
        arr->count = slots; // We've lots some elements.
    }

    if ( arr->data ) {
        if ( slots == 0 ) {
            free(arr->data);
        } else {
            arr->data = realloc(arr->data, slots * arr->esize);
            ASSERT(arr->data != NULL);
        }
    }
}

void * Insert(Array * arr, void * element, int i)
{
    ASSERT(i <= arr->count); // Inserting at arr->count will resize the array.
    if ( arr->count + 1 > arr->slots ) {
        Grow(arr);
    }

    // In case someone tried to add to a full static array.
    ASSERT(arr->count + 1 <= arr->slots);

    // Move the latter part of the array right.
    memmove((u8 *)arr->data + arr->esize * (i + 1),
            (u8 *)arr->data + arr->esize * i,
            arr->esize * (arr->count - i));

    // Insert element at i.
    memmove((u8 *)arr->data + arr->esize * i, element, arr->esize);

    ++arr->count;
    return (u8 *)arr->data + (arr->count - 1) * arr->esize;
}

void Remove(Array * arr, int i)
{
    ASSERT((unsigned)i < arr->count);

    // move the latter part of the array left
    memmove((u8 *)arr->data + arr->esize * i,
            (u8 *)arr->data + arr->esize * (i + 1),
            arr->esize * (arr->count - i - 1));
    --arr->count;
}

void FastRemove(Array * arr, int i)
{
    ASSERT((unsigned)i < arr->count)

    // move the last element to i
    memmove((u8 *)arr->data + arr->esize * i,
            (u8 *)arr->data + arr->esize * (arr->count - 1),
            arr->esize);

    --arr->count;
}

void Replace(Array * arr, void * element, int i)
{
    ASSERT((unsigned)i < arr->count);

    memmove((u8 *)arr->data + arr->esize * i, element, arr->esize);
}

void Pop(Array * arr, void * out)
{
    ASSERT(arr->count > 0);
    memcpy(out, Peek(arr), arr->esize);
    arr->count--;
}

void * Peek(Array * arr)
{
    ASSERT(arr->count > 0);
    return (u8 *)arr->data + (arr->count - 1) * arr->esize;
}
