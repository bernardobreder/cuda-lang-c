#ifndef BREDER_MEMORY_H
#define BREDER_MEMORY_H

/**
 * Enable or Disable the debug of memory.
 * The size determine how many size the memory has.
 */
void b_memory_debug(int flag, long long size);

/**
 * Free the pointer of memory
 */
void b_memory_free(void* data);

/**
 * Alloc a new pointer of memory
 */
void* b_memory_alloc(long long size);

/**
 * Realloc a new pointer of memory baseated with the old pointer of memory and the new size
 */
void* b_memory_realloc(void* old, long long size);

/**
 * Return the free memory available. Return a negative value if is not in the debug environment registed by b_memory_debug.
 */
long long b_memory_size();

/**
 * Teste the memory library
 */
void b_memory_test();

#endif
