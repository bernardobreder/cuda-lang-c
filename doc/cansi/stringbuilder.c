#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "stringbuilder.h"

struct stringbuilder_t* stringbuilder_new() {
	struct stringbuilder_t* self = (struct stringbuilder_t*) malloc(sizeof(struct stringbuilder_t));
	if (!self) {
		return 0;
	}
	self->first = 0;
	self->last = 0;
	self->size = 0;
	return self;
}

void stringbuilder_free(struct stringbuilder_t* self) {
	struct stringbuilder_node_t* node = self->first;
	while (node) {
		if (node->can_free) {
			free(node->buffer);
		}
		free(node);
		node = node->next;
	}
	free(self);
}

unsigned char stringbuilder_append_chars(struct stringbuilder_t* self, unsigned char* buffer, size_t length, unsigned char clone) {
	if (length <= 0) {
		return 0;
	}
	struct stringbuilder_node_t* node = (struct stringbuilder_node_t*) malloc(sizeof(struct stringbuilder_node_t));
	if (!node) {
		return 1;
	}
	if (clone) {
		unsigned char* data = (unsigned char*) malloc((length + 1) * sizeof(unsigned char));
		if (!data) {
			free(node);
			return 1;
		}
		strncpy((char*) data, (char*) buffer, length);
		data[length] = 0;
		node->buffer = data;
		node->size = length;
	} else {
		node->buffer = buffer;
		node->size = length;
	}
	node->can_free = clone;
	node->next = 0;
	if (self->last) {
		self->last->next = node;
	} else {
		self->first = node;
	}
	self->last = node;
	self->size += length;
	return 0;
}

unsigned char stringbuilder_append_uint(struct stringbuilder_t* self, unsigned int value) {
	struct stringbuilder_node_t* node = (struct stringbuilder_node_t*) malloc(sizeof(struct stringbuilder_node_t));
	if (!node) {
		return 1;
	}
	unsigned char* data = (unsigned char*) malloc(11 * sizeof(unsigned char));
	if (!data) {
		free(node);
		return 1;
	}
	int i = (int) log10((double) value);
	int length = i + 1;
	while (value > 0) {
		data[i] = (value % 10) + '0';
		value = value / 10;
		i = i - 1;
	}
	data[i] = 0;
	node->buffer = data;
	node->size = length;
	node->can_free = 1;
	node->next = 0;
	if (self->last) {
		self->last->next = node;
	} else {
		self->first = node;
	}
	self->last = node;
	self->size += length;
	return 0;
}

unsigned char* stringbuilder_tostring(struct stringbuilder_t* self, size_t* length) {
	if (self->size == 0) {
		unsigned char* data = (unsigned char*) malloc(1 * sizeof(unsigned char));
		if (!data) {
			return 0;
		}
		*data = 0;
		return data;
	}
	if (self->first->next) {
		if (stringbuilder_trim(self)) {
			return 0;
		}
	}
	if (length) {
		*length = self->size;
	}
	unsigned char* data = (unsigned char*) malloc((self->size + 1) * sizeof(unsigned char));
	if (!data) {
		return 0;
	}
	data[self->size] = 0;
	strncpy((char*) data, (char*) self->first->buffer, self->size);
	return data;
}

unsigned char stringbuilder_trim(struct stringbuilder_t* self) {
	if (self->size == 0) {
		return 0;
	}
	if (!self->first->next) {
		return 0;
	}
	unsigned char* data = (unsigned char*) malloc((self->size + 1) * sizeof(unsigned char));
	if (!data) {
		return 1;
	}
	data[self->size] = 0;
	unsigned char* aux = data;
	struct stringbuilder_node_t* first = self->first;
	struct stringbuilder_node_t* node = first;
	while (node) {
		strncpy((char*) aux, (char*) node->buffer, node->size);
		aux += node->size;
		struct stringbuilder_node_t* next = node->next;
		if (node->can_free) {
			free(node->buffer);
		}
		if (node != first) {
			free(node);
		}
		node = next;
	}
	self->first->buffer = data;
	self->first->size = self->size;
	self->first->can_free = 1;
	self->last = self->first;
	self->first->next = 0;
	return 0;
}