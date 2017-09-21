#ifndef BREDER_ARRAYQUEUE_H
#define BREDER_ARRAYQUEUE_H

/**
 * Define the type of arrayqueue
 */
typedef struct b_arrayqueue_t b_arrayqueue_t;

/**
 * Define the type of error
 */
typedef unsigned char b_arrayqueue_error_t;

/**
 * Create a new arrayqueue with default configuration
 */
struct b_arrayqueue_t* b_arrayqueue_new_default();

/**
 * Create a new arrayqueue with custom configuration
 * The inicial size of array is define by parameter 'size'
 * The free function (can be null) is used for remove element or clear the arrayqueue.
 */
struct b_arrayqueue_t* b_arrayqueue_new_custom(int size);

/**
 * Free the memory
 */
void b_arrayqueue_free(struct b_arrayqueue_t* self);

/**
 * Add a new element in the end of queue
 */
b_arrayqueue_error_t b_arrayqueue_add(struct b_arrayqueue_t* data, void* element);

/**
 * Return the first element of arrayqueue
 */
void* b_arrayqueue_get(struct b_arrayqueue_t* data);

/**
 * Remove the first element of arrayqueue.
 * After remove, put the last element in the index
 * This method remove the order of elements
 */
void* b_arrayqueue_remove(struct b_arrayqueue_t* data);

/**
 * Return the size of arrayqueue
 */
int b_arrayqueue_size(struct b_arrayqueue_t* data);

/**
 * Remove and Return a element of arrayqueue
 */
void b_arrayqueue_clear(struct b_arrayqueue_t* data);

#endif
