#include <stdlib.h>
#include <string.h>
#include "b_arrayqueue.h"

struct b_arrayqueue_t {
	int length;
	void** elements;
	int head;
	int tail;
	int initialCapacity;
};

struct b_arrayqueue_t* b_arrayqueue_new_default() {
	return b_arrayqueue_new_custom(16);
}

struct b_arrayqueue_t* b_arrayqueue_new_custom(int length) {
	struct b_arrayqueue_t* data = (struct b_arrayqueue_t*) calloc(1, sizeof(struct b_arrayqueue_t));
	if (!data) {
		return 0;
	}
	int initialCapacity = 8;
	if (length >= initialCapacity) {
		initialCapacity = length;
		initialCapacity |= (initialCapacity >> 1);
		initialCapacity |= (initialCapacity >> 2);
		initialCapacity |= (initialCapacity >> 4);
		initialCapacity |= (initialCapacity >> 8);
		initialCapacity |= (initialCapacity >> 16);
		initialCapacity++;
		if (initialCapacity < 0) {
			initialCapacity = initialCapacity >> 1;
		}
	}
	data->elements = (void**) calloc(initialCapacity, sizeof(void*));
	if (!data->elements) {
		free(data);
		return 0;
	}
	data->length = length;
	data->initialCapacity = initialCapacity;
	return data;
}

void b_arrayqueue_free(struct b_arrayqueue_t* self) {
	free(self->elements);
	free(self);
}

b_arrayqueue_error_t b_arrayqueue_add(struct b_arrayqueue_t* self, void* element) {
	if (((self->tail + 1) % self->length) == self->head) {
		int newCapacity = self->length << 1;
		void** elements = (void**) realloc(self->elements, newCapacity * sizeof(void*));
		if (!elements) {
			return 1;
		}
		if (self->tail < self->head) {
			memcpy(elements + self->length, elements, self->tail);
			self->tail = self->length + self->tail - 1;
		}
		self->elements = elements;
		self->length = newCapacity;
	}
	self->elements[self->tail] = element;
	self->tail = (self->tail + 1) % self->length;
	return 0;
}

void* b_arrayqueue_get(struct b_arrayqueue_t* self) {
	return self->elements[self->head];
}

int b_arrayqueue_size(struct b_arrayqueue_t* self) {
	if (self->tail >= self->head) {
		return self->tail - self->head;
	} else {
		return self->length - self->head - self->tail - 1;
	}
}

void* b_arrayqueue_remove(struct b_arrayqueue_t* self) {
	int h = self->head;
	void* result = self->elements[h];
	self->head = (h + 1) % (self->length - 1);
	if (((self->tail - self->head) % (self->length - 1)) == 0) {
		b_arrayqueue_clear(self);
	}
	return result;
}

void b_arrayqueue_clear(struct b_arrayqueue_t* self) {
	self->head = self->tail = 0;
	void** elements = (void**) realloc(self->elements, self->initialCapacity * sizeof(void*));
	if (elements) {
		self->elements = elements;
		self->length = self->initialCapacity;
	}
}

