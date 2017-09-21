#ifndef BREDER_ARRAYLIST_H
#define BREDER_ARRAYLIST_H

/**
 * Define the type of arraylist
 */
typedef struct b_arraylist_t b_arraylist_t;

/**
 * Define the type of error
 */
typedef unsigned char b_arraylist_error_t;

/**
 * Create a new arraylist with default configuration
 */
struct b_arraylist_t* b_arraylist_new_default();

/**
 * Create a new arraylist with custom configuration
 * The inicial size of array is define by parameter 'size'
 * The free function (can be null) is used for remove element or clear the arraylist.
 */
struct b_arraylist_t* b_arraylist_new_custom(int size, void (*freeFunc)(void*));

/**
 * Free the arraylist struct
 */
void b_arraylist_free(struct b_arraylist_t* self) ;

/**
 * Add a new element
 */
b_arraylist_error_t b_arraylist_add(struct b_arraylist_t* data, void* element);

/**
 * Return a element of arraylist
 */
void* b_arraylist_get(struct b_arraylist_t* data, int index);

/**
 * Remove a element of arraylist.
 * After remove, put the last element in the index
 * This method remove the order of elements
 */
void b_arraylist_remove_and_last(struct b_arraylist_t* data, int index);

/**
 * Remove a element of arraylist
 * After remove, move the nexts element to left
 * This method maintain the order but need to move the memory
 */
void b_arraylist_remove_and_shift(struct b_arraylist_t* data, int index);

/**
 * Change the element of arraylist
 */
void b_arraylist_set(struct b_arraylist_t* data, int index, void* element);

/**
 * Return the size of arraylist
 */
int b_arraylist_size(struct b_arraylist_t* data);

/**
 * Remove and Return a element of arraylist
 */
void b_arraylist_clear(struct b_arraylist_t* data);

/**
 * Return a iterator of arraylist. The size of iterator defined by 'b_arraylist_size'
 * The return can not free of memomry
 */
void** b_arraylist_iterator(struct b_arraylist_t* data);

#endif
