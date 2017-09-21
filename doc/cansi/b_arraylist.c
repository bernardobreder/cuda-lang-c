#include <stdlib.h>
#include <string.h>
#include "b_arraylist.h"

struct b_arraylist_t {
	int size;
	int length;
	void** datas;
	void (*freeFunc)(void*);
};

struct b_arraylist_t* b_arraylist_new_default() {
	return b_arraylist_new_custom(15, 0);
}

struct b_arraylist_t* b_arraylist_new_custom(int length, void (*freeFunc)(void*)) {
	struct b_arraylist_t* data = (struct b_arraylist_t*) calloc(1, sizeof(struct b_arraylist_t));
	if (!data) {
		return 0;
	}
	data->datas = (void**) calloc(length, sizeof(void*));
	if (!data->datas) {
		free(data);
		return 0;
	}
	data->length = length;
	data->size = 0;
	data->freeFunc = freeFunc;
	return data;
}

void b_arraylist_free(struct b_arraylist_t* self) {
	b_arraylist_clear(self);
	free(self->datas);
	free(self);
}

b_arraylist_error_t b_arraylist_add(struct b_arraylist_t* data, void* element) {
	if (data->size == data->length) {
		void** datas = (void**) realloc(data->datas, data->length * 2 * sizeof(void*));
		if (!datas) {
			return 1;
		}
		data->datas = datas;
		data->length *= 2;
	}
	data->datas[data->size++] = element;
	return 0;
}

void* b_arraylist_get(struct b_arraylist_t* data, int index) {
	if (index < 0 || index >= data->size) {
		return 0;
	}
	return data->datas[index];
}

int b_arraylist_size(struct b_arraylist_t* data) {
	return data->size;
}

void b_arraylist_remove_and_last(struct b_arraylist_t* data, int index) {
	int lastIndex = data->size - 1;
	if (index < 0 || index > lastIndex) {
		return;
	}
	if (data->freeFunc) {
		data->freeFunc(data->datas[index]);
	}
	if (index != lastIndex) {
		data->datas[index] = data->datas[lastIndex];
	}
	data->size--;
}

void b_arraylist_remove_and_shift(struct b_arraylist_t* data, int index) {
	int lastIndex = data->size - 1;
	if (index < 0 || index > lastIndex) {
		return;
	}
	if (data->freeFunc) {
		data->freeFunc(data->datas[index]);
	}
	if (index != lastIndex) {
		memcpy(data->datas + index, data->datas + index + 1, lastIndex - index);
	}
	data->size--;
}

void b_arraylist_set(struct b_arraylist_t* data, int index, void* element) {
	if (index < 0 || index >= data->size) {
		return;
	}
	if (data->freeFunc) {
		data->freeFunc(data->datas[index]);
	}
	data->datas[index] = element;
}

void b_arraylist_clear(struct b_arraylist_t* data) {
	if (data->size == 0) {
		return;
	}
	if (data->freeFunc) {
		void** datas = data->datas;
		int n;
		for (n = 0; n < data->size; n++) {
			data->freeFunc(*datas++);
		}
	}
	data->size = 0;
}

void** b_arraylist_iterator(struct b_arraylist_t* data) {
	return data->datas;
}
