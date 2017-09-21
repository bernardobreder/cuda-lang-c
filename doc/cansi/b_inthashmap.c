#include <stdlib.h>
#include "b_inthashmap.h"

struct b_inthashmap_entry_t {
	void* value;
	int hash;
	struct b_inthashmap_entry_t* next;
};

struct b_inthashmap_t {
	struct b_inthashmap_entry_t** table;
	int length;
	int size;
	int threshold;
	int initialCapacity;
	float loadFactor;
	void (*freeValueFunc)(void*);
};

struct b_inthashmap_t* b_inthashmap_new_default(void (*freeValueFunc)(void*)) {
	struct b_inthashmap_t* self = (struct b_inthashmap_t*) calloc(1,
			sizeof(struct b_inthashmap_t));
	if (!self) {
		return 0;
	}
	struct b_inthashmap_entry_t** table =
			(struct b_inthashmap_entry_t**) calloc(16,
					sizeof(struct b_inthashmap_entry_t*));
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
	self->freeValueFunc = freeValueFunc;
	return self;
}

struct b_inthashmap_t* b_inthashmap_new_custom(int initialCapacity,
		float loadFactor, void (*freeValueFunc)(void*)) {
	int capacity = 1;
	while (capacity < initialCapacity) {
		capacity <<= 1;
	}
	struct b_inthashmap_t* self = (struct b_inthashmap_t*) calloc(1,
			sizeof(struct b_inthashmap_t));
	if (!self) {
		return 0;
	}
	struct b_inthashmap_entry_t** table =
			(struct b_inthashmap_entry_t**) calloc(capacity,
					sizeof(struct b_inthashmap_entry_t*));
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
	self->freeValueFunc = freeValueFunc;
	return self;
}

static int b_inthashmap_hash(int h) {
	h ^= (h >> 20) ^ (h >> 12);
	return h ^ (h >> 7) ^ (h >> 4);
}

static int b_inthashmap_index(int h, int length) {
	return h & (length - 1);
}

static int b_inthashmap_resize(struct b_inthashmap_t* self, int newCapacity) {
	struct b_inthashmap_entry_t** newTable =
			(struct b_inthashmap_entry_t**) calloc(newCapacity,
					sizeof(struct b_inthashmap_entry_t*));
	if (!newTable) {
		return 1;
	}
	struct b_inthashmap_entry_t** oldTable = self->table;
	int j;
	for (j = 0; j < self->length; j++) {
		struct b_inthashmap_entry_t* srcEntry = oldTable[j];
		if (srcEntry) {
			oldTable[j] = 0;
			do {
				struct b_inthashmap_entry_t* next = srcEntry->next;
				int index = b_inthashmap_index(srcEntry->hash, newCapacity);
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

void* b_inthashmap_get(struct b_inthashmap_t* self, int key) {
	int hash = b_inthashmap_hash(key);
	int indexFor = b_inthashmap_index(hash, self->length);
	struct b_inthashmap_entry_t* e = self->table[indexFor];
	for (; e; e = e->next) {
		if (e->hash == hash) {
			return e->value;
		}
	}
	return 0;
}

int b_inthashmap_put(struct b_inthashmap_t* self, int key, void* value,
		void** oldValue) {
	int hash = b_inthashmap_hash(key);
	int indexFor = b_inthashmap_index(hash, self->length);
	struct b_inthashmap_entry_t* e = self->table[indexFor];
	for (; e; e = e->next) {
		if (e->hash == hash) {
			void* old = e->value;
			e->value = value;
			if (oldValue) {
				*oldValue = old;
			}
			return 0;
		}
	}
	e = (struct b_inthashmap_entry_t*) calloc(1,
			sizeof(struct b_inthashmap_entry_t));
	if (!e) {
		return 1;
	}
	e->value = value;
	e->hash = hash;
	e->next = self->table[indexFor];
	self->table[indexFor] = e;
	if (self->size++ >= self->threshold) {
		if (b_inthashmap_resize(self, 2 * self->length)) {
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

void* b_inthashmap_remove(struct b_inthashmap_t* self, int key) {
	int hash = b_inthashmap_hash(key);
	int indexFor = b_inthashmap_index(hash, self->length);
	struct b_inthashmap_entry_t* prev = self->table[indexFor];
	struct b_inthashmap_entry_t* e = prev;
	void* value = 0;
	while (e) {
		if (e->hash == hash) {
			self->size--;
			if (prev == e) {
				self->table[indexFor] = e->next;
			} else {
				prev->next = e->next;
			}
			if (self->freeValueFunc) {
				self->freeValueFunc(e->value);
			}
			value = e->value;
			free(e);
			break;
		}
		prev = e;
		e = e->next;
	}
	if (self->size <= self->threshold / 2
			&& self->length >= self->initialCapacity) {
		b_inthashmap_resize(self, self->length / 2);
	}
	return value;
}

int b_inthashmap_size(struct b_inthashmap_t* self) {
	return self->size;
}

void b_inthashmap_clear(struct b_inthashmap_t* self) {
	struct b_inthashmap_entry_t** table = self->table;
	int i;
	for (i = 0; i < self->length; i++) {
		struct b_inthashmap_entry_t* e = table[i];
		while (e) {
			struct b_inthashmap_entry_t* next = e->next;
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
