#ifndef BREDER_MEMORYSTREAM_H
#define BREDER_MEMORYSTREAM_H

/**
 * Type of Memory Input Stream
 */
typedef struct b_memoryinputstream_t b_memoryinputstream_t;

/**
 * Type of Memory Output Stream
 */
typedef struct b_memoryoutputstream_t b_memoryoutputstream_t;

/**
 * Type of Error
 */
typedef unsigned char b_memorystream_error_t;

/**
 * Create a Memory Input Stream with a path of memorysystem.
 * Return null if the memory not exist.
 * Return null if same error happended.
 */
struct b_memoryinputstream_t* b_memoryinputstream_new(unsigned char* bytes, int size);

/**
 * Return the next byte between 0-255.
 * Return -1 if the end of memory.
 */
int b_memoryinputstream_read(struct b_memoryinputstream_t* self);

/**
 * End of file of memory
 */
unsigned char b_memoryinputstream_is_eof(struct b_memoryinputstream_t* self) ;

/**
 * Close the memory
 */
void b_memoryinputstream_close(struct b_memoryinputstream_t* self);

/**
 * Create a Memory Output Stream with a path of memorysystem.
 * If the memory not exist, it will create a new.
 * Return null if same error happended.
 */
struct b_memoryoutputstream_t* b_memoryoutputstream_new(const char* name);

/**
 * Write a byte.
 * Return a number different of zero if same error happended.
 */
b_memorystream_error_t b_memoryoutputstream_write(struct b_memoryoutputstream_t* self, unsigned char byte);

/**
 * Close the memory
 */
void b_memoryoutputstream_close(struct b_memoryoutputstream_t* self);

#endif
