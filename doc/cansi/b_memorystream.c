#include <stdlib.h>
#include <stdio.h>
#include "b_memorystream.h"

struct b_memoryinputstream_t {
	unsigned char* bytes;
	int length;
	int index;
};

struct b_memoryoutputstream_t {
	unsigned char* bytes;
	int length;
	int size;
};

struct b_memoryinputstream_t* b_memoryinputstream_new(unsigned char* bytes,
		int size) {
	struct b_memoryinputstream_t* self = (struct b_memoryinputstream_t*) calloc(
			1, sizeof(struct b_memoryinputstream_t));
	if (!self) {
		return 0;
	}
	self->bytes = bytes;
	self->length = size;
	return self;
}

int b_memoryinputstream_read(struct b_memoryinputstream_t* self) {
	if (self->index == self->length) {
		return -1;
	}
	return self->bytes[self->index++];
}

unsigned char b_memoryinputstream_is_eof(struct b_memoryinputstream_t* self) {
	return self->index == self->length;
}

void b_memoryinputstream_close(struct b_memoryinputstream_t* self) {
	free(self->bytes);
	free(self);
}

struct b_memoryoutputstream_t* b_memoryoutputstream_new_create() {
	struct b_memoryoutputstream_t* self =
			(struct b_memoryoutputstream_t*) calloc(1,
					sizeof(struct b_memoryoutputstream_t));
	if (!self) {
		return 0;
	}
	self->length = 16;
	self->bytes = (unsigned char*) calloc(self->length, sizeof(unsigned char));
	return self;
}

b_memorystream_error_t b_memoryoutputstream_write(
		struct b_memoryoutputstream_t* self, unsigned char byte) {
	if (self->size == self->length) {
		unsigned char* data = (unsigned char*) realloc(self->bytes,
				self->length * 2 * sizeof(unsigned char));
		if (!data) {
			return 1;
		}
		self->length *= 2;
		self->bytes = data;
	}
	self->bytes[self->size++] = byte;
	return 0;
}

void b_memoryoutputstream_close(struct b_memoryoutputstream_t* self) {
	free(self->bytes);
	free(self);
}
