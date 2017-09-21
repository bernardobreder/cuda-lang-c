#ifndef BREDER_HASHMAP_H
#define BREDER_HASHMAP_H

/**
 * Type of hashmap
 */
typedef struct b_hashmap_t b_hashmap_t;

/**
 * Type of error
 */
typedef unsigned char b_hashmap_error_t;

/**
 * Create a map with default properties
 *
 * Parameters:
 * - hashFunc function that return the hash of key
 * - equalFunc function that return if the keys is equals
 * - freeKeyFunc function (optional) that delete the key
 * - freeValueFunc function (optional) that delete the value
 */
struct b_hashmap_t* b_hashmap_new_default(int (*hashFunc)(void*),
		int (*equalFunc)(void*, void*), void (*freeKeyFunc)(void*),
		void (*freeValueFunc)(void*));

/**
 * Create a map with default properties
 * The key is a int
 *
 * Parameters:
 * - freeValueFunc função opcional que libera da memoria o valor
 */
struct b_hashmap_t* b_hashmap_new_default_int(void (*freeValueFunc)(void*));

/**
 * Create a map with default properties
 * The key is a char*.
 *
 * Parameters:
 * - freeValueFunc função opcional que libera da memoria o valor
 */
struct b_hashmap_t* b_hashmap_new_default_vchar(void (*freeValueFunc)(void*));

/**
 * Create a map with custom properties
 *
 * Parameters:
 * - initialCapactity valor da capacidade inicial
 * - loadFactor fator de carga de novos espaços internamente
 * - hashFunc function that return the hash of key
 * - equalFunc function that return if the keys is equals
 * - freeKeyFunc function (optional) that delete the key
 * - freeValueFunc function (optional) that delete the value
 */
struct b_hashmap_t* b_hashmap_new_custom(int initialCapacity, float loadFactor,
		int (*hashFunc)(void*), int (*equalFunc)(void*, void*),
		void (*freeKeyFunc)(void*), void (*freeValueFunc)(void*));

/**
 * Free the hashmap struct
 */
void b_hashmap_free(struct b_hashmap_t* self);

/**
 * Return a value with a key. The key can not be null.
 * Return null if the map not contain the key.
 *
 * Parameters:
 * - self hashmap
 * - key key to find
 */
void* b_hashmap_get(struct b_hashmap_t* self, void* key);

/**
 * Return a value with a key. The key can not be null.
 * Return null if the map not contain the key.
 *
 * Parameters:
 * - self hashmap
 * - key key to find
 */
void* b_hashmap_get_int(struct b_hashmap_t* self, int key);

/**
 * Add or replace a new element in the hashmap. The key can not be null.
 * Return the old value if the the key already exist and replace the value.
 * Return null if same memory error happened.
 *
 * Parameters:
 * - self hashmap
 * - key key to find and put
 * - value value to put
 * - oldValue pointer (optional) to register the old value
 */
b_hashmap_error_t b_hashmap_put(struct b_hashmap_t* self, void* key,
		void* value, void** oldValue);

/**
 * Add or replace a new element in the hashmap. The key can not be null.
 * Return the old value if the the key already exist and replace the value.
 * Return null if same memory error happened.
 *
 * Parameters:
 * - self hashmap
 * - key key to find and put
 * - value value to put
 * - oldValue pointer (optional) to register the old value
 */
b_hashmap_error_t b_hashmap_put_int(struct b_hashmap_t* self, int key,
		void* value, void** oldValue);

/**
 * Remove a element from the hashmap.
 * If a free function registered, the key and the value will free.
 *
 * Parameters:
 * - self hashmap
 * - key key to find and remove
 */
void* b_hashmap_remove(struct b_hashmap_t* self, void* key);

/**
 * Remove a element from the hashmap.
 * If a free function registered, the key and the value will free.
 *
 * Parameters:
 * - self hashmap
 * - key key to find and remove
 */
void* b_hashmap_remove_int(struct b_hashmap_t* self, int key);

/**
 * Return the size of hashmap
 *
 * Parameters:
 * - self hashmap
 */
int b_hashmap_size(struct b_hashmap_t* self);

/**
 * Clean the hashmap
 * If a free function registered, the all keys and all values will free.
 *
 * Parameters:
 * - self hashmap
 */
void b_hashmap_clear(struct b_hashmap_t* self);

/**
 * Test the hashmap library
 */
void b_hashmap_test();

#endif
