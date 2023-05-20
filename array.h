//
//  array.h
//  Game
//
//  Created by Thomas Foster on 9/30/22.
//

#ifndef array_h
#define array_h

#include <stdlib.h>

#define ARRAY_DOUBLE (-1)

/// Dynamic Array.
///
/// Grows by chosen factor when adding elements if there is no room.
/// Does not shrink unless told to.
typedef struct
{
    size_t  esize;  // Element size in bytes.
    int     count;  // Current num of elements.
    int     slots;  // Total number of slots allocated.
    int     resize; // Number of slots added when needing to grow.

    void * data;
} Array;

/// Allocate an initialize a new `Array`.
/// - parameter slots: The initialize number of slots to allocate.
/// - parameter esize: Element size in bytes.
/// - parameter resize: The number of slots to add when growing the array.
Array * NewArray(int slots, size_t esize, int resize);

/// Free array and the array's `data` member.
void FreeArray(Array * array);

//
// Taking a risk and not namespacing these for convenience...
//

/// Remove all elements by setting the array count to zero. Nothing is resized.
void Clear(Array * array);

/// Resize array to number.
/// - parameter slots: The new number of slots.
void Resize(Array * array, int slots);

/// Push `element` to end of array.
void * Push(Array * array, void * element);

/// Remove and return last element in `out`.
void Pop(Array * array, void * out);

/// Get a pointer to the last element.
void * Peek(Array * array);

/// Get a pointer to element at `index`.
void * Get(Array * array, int index);

/// Insert `element` at `index`. Elements are shifted to make room.
void * Insert(Array * array, void * element, int index);

/// Replace element at `index` with `element`
void Replace(Array * array, void * element, int index);

/// Remove element at `index`. Elements are shifted.
void Remove(Array * array, int index);

/// Remove element at `index` quickly by moving the last element in the array
/// to `index`.
void FastRemove(Array * arr, int index);

#endif /* array_h */
