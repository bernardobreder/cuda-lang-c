#include <stdlib.h>
#include <string.h>
#include "b_hashmap.h"

struct b_hashmap_entry_t {
	void* key;
	void* value;
	int hash;
	struct b_hashmap_entry_t* next;
};

struct b_hashmap_t {
	struct b_hashmap_entry_t** table;
	int length;
	int size;
	int threshold;
	int initialCapacity;
	float loadFactor;
	int (*hashFunc)(void*);
	int (*equalFunc)(void*, void*);
	void (*freeKeyFunc)(void*);
	void (*freeValueFunc)(void*);
};

struct b_hashmap_t* b_hashmap_new_default(int (*hashFunc)(void*), int (*equalFunc)(void*, void*), void (*freeKeyFunc)(void*), void (*freeValueFunc)(void*)) {
	struct b_hashmap_t* self = (struct b_hashmap_t*) calloc(1, sizeof(struct b_hashmap_t));
	if (!self) {
		return 0;
	}
	struct b_hashmap_entry_t** table = (struct b_hashmap_entry_t**) calloc(16, sizeof(struct b_hashmap_entry_t*));
	if (!table) {
		free(self);
		return 0;
	}
	self->initialCapacity = 16;
	self->table = table;
	self->length = 16;
	self->size = 0;
	self->threshold = 12;
	self->loadFactor = 0.75f;
	self->hashFunc = hashFunc;
	self->equalFunc = equalFunc;
	self->freeKeyFunc = freeKeyFunc;
	self->freeValueFunc = freeValueFunc;
	return self;
}

void b_hashmap_free(struct b_hashmap_t* self) {
	b_hashmap_clear(self);
	free(self->table);
	free(self);
}

static int b_hashmap_int_hash(void* key) {
	return (long long) key;
}

static int b_hashmap_int_equal(void* o1, void* o2) {
	return o1 == o2;
}

struct b_hashmap_t* b_hashmap_new_default_int(void (*freeValueFunc)(void*)) {
	return b_hashmap_new_default(b_hashmap_int_hash, b_hashmap_int_equal, 0, freeValueFunc);
}

static int b_hashmap_vchar_hash(void* key) {
	char* chars = (char*) key;
	if (*chars) {
		return 0;
	}
	return *chars;
}

static int b_hashmap_vchar_equal(void* o1, void* o2) {
	return !strcmp((char*) o1, (char*) o2);
}

struct b_hashmap_t* b_hashmap_new_default_vchar(void (*freeValueFunc)(void*)) {
	return b_hashmap_new_default(b_hashmap_vchar_hash, b_hashmap_vchar_equal, 0, freeValueFunc);
}

struct b_hashmap_t* b_hashmap_new_custom(int initialCapacity, float loadFactor, int (*hashFunc)(void*), int (*equalFunc)(void*, void*), void (*freeKeyFunc)(void*), void (*freeValueFunc)(void*)) {
	int capacity = 1;
	while (capacity < initialCapacity) {
		capacity <<= 1;
	}
	struct b_hashmap_t* self = (struct b_hashmap_t*) calloc(1, sizeof(struct b_hashmap_t));
	if (!self) {
		return 0;
	}
	struct b_hashmap_entry_t** table = (struct b_hashmap_entry_t**) calloc(capacity, sizeof(struct b_hashmap_entry_t*));
	if (!table) {
		free(self);
		return 0;
	}
	self->initialCapacity = capacity;
	self->table = table;
	self->length = capacity;
	self->size = 0;
	self->threshold = capacity * loadFactor;
	self->loadFactor = loadFactor;
	self->hashFunc = hashFunc;
	self->equalFunc = equalFunc;
	self->freeKeyFunc = freeKeyFunc;
	self->freeValueFunc = freeValueFunc;
	return self;
}

static int b_hashmap_hash(int h) {
	h ^= (h >> 20) ^ (h >> 12);
	return h ^ (h >> 7) ^ (h >> 4);
}

#define b_hashmap_index(h,l) (h & (l-1))

static int b_hashmap_resize(struct b_hashmap_t* self, int newCapacity) {
	struct b_hashmap_entry_t** newTable = (struct b_hashmap_entry_t**) calloc(newCapacity, sizeof(struct b_hashmap_entry_t*));
	if (!newTable) {
		return 1;
	}
	struct b_hashmap_entry_t** oldTable = self->table;
	int j;
	for (j = 0; j < self->length; j++) {
		struct b_hashmap_entry_t* srcEntry = oldTable[j];
		if (srcEntry) {
			oldTable[j] = 0;
			do {
				struct b_hashmap_entry_t* next = srcEntry->next;
				int index = b_hashmap_index(srcEntry->hash, newCapacity);
				srcEntry->next = newTable[index];
				newTable[index] = srcEntry;
				srcEntry = next;
			} while (srcEntry);
		}
	}
	free(oldTable);
	self->table = newTable;
	self->length = newCapacity;
	self->threshold = newCapacity * self->loadFactor;
	return 0;
}

void* b_hashmap_get(struct b_hashmap_t* self, void* key) {
	int hash = b_hashmap_hash(self->hashFunc(key));
	int indexFor = b_hashmap_index(hash, self->length);
	struct b_hashmap_entry_t* e = self->table[indexFor];
	for (; e; e = e->next) {
		if (e->hash == hash) {
			void* k = e->key;
			if (k == key || self->equalFunc(key, k)) {
				return e->value;
			}
		}
	}
	return 0;
}

void* b_hashmap_get_int(struct b_hashmap_t* self, int key) {
	void* p = 0;
	p += key;
	return b_hashmap_get(self, p);
}

b_hashmap_error_t b_hashmap_put(struct b_hashmap_t* self, void* key, void* value, void** oldValue) {
	int hash = b_hashmap_hash(self->hashFunc(key));
	int indexFor = b_hashmap_index(hash, self->length);
	struct b_hashmap_entry_t* e = self->table[indexFor];
	for (; e; e = e->next) {
		if (e->hash == hash) {
			void* k = e->key;
			if (k == key || self->equalFunc(key, k)) {
				void* old = e->value;
				e->value = value;
				if (oldValue) {
					*oldValue = old;
				}
				return 0;
			}
		}
	}
	e = (struct b_hashmap_entry_t*) calloc(1, sizeof(struct b_hashmap_entry_t));
	if (!e) {
		return 1;
	}
	e->key = key;
	e->value = value;
	e->hash = hash;
	e->next = self->table[indexFor];
	self->table[indexFor] = e;
	if (self->size++ >= self->threshold) {
		if (b_hashmap_resize(self, 2 * self->length)) {
			self->table[indexFor] = e->next;
			free(e);
			return 1;
		}
	}
	if (oldValue) {
		*oldValue = 0;
	}
	return 0;
}

b_hashmap_error_t b_hashmap_put_int(struct b_hashmap_t* self, int key, void* value, void** oldValue) {
	void* p = 0;
	p += key;
	return b_hashmap_put(self, p, value, oldValue);
}

void* b_hashmap_remove(struct b_hashmap_t* self, void* key) {
	int hash = b_hashmap_hash(self->hashFunc(key));
	int indexFor = b_hashmap_index(hash, self->length);
	struct b_hashmap_entry_t* prev = self->table[indexFor];
	struct b_hashmap_entry_t* e = prev;
	void* value = 0;
	while (e) {
		if (e->hash == hash) {
			void* k = e->key;
			if (k == key || self->equalFunc(key, k)) {
				self->size--;
				if (prev == e) {
					self->table[indexFor] = e->next;
				} else {
					prev->next = e->next;
				}
				if (self->freeKeyFunc) {
					self->freeKeyFunc(e->key);
				}
				if (self->freeValueFunc) {
					self->freeValueFunc(e->value);
				}
				value = e->value;
				free(e);
				break;
			}
		}
		prev = e;
		e = e->next;
	}
	if (self->size <= self->threshold / 2 && self->length >= self->initialCapacity) {
		b_hashmap_resize(self, self->length / 2);
	}
	return value;
}

void* b_hashmap_remove_int(struct b_hashmap_t* self, int key) {
	void* p = 0;
	p += key;
	return b_hashmap_remove(self, p);
}

int b_hashmap_size(struct b_hashmap_t* self) {
	return self->size;
}

void b_hashmap_clear(struct b_hashmap_t* self) {
	if (self->size == 0) {
		return;
	}
	struct b_hashmap_entry_t** table = self->table;
	int i;
	for (i = 0; i < self->length; i++) {
		struct b_hashmap_entry_t* e = table[i];
		while (e) {
			struct b_hashmap_entry_t* next = e->next;
			if (self->freeKeyFunc) {
				self->freeKeyFunc(e->key);
			}
			if (self->freeValueFunc) {
				self->freeValueFunc(e->value);
			}
			free(e);
			e = next;
		}
		table[i] = 0;
	}
	self->size = 0;
}
