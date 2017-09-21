#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <wchar.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "breder.h"

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



static char b64string[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

long b_base64_encode(char* to, unsigned char* from, unsigned int len) {
	unsigned char *fromp = from;
	char *top = to;
	unsigned char cbyte;
	unsigned char obyte;
	char end[3];
	for (; len >= 3; len -= 3) {
		cbyte = *fromp++;
		*top++ = b64string[(int) (cbyte >> 2)];
		obyte = (cbyte << 4) & 0x30; /* 0011 0000 */
		cbyte = *fromp++;
		obyte |= (cbyte >> 4); /* 0000 1111 */
		*top++ = b64string[(int) obyte];
		obyte = (cbyte << 2) & 0x3C; /* 0011 1100 */
		cbyte = *fromp++;
		obyte |= (cbyte >> 6); /* 0000 0011 */
		*top++ = b64string[(int) obyte];
		*top++ = b64string[(int) (cbyte & 0x3F)];/* 0011 1111 */
	}
	if (len) {
		end[0] = *fromp++;
		if (--len) {
			end[1] = *fromp++;
		} else {
			end[1] = 0;
		}
		end[2] = 0;
		cbyte = end[0];
		*top++ = b64string[(int) (cbyte >> 2)];
		obyte = (cbyte << 4) & 0x30; /* 0011 0000 */
		cbyte = end[1];
		obyte |= (cbyte >> 4);
		*top++ = b64string[(int) obyte];
		obyte = (cbyte << 2) & 0x3C; /* 0011 1100 */
		if (len) {
			*top++ = b64string[(int) obyte];
		} else {
			*top++ = '=';
		}
		*top++ = '=';
	}
	*top = 0;
	return top - to;
}

long b_base64_decode(unsigned char* to, char* from, unsigned int len) {
	char *fromp = from;
	unsigned char *top = to;
	char *p;
	unsigned char cbyte;
	unsigned char obyte;
	int padding = 0;
	for (; len >= 4; len -= 4) {
		if ((cbyte = *fromp++) == '=') {
			cbyte = 0;
		} else {
			if ((!(p = (char*) memchr(b64string, cbyte, 64)))) {
				return -1;
			}
			cbyte = (p - b64string);
		}
		obyte = cbyte << 2; /* 1111 1100 */
		if ((cbyte = *fromp++) == '=') {
			cbyte = 0;
		} else {
			if ((!(p = (char*) memchr(b64string, cbyte, 64)))) {
				return -1;
			}
			cbyte = p - b64string;
		}
		obyte |= cbyte >> 4; /* 0000 0011 */
		*top++ = obyte;
		obyte = cbyte << 4; /* 1111 0000 */
		if ((cbyte = *fromp++) == '=') {
			cbyte = 0;
			padding++;
		} else {
			padding = 0;
			if ((!(p = (char*) memchr(b64string, cbyte, 64)))) {
				return -1;
			}
			cbyte = p - b64string;
		}
		obyte |= cbyte >> 2; /* 0000 1111 */
		*top++ = obyte;

		obyte = cbyte << 6; /* 1100 0000 */
		if ((cbyte = *fromp++) == '=') {
			cbyte = 0;
			padding++;
		} else {
			padding = 0;
			if ((!(p = (char*) memchr(b64string, cbyte, 64)))) {
				return -1;
			}
			cbyte = p - b64string;
		}
		obyte |= cbyte; /* 0011 1111 */
		*top++ = obyte;
	}
	*top = 0;
	if (len) {
		return -1;
	}
	return (top - to) - padding;
}

struct b_consoleinputstream_t {
};

struct b_consoleoutputstream_t {
};

struct b_consoleinputstream_t* b_consoleinputstream_new() {
	struct b_consoleinputstream_t* self = (struct b_consoleinputstream_t*) calloc(1,
			sizeof(struct b_consoleinputstream_t));
	if (!self) {
		return 0;
	}
	return self;
}

int b_consoleinputstream_read(struct b_consoleinputstream_t* self) {
	return getchar();
}

unsigned char b_consoleinputstream_is_eof(struct b_consoleinputstream_t* self){
	return 0;
}

void b_consoleinputstream_close(struct b_consoleinputstream_t* self) {
	free(self);
}

struct b_consoleoutputstream_t* b_consoleoutputstream_new() {
	struct b_consoleoutputstream_t* self = (struct b_consoleoutputstream_t*) calloc(1,
			sizeof(struct b_consoleoutputstream_t));
	if (!self) {
		return 0;
	}
	return self;
}


b_consolestream_error_t b_consoleoutputstream_write(struct b_consoleoutputstream_t* self,
		unsigned char byte) {
	putchar(byte);
	return 0;
}

void b_consoleoutputstream_close(struct b_consoleoutputstream_t* self) {
	free(self);
}

struct b_fileinputstream_t {
	FILE* file;
};

struct b_fileoutputstream_t {
	FILE* file;
};

struct b_fileinputstream_t* b_fileinputstream_new(const char* path) {
	FILE* file = fopen(path, "rb");
	if (!file) {
		return 0;
	}
	struct b_fileinputstream_t* self = (struct b_fileinputstream_t*) calloc(1,
			sizeof(struct b_fileinputstream_t));
	if (!self) {
		fclose(file);
		return 0;
	}
	return self;
}

int b_fileinputstream_read(struct b_fileinputstream_t* self) {
	return fgetc(self->file);
}

unsigned char b_fileinputstream_is_eof(struct b_fileinputstream_t* self){
	return feof(self->file);
}

void b_fileinputstream_close(struct b_fileinputstream_t* self) {
	fclose(self->file);
	free(self);
}

struct b_fileoutputstream_t* b_fileoutputstream_new_create(const char* path) {
	FILE* file = fopen(path, "wb");
	if (!file) {
		return 0;
	}
	struct b_fileoutputstream_t* self = (struct b_fileoutputstream_t*) calloc(1,
			sizeof(struct b_fileoutputstream_t));
	if (!self) {
		fclose(file);
		return 0;
	}
	return self;
}


struct b_fileoutputstream_t* b_fileoutputstream_new_append(const char* path) {
	FILE* file = fopen(path, "ab");
	if (!file) {
		return 0;
	}
	struct b_fileoutputstream_t* self = (struct b_fileoutputstream_t*) calloc(1,
			sizeof(struct b_fileoutputstream_t));
	if (!self) {
		fclose(file);
		return 0;
	}
	return self;
}

b_filestream_error_t b_fileoutputstream_write(struct b_fileoutputstream_t* self,
		unsigned char byte) {
	return fputc(byte, self->file) == EOF;
}

void b_fileoutputstream_flush(struct b_fileoutputstream_t* self) {
	fflush(self->file);
}

void b_fileoutputstream_close(struct b_fileoutputstream_t* self) {
	fclose(self->file);
	free(self);
}

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

unsigned char b_vm_read_byte(unsigned char** bytes) {
	return *((*bytes)++);
}

int b_vm_read_index(unsigned char** bytes, unsigned int* result) {
	unsigned int i1 = *((*bytes)++);
	if (i1 <= 0x7F) {
		if (result) {
			*result = i1;
		}
		return 1;
	} else if ((i1 >> 5) == 0x6) {
		int i2 = *((*bytes)++);
		if (result) {
			*result = ((i1 & 0x1F) << 6) + (i2 & 0x3F);
		}
		return 2;
	} else if ((i1 >> 4) == 0xE) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		if (result) {
			*result = ((i1 & 0xF) << 12) + ((i2 & 0x3F) << 6) + (i3 & 0x3F);
		}
		return 3;
	} else if ((i1 >> 3) == 0x1E) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		if (result) {
			*result = ((i1 & 0x7) << 18) + ((i2 & 0x3F) << 12)
					+ ((i3 & 0x3F) << 6) + (i4 & 0x3F);
		}
		return 4;
	} else if ((i1 >> 2) == 0x3E) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		int i5 = *((*bytes)++);
		if (result) {
			*result = ((i1 & 0x3) << 24) + ((i2 & 0x3F) << 18)
					+ ((i3 & 0x3F) << 12) + ((i4 & 0x3F) << 6) + (i5 & 0x3F);
		}
		return 5;
	} else if ((i1 >> 1) == 0x7E) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		int i5 = *((*bytes)++);
		int i6 = *((*bytes)++);
		if (result) {
			*result = ((i1 & 0x1) << 30) + ((i2 & 0x3F) << 24)
					+ ((i3 & 0x3F) << 18) + ((i4 & 0x3F) << 12)
					+ ((i5 & 0x3F) << 6) + (i6 & 0x3F);
		}
		return 6;
	} else {
		(*bytes)--;
		return 0;
	}
}

int b_vm_read_utf(unsigned char** bytes, wchar_t** result, int* length) {
	unsigned int len;
	int count = b_vm_read_index(bytes, &len);
	if (!count) {
		return 0;
	}
	wchar_t* data = *result = (wchar_t*) calloc(len + 1, sizeof(wchar_t));
	if (!data) {
		*bytes -= count;
		return 0;
	}
	int n;
	for (n = 0; n < len; n++) {
		int m = b_vm_read_index(bytes, (unsigned int*) data);
		if (!m) {
			free(data);
			*bytes -= count;
			return 0;
		}
		count += m;
		data++;
	}
	return count;
}

int b_vm_read_int(unsigned char** bytes, int* result) {
	int i1 = *((*bytes)++);
	if (i1 >> 7 == 0) {
		if (result) {
			if (i1 > 63) {
				*result = -(i1 & 63);
			} else {
				*result = +(i1 & 63);
			}
		}
		return 1;
	} else if (i1 >> 6 == 2) {
		int i2 = *((*bytes)++);
		if (result) {
			if ((i1 & 32) == 32) {
				*result = -(((i1 & 31) << 8) + i2);
			} else {
				*result = +(((i1 & 31) << 8) + i2);
			}
		}
		return 2;
	} else if (i1 >> 5 == 6) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		if (result) {
			if ((i1 & 16) == 16) {
				*result = -(((i1 & 15) << 16) + (i2 << 8) + i3);
			} else {
				*result = +(((i1 & 15) << 16) + (i2 << 8) + i3);
			}
		}
		return 3;
	} else if (i1 >> 4 == 14) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		if (result) {
			if ((i1 & 8) == 8) {
				*result = -(((i1 & 7) << 24) + (i2 << 16) + (i3 << 8) + i4);
			} else {
				*result = +(((i1 & 7) << 24) + (i2 << 16) + (i3 << 8) + i4);
			}
		}
		return 4;
	} else if (i1 >> 3 == 30) {
		i1 = *((*bytes)++);
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		if (result) {
			if (i1 > 127) {
				*result = -(((i1 - 128) << 24) + (i2 << 16) + (i3 << 8) + i4);
			} else {
				*result = +((i1 << 24) + (i2 << 16) + (i3 << 8) + i4);
			}
		}
		return 5;
	} else {
		(*bytes)--;
		return 0;
	}

}

struct b_lexical_t {
	int (*readFunc)(void*);
	void (*closeFunc)(void*);
	void* data;
	int look;
};

struct b_token_t {
	int tag;
};

struct b_token_pchar_t {
	int tag;
	char* value;
};

struct b_token_double_t {
	int tag;
	double value;
};

struct b_token_pwchar_t {
	int tag;
	wchar_t* value;
};

static struct b_token_t *AND_TOKEN = 0, *BREAK_TOKEN = 0, *CONTINUE_TOKEN = 0, *DO_TOKEN = 0, *ELSE_TOKEN = 0, *END_TOKEN = 0, *FALSE_TOKEN = 0, *FOR_TOKEN = 0, *IF_TOKEN = 0, *OR_TOKEN = 0,
		*REPEAT_TOKEN = 0, *THIS_TOKEN = 0, *TRUE_TOKEN = 0, *WHILE_TOKEN = 0, *RETURN_TOKEN = 0;

static struct b_token_t *DEC_TOKEN = 0, *INC_TOKEN = 0, *GE_TOKEN = 0, *LE_TOKEN = 0, *NE_TOKEN = 0, *EQ_TOKEN = 0, *NOT_TOKEN = 0, *ASSIGN_TOKEN = 0, *GT_TOKEN = 0, *LT_TOKEN = 0, *SUM_TOKEN = 0,
		*SUB_TOKEN = 0;

struct b_lexical_t* b_lexical_new(void* data, int (*readFunc)(void*), void (*closeFunc)(void*)) {
	struct b_lexical_t* self = (struct b_lexical_t*) calloc(1, sizeof(struct b_lexical_t));
	if (!self) {
		return 0;
	}
	self->readFunc = readFunc;
	self->closeFunc = closeFunc;
	self->data = data;
	self->look = -2;
	return self;
}

void b_lexical_free(struct b_lexical_t* self) {
	if (self->closeFunc) {
		self->closeFunc(self->data);
	}
	free(self);
}

void b_token_free(struct b_token_t* self) {
	if (self == AND_TOKEN || self == BREAK_TOKEN || self == CONTINUE_TOKEN || self == DO_TOKEN || self == ELSE_TOKEN || self == END_TOKEN || self == FALSE_TOKEN || self == FOR_TOKEN
			|| self == IF_TOKEN || self == OR_TOKEN || self == REPEAT_TOKEN || self == THIS_TOKEN || self == TRUE_TOKEN || self == WHILE_TOKEN || self == RETURN_TOKEN || self == DEC_TOKEN
			|| self == INC_TOKEN || self == GE_TOKEN || self == LE_TOKEN || self == NE_TOKEN || self == EQ_TOKEN || self == NOT_TOKEN || self == ASSIGN_TOKEN || self == GT_TOKEN || self == LT_TOKEN
			|| self == SUM_TOKEN || self == SUB_TOKEN) {
	} else {
		if (self->tag == B_LEXICAL_STR_ID) {
			free(((struct b_token_pwchar_t*) self)->value);
		} else if (self->tag == B_LEXICAL_NUM_ID) {
		} else {
			free(((struct b_token_pchar_t*) self)->value);
		}
		free(self);
	}
}

static unsigned char b_lexical_is_word_start(int c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$';
}

static unsigned char b_lexical_is_word_part(int c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$';
}

static unsigned char b_lexical_is_string_start(int c) {
	return c == '\"';
}

static unsigned char b_lexical_is_number_start(int c) {
	return c >= '0' && c <= '9';
}

static int b_lexical_read_char(struct b_lexical_t* self) {
	int c = self->readFunc(self->data);
	if (c <= 0x7F) {
		return c;
	} else if ((c >> 5) == 0x6) {
		int i2 = self->readFunc(self->data);
		if (i2 < 0) {
			return -1;
		}
		return (((c & 0x1F) << 6) + (i2 & 0x3F));
	} else {
		int i2 = self->readFunc(self->data);
		if (i2 < 0) {
			return -1;
		}
		int i3 = self->readFunc(self->data);
		if (i3 < 0) {
			return -1;
		}
		return (((c & 0xF) << 12) + ((i2 & 0x3F) << 6) + (i3 & 0x3F));
	}
}

static int b_lexical_look(struct b_lexical_t* self) {
	if (self->look == -2) {
		self->look = b_lexical_read_char(self);
	}
	return self->look;
}

static int b_lexical_next(struct b_lexical_t* self) {
	return self->look = b_lexical_read_char(self);
}

static unsigned char b_lexical_is_word(struct b_lexical_t* self) {
	int c = b_lexical_look(self);
	while (c <= 32) {
		if (c < 0) {
			return -1;
		}
		c = b_lexical_next(self);
	}
	return b_lexical_is_word_start(c);
}

static unsigned char b_lexical_is_number(struct b_lexical_t* self) {
	int c = b_lexical_look(self);
	while (c <= 32) {
		if (c < 0) {
			return -1;
		}
		c = b_lexical_next(self);
	}
	return b_lexical_is_number_start(c);
}

static unsigned char b_lexical_is_string(struct b_lexical_t* self) {
	int c = b_lexical_look(self);
	while (c <= 32) {
		if (c < 0) {
			return -1;
		}
		c = b_lexical_next(self);
	}
	return b_lexical_is_string_start(c);
}

static struct b_token_t* b_lexical_token_pchar_new(int tag, char* text) {
	struct b_token_pchar_t* self = (struct b_token_pchar_t*) calloc(1, sizeof(struct b_token_pchar_t));
	if (!self) {
		return 0;
	}
	self->tag = tag;
	self->value = text;
	return (b_token_t*) self;
}

static struct b_token_t* b_lexical_token_pwchar_new(wchar_t* text) {
	struct b_token_pwchar_t* self = (struct b_token_pwchar_t*) calloc(1, sizeof(struct b_token_pwchar_t));
	if (!self) {
		return 0;
	}
	self->tag = B_LEXICAL_STR_ID;
	self->value = text;
	return (b_token_t*) self;
}

static struct b_token_t* b_lexical_token_double_new(double text) {
	struct b_token_double_t* self = (struct b_token_double_t*) calloc(1, sizeof(struct b_token_double_t));
	if (!self) {
		return 0;
	}
	self->tag = B_LEXICAL_NUM_ID;
	self->value = text;
	return (b_token_t*) self;
}

static struct b_token_t* b_lexical_read_token(int id, const char* text) {
	char* image = strdup(text);
	if (!image) {
		return 0;
	}
	struct b_token_t* data = b_lexical_token_pchar_new(id, image);
	if (!data) {
		free(image);
		return 0;
	}
	return data;
}

static struct b_token_t* b_lexical_read_word(struct b_lexical_t* self) {
	int c = b_lexical_look(self);
	while (c <= 32) {
		if (c < 0) {
			return 0;
		}
		c = b_lexical_next(self);
	}
	int length = 1;
	char* text = (char*) calloc(length + 1, sizeof(char));
	char* aux = text;
	if (!text) {
		return 0;
	}
	*aux++ = c;
	c = b_lexical_next(self);
	while (b_lexical_is_word_part(c)) {
		if (text + length == aux) {
			length *= 2;
			char* data = (char*) realloc(text, (length + 1) * sizeof(char));
			if (!data) {
				free(text);
				return 0;
			}
			text = data;
			aux = text + length / 2;
		}
		*aux++ = c;
		c = b_lexical_next(self);
	}
	*aux = 0;
	aux = text;
	switch (*aux++) {
	case 'a': {
		if (*aux++ == 'n' && *aux++ == 'd' && *aux++ == 0) {
			if (!AND_TOKEN) {
				AND_TOKEN = b_lexical_read_token(B_LEXICAL_AND_ID, "and");
			}
			free(text);
			return AND_TOKEN;
		}
		break;
	}
	case 'b': {
		if (*aux++ == 'r' && *aux++ == 'e' && *aux++ == 'a' && *aux++ == 'k' && *aux++ == 0) {
			if (!BREAK_TOKEN) {
				BREAK_TOKEN = b_lexical_read_token(B_LEXICAL_BREAK_ID, "break");
			}
			free(text);
			return BREAK_TOKEN;
		}
		break;
	}
	case 'c': {
		if (*aux++ == 'o' && *aux++ == 'n' && *aux++ == 't' && *aux++ == 'i' && *aux++ == 'n' && *aux++ == 'u' && *aux++ == 'e' && *aux++ == 0) {
			if (!CONTINUE_TOKEN) {
				CONTINUE_TOKEN = b_lexical_read_token(B_LEXICAL_CONTINUE_ID, "continue");
			}
			free(text);
			return CONTINUE_TOKEN;
		}
		break;
	}
	case 'd': {
		if (*aux++ == 'o' && *aux++ == 0) {
			if (!DO_TOKEN) {
				DO_TOKEN = b_lexical_read_token(B_LEXICAL_DO_ID, "do");
			}
			free(text);
			return DO_TOKEN;
		}
		break;
	}
	case 'e': {
		switch (*aux++) {
		case 'n': {
			if (*aux++ == 'd' && *aux++ == 0) {
				if (!END_TOKEN) {
					END_TOKEN = b_lexical_read_token(B_LEXICAL_END_ID, "end");
				}
				free(text);
				return END_TOKEN;
			}
			break;
		}
		}
		case 'l':
		{
			if (*aux++ == 's' && *aux++ == 'e' && *aux++ == 0) {
				if (!ELSE_TOKEN) {
					ELSE_TOKEN = b_lexical_read_token(B_LEXICAL_ELSE_ID, "else");
				}
				free(text);
				return ELSE_TOKEN;
			}
			break;
		}
		break;
	}
	case 'f': {
		switch (*aux++) {
		case 'a': {
			if (*aux++ == 'l' && *aux++ == 's' && *aux++ == 'e' && *aux++ == 0) {
				if (!FALSE_TOKEN) {
					FALSE_TOKEN = b_lexical_read_token(B_LEXICAL_FALSE_ID, "false");
				}
				free(text);
				return FALSE_TOKEN;
			}
			break;
		}
		case 'o': {
			if (*aux++ == 'r' && *aux++ == 0) {
				if (!FOR_TOKEN) {
					FOR_TOKEN = b_lexical_read_token(B_LEXICAL_FOR_ID, "for");
				}
				free(text);
				return FOR_TOKEN;
			}
			break;
		}
		}
		break;
	}
	case 'i': {
		if (*aux++ == 'f' && *aux++ == 0) {
			if (!IF_TOKEN) {
				IF_TOKEN = b_lexical_read_token(B_LEXICAL_IF_ID, "if");
			}
			free(text);
			return IF_TOKEN;
		}
		break;
	}
	case 'o': {
		if (*aux++ == 'r' && *aux++ == 0) {
			if (!OR_TOKEN) {
				OR_TOKEN = b_lexical_read_token(B_LEXICAL_OR_ID, "or");
			}
			free(text);
			return OR_TOKEN;
		}
		break;
	}
	case 'r': {
		if (*aux++ == 'e') {
			switch (*aux++) {
			case 'p': {
				if (*aux++ == 'e' && *aux++ == 'a' && *aux++ == 't' && *aux++ == 0) {
					if (!REPEAT_TOKEN) {
						REPEAT_TOKEN = b_lexical_read_token(B_LEXICAL_REPEAT_ID, "repeat");
					}
					free(text);
					return REPEAT_TOKEN;
				}
				break;
			}
			case 't': {
				if (*aux++ == 'u' && *aux++ == 'r' && *aux++ == 'n' && *aux++ == 0) {
					if (!RETURN_TOKEN) {
						RETURN_TOKEN = b_lexical_read_token(B_LEXICAL_RETURN_ID, "return");
					}
					free(text);
					return RETURN_TOKEN;
				}
				break;
			}
			}

		}
		break;
	}
	case 't': {
		switch (*aux++) {
		case 'h': {
			if (*aux++ == 'i' && *aux++ == 's' && *aux++ == 0) {
				if (!THIS_TOKEN) {
					THIS_TOKEN = b_lexical_read_token(B_LEXICAL_THIS_ID, "this");
				}
				free(text);
				return THIS_TOKEN;
			}
			break;
		}
		case 'r': {
			if (*aux++ == 'u' && *aux++ == 'e' && *aux++ == 0) {
				if (!TRUE_TOKEN) {
					TRUE_TOKEN = b_lexical_read_token(B_LEXICAL_TRUE_ID, "true");
				}
				free(text);
				return TRUE_TOKEN;
			}
			break;
		}
		}
		break;
	}
	case 'w': {
		if (*aux++ == 'h' && *aux++ == 'i' && *aux++ == 'l' && *aux++ == 'e' && *aux++ == 0) {
			if (!WHILE_TOKEN) {
				WHILE_TOKEN = b_lexical_read_token(B_LEXICAL_WHILE_ID, "while");
			}
			free(text);
			return WHILE_TOKEN;
		}
		break;
	}
	}
	struct b_token_t* token = b_lexical_token_pchar_new(B_LEXICAL_ID_ID, text);
	if (!token) {
		free(text);
		return 0;
	}
	return token;
}

static struct b_token_t* b_lexical_read_number(struct b_lexical_t* self) {
	int c = b_lexical_look(self);
	while (c <= 32) {
		if (c < 0) {
			return 0;
		}
		c = b_lexical_next(self);
	}
	double value = 0;
	int dot = 10;
	value = c - '0';
	c = b_lexical_next(self);
	while (b_lexical_is_number_start(c)) {
		value = 10 * value + (c - '0');
		c = b_lexical_next(self);
	}
	if (c == '.') {
		c = b_lexical_next(self);
		while (b_lexical_is_number_start(c)) {
			value += (double) (c - '0') / dot;
			dot *= 10;
			c = b_lexical_next(self);
		}
	}
	struct b_token_t* token = b_lexical_token_double_new(value);
	if (!token) {
		return 0;
	}
	return token;
}

static struct b_token_t* b_lexical_read_string(struct b_lexical_t* self) {
	int c = b_lexical_look(self);
	while (c <= 32) {
		if (c < 0) {
			return 0;
		}
		c = b_lexical_next(self);
	}
	int length = 1;
	wchar_t* text = (wchar_t*) calloc(length + 1, sizeof(wchar_t));
	wchar_t* aux = text;
	if (!text) {
		return 0;
	}
	c = b_lexical_next(self);
	while (!b_lexical_is_string_start(c)) {
		if (text + length == aux) {
			length *= 2;
			wchar_t* data = (wchar_t*) realloc(text, (length + 1) * sizeof(wchar_t));
			if (!data) {
				free(text);
				return 0;
			}
			text = data;
			aux = text + length / 2;
		}
		if (c == '\\') {
			switch (b_lexical_next(self)) {
			case 'n': {
				*aux++ = '\n';
				break;
			}
			case 'r': {
				*aux++ = '\r';
				break;
			}
			case 't': {
				*aux++ = '\t';
				break;
			}
			case 'f': {
				*aux++ = '\f';
				break;
			}
			case 'b': {
				*aux++ = '\b';
				break;
			}
			case '\\': {
				*aux++ = '\\';
				break;
			}
			default: {
				free(text);
				return 0;
			}
			}
		} else {
			*aux++ = c;
		}
		c = b_lexical_next(self);
	}
	*aux = 0;
	b_lexical_next(self);
	struct b_token_t* token = b_lexical_token_pwchar_new(text);
	if (!token) {
		free(text);
		return 0;
	}
	return token;
}

static struct b_token_t* b_lexical_read_symbol(struct b_lexical_t* self) {
	int c = b_lexical_look(self);
	while (c <= 32) {
		if (c < 0) {
			return 0;
		}
		c = b_lexical_next(self);
	}
	switch (c) {
	case '!': {
		c = b_lexical_next(self);
		if (c == '=') {
			c = b_lexical_next(self);
			if (!NE_TOKEN) {
				NE_TOKEN = b_lexical_read_token(B_LEXICAL_NE_ID, "!=");
			}
			return NE_TOKEN;
		} else {
			if (!NOT_TOKEN) {
				NOT_TOKEN = b_lexical_read_token('!', "!");
			}
			return NOT_TOKEN;
		}
	}
	case '=': {
		c = b_lexical_next(self);
		if (c == '=') {
			c = b_lexical_next(self);
			if (!EQ_TOKEN) {
				EQ_TOKEN = b_lexical_read_token(B_LEXICAL_EQ_ID, "==");
			}
			return EQ_TOKEN;
		} else {
			if (!ASSIGN_TOKEN) {
				ASSIGN_TOKEN = b_lexical_read_token('=', "=");
			}
			return ASSIGN_TOKEN;
		}
	}
	case '-': {
		c = b_lexical_next(self);
		if (c == '-') {
			c = b_lexical_next(self);
			if (!DEC_TOKEN) {
				DEC_TOKEN = b_lexical_read_token(B_LEXICAL_DEC_ID, "--");
			}
			return DEC_TOKEN;
		} else {
			if (!SUB_TOKEN) {
				SUB_TOKEN = b_lexical_read_token('-', "-");
			}
			return SUB_TOKEN;
		}
	}
	case '+': {
		c = b_lexical_next(self);
		if (c == '+') {
			c = b_lexical_next(self);
			if (!INC_TOKEN) {
				INC_TOKEN = b_lexical_read_token(B_LEXICAL_INC_ID, "++");
			}
			return INC_TOKEN;
		} else {
			if (!SUM_TOKEN) {
				SUM_TOKEN = b_lexical_read_token('+', "+");
			}
			return SUM_TOKEN;
		}
	}
	case '>': {
		c = b_lexical_next(self);
		if (c == '=') {
			c = b_lexical_next(self);
			if (!GE_TOKEN) {
				GE_TOKEN = b_lexical_read_token(B_LEXICAL_GE_ID, ">=");
			}
			return GE_TOKEN;
		} else {
			if (!GT_TOKEN) {
				GT_TOKEN = b_lexical_read_token('>', ">");
			}
			return GT_TOKEN;
		}
	}
	case '<': {
		c = b_lexical_next(self);
		if (c == '=') {
			c = b_lexical_next(self);
			if (!LE_TOKEN) {
				LE_TOKEN = b_lexical_read_token(B_LEXICAL_LE_ID, "<=");
			}
			return LE_TOKEN;
		} else {
			if (!LT_TOKEN) {
				LT_TOKEN = b_lexical_read_token('<', "<");
			}
			return LT_TOKEN;
		}
	}
	}
	b_lexical_next(self);
	char* data = (char*) calloc(2, sizeof(char));
	if (!data) {
		return 0;
	}
	data[0] = c;
	return b_lexical_token_pchar_new(c, data);
}

struct b_token_t* b_lexical_read(struct b_lexical_t* self) {
	if (b_lexical_look(self) == -1) {
		return 0;
	} else if (b_lexical_is_word(self)) {
		return b_lexical_read_word(self);
	} else if (b_lexical_is_number(self)) {
		return b_lexical_read_number(self);
	} else if (b_lexical_is_string(self)) {
		return b_lexical_read_string(self);
	} else {
		return b_lexical_read_symbol(self);
	}
}

int b_lexical_token_id(struct b_token_t* self) {
	return self->tag;
}

const char* b_lexical_token_pchar(struct b_token_t* self) {
	return ((struct b_token_pchar_t*) self)->value;
}

const wchar_t* b_lexical_token_utf(struct b_token_t* self) {
	return ((struct b_token_pwchar_t*) self)->value;
}

double b_lexical_token_double(struct b_token_t* self) {
	return ((struct b_token_double_t*) self)->value;
}

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

struct b_sdl_t {
	SDL_Surface* surface;
	struct {
		void (*paint)();
		void (*key_pressed)();
		void (*key_released)();
	} func;
	int color;
};

struct b_sdl_surface_t {
	SDL_Surface* surface;
};

#define PITCH(surface) (surface->pitch / 4)

static struct b_sdl_t* sdl = 0;

b_sdl_error_t b_sdl_init(int width, int height) {
	if (sdl) {
		return 0;
	}
	sdl = (struct b_sdl_t*) calloc(1, sizeof(struct b_sdl_t));
	if (!sdl) {
		return 1;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0 && TTF_Init() != 0) {
		TTF_Quit();
		SDL_Quit();
		free(sdl);
		sdl = 0;
		return 2;
	}
	atexit(SDL_Quit);
	atexit(TTF_Quit);
	TTF_ByteSwappedUNICODE(1);
	sdl->surface = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE);
	if (!sdl->surface) {
		free(sdl);
		sdl = 0;
		return 4;
	}
	SDL_EnableKeyRepeat(0, 0);
	SDL_EnableUNICODE(1);
	return 0;
}

void b_sdl_init_video_func(void (*paint)()) {
	sdl->func.paint = paint;
}

void b_sdl_init_keyboard_func(void (*key_pressed)(), void (*key_released)()) {
	sdl->func.key_pressed = key_pressed;
	sdl->func.key_released = key_released;
}

void b_sdl_graphic_draw_circle(int x, int y, int r) {
	SDL_Surface* surface = sdl->surface;
	int surfaceWidth = sdl->surface->w;
	int surfaceHeight = sdl->surface->h;
	int c = sdl->color;
	unsigned int* pixels = (unsigned int*) surface->pixels;
	int i, j;
	for (i = 0; i < 2 * r; i++) {
		if ((y - r + i) >= 0 && (y - r + i) < surfaceHeight) {
			int len = (int) sqrt((float) (r * r - (r - i) * (r - i))) * 2;
			int xofs = x - len / 2;
			if (xofs < 0) {
				len += xofs;
				xofs = 0;
			}
			if (xofs + len >= surfaceWidth) {
				len -= (xofs + len) - surfaceWidth;
			}
			int ofs = (y - r + i) * PITCH(surface) + xofs;
			unsigned int* row = pixels + ofs;
			for (j = 0; j < len; j++) {
				*row++ = c;
			}
		}
	}
}

void b_sdl_graphic_draw_rect(int x, int y, int width, int height) {
	SDL_Surface* surface = sdl->surface;
	int surfaceWidth = sdl->surface->w;
	int surfaceHeight = sdl->surface->h;
	int c = sdl->color;
	unsigned int* pixels = (unsigned int*) surface->pixels;
	int i, j;
	for (i = 0; i < height; i++) {
		if ((y + i) >= 0 && (y + i) < surfaceHeight) {
			int len = width;
			int xofs = x;
			if (xofs < 0) {
				len += xofs;
				xofs = 0;
			}
			if (xofs + len >= surfaceWidth) {
				len -= (xofs + len) - surfaceWidth;
			}
			int ofs = (i + y) * PITCH(surface) + xofs;
			unsigned int* row = pixels + ofs;
			for (j = 0; j < len; j++) {
				*row++ = c;
			}
		}
	}
}

void b_sdl_graphic_color(int r, int g, int b) {
	sdl->color = SDL_MapRGB(sdl->surface->format, r, g, b);
}

const char* b_sdl_error() {
	return SDL_GetError();
}

static void b_sdl_render(int width, int height) {
	SDL_Surface* screen = sdl->surface;
	if (SDL_MUSTLOCK(screen)) {
		if (SDL_LockSurface(screen) < 0) {
			return;
		}
	}
	if (sdl->func.paint) {
		sdl->func.paint();
	}
	if (SDL_MUSTLOCK(screen)) {
		SDL_UnlockSurface(screen);
	}
	SDL_UpdateRect(screen, 0, 0, width, height);
}

b_sdl_error_t b_sdl_loop() {
	if (!sdl) {
		return 1;
	}
	for (;;) {
		b_sdl_render(sdl->surface->w, sdl->surface->h);
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (sdl->func.key_pressed) {
					sdl->func.key_pressed();
				}
				break;
			case SDL_KEYUP:
				if (sdl->func.key_released) {
					sdl->func.key_released();
				}
				break;
			case SDL_QUIT:
				return 0;
			}
		}
	}
	return 0;
}

unsigned char* b_sdl_keyboard_state() {
	return SDL_GetKeyState(0);
}

unsigned char b_sdl_keyboard_is_shift_pressed() {
	return SDL_GetModState() & KMOD_SHIFT;
}

unsigned char b_sdl_keyboard_is_ctrl_pressed() {
	return SDL_GetModState() & KMOD_CTRL;
}

unsigned char b_sdl_keyboard_is_alt_pressed() {
	return SDL_GetModState() & KMOD_ALT;
}

unsigned char b_sdl_keyboard_is_meta_pressed() {
	return SDL_GetModState() & KMOD_META;
}

struct b_sdl_font_t {
	TTF_Font *font;
};

const char* b_sdl_ttf_error() {
	return TTF_GetError();
}

struct b_sdl_font_t* b_sdl_ttf_new_file(char* path, int size) {
	struct b_sdl_font_t* self = (struct b_sdl_font_t*) calloc(1,
			sizeof(struct b_sdl_font_t));
	if (!self) {
		return 0;
	}
	self->font = TTF_OpenFont(path, size);
	if (!self->font) {
		free(self);
		return 0;
	}
	return self;
}

void b_sdl_ttf_free(struct b_sdl_font_t* self) {
	TTF_CloseFont(self->font);
	free(self);
}

unsigned char b_sdl_ttf_is_bold(struct b_sdl_font_t* self) {
	return (TTF_GetFontStyle(self->font) & TTF_STYLE_BOLD) != 0;
}

unsigned char b_sdl_ttf_is_italic(struct b_sdl_font_t* self) {
	return (TTF_GetFontStyle(self->font) & TTF_STYLE_ITALIC) != 0;
}

unsigned char b_sdl_ttf_is_underline(struct b_sdl_font_t* self) {
	return (TTF_GetFontStyle(self->font) & TTF_STYLE_UNDERLINE) != 0;
}

unsigned char b_sdl_ttf_is_strikethrough(struct b_sdl_font_t* self) {
	return (TTF_GetFontStyle(self->font) & TTF_STYLE_STRIKETHROUGH) != 0;
}

void b_sdl_ttf_set_style(struct b_sdl_font_t* self, unsigned char bold,
		unsigned char italic, unsigned char underline,
		unsigned char strikethrough) {
	if (bold || italic || underline || strikethrough) {
		TTF_SetFontStyle(self->font,
				(bold ? TTF_STYLE_BOLD : 0) | (italic ? TTF_STYLE_ITALIC : 0)
						| (underline ? TTF_STYLE_UNDERLINE : 0)
						| (strikethrough ? TTF_STYLE_STRIKETHROUGH : 0));
	} else {
		TTF_SetFontStyle(self->font, TTF_STYLE_NORMAL);
	}
}

int b_sdl_ttf_height(struct b_sdl_font_t* self) {
	return TTF_FontHeight(self->font);
}

int b_sdl_ttf_ascent(struct b_sdl_font_t* self) {
	return TTF_FontAscent(self->font);
}

int b_sdl_ttf_descent(struct b_sdl_font_t* self) {
	return TTF_FontDescent(self->font);
}

int b_sdl_ttf_line_skip(struct b_sdl_font_t* self) {
	return TTF_FontLineSkip(self->font);
}

unsigned char b_sdl_ttf_is_monospace(struct b_sdl_font_t* self) {
	return TTF_FontFaceIsFixedWidth(self->font) > 0;
}

b_sdl_error_t b_sdl_text_width(struct b_sdl_font_t* self, const char* text,
		int* w, int* h) {
	return TTF_SizeUTF8(self->font, text, w, h) != 0;
}

b_sdl_surface_t* b_sdl_text_draw(struct b_sdl_font_t* self, const char* text,
		int c) {
	unsigned char r, g, b;
	SDL_GetRGB(c, sdl->surface->format, &r, &g, &b);
	SDL_Color color = { r, g, b }, bgcolor = { 0xff, 0xff, 0xff };
	SDL_Surface* text_surface = TTF_RenderUTF8_Shaded(self->font, text, color,
			bgcolor);
	if (!text_surface) {
		return 0;
	}
	b_sdl_surface_t* surface = (b_sdl_surface_t*) calloc(1,
			sizeof(b_sdl_surface_t));
	if (!surface) {
		SDL_FreeSurface(text_surface);
		return 0;
	}
	surface->surface = text_surface;
	return surface;
}

void b_sdl_surface_draw(struct b_sdl_surface_t* src, int srcX, int srcY,
		int srcW, int srcH, struct b_sdl_surface_t* dest, int destX, int destY,
		int destW, int destH) {
	SDL_Rect src_rect = { srcX, srcY, srcW, srcY };
	SDL_Rect dest_rect = { destX, destY, destW, destY };
	SDL_BlitSurface(src->surface, &src_rect, sdl->surface, &dest_rect);
}

struct b_syntax_t {
	void* data;
	void* (*read_func)(void* data);
	void (*token_free_func)(void* token);
	int (*token_id_func)(void* token);
	const char* (*token_pchar_func)(void* token);
	const wchar_t*(*token_utf_func)(void* token);
	double (*token_double_func)(void* token);
	void* look;
};

struct b_syntax_t* b_syntax_new(void* data, void* (*read_func)(void* data), void (*token_free_func)(void* token), int (*token_id_func)(void* token), const char* (*token_pchar_func)(void*),
		const wchar_t*(*token_utf_func)(void* token), double (*token_double_func)(void* token)) {
	struct b_syntax_t* self = (struct b_syntax_t*) calloc(1, sizeof(struct b_syntax_t));
	if (!self) {
		return 0;
	}
	self->data = data;
	self->read_func = read_func;
	self->token_free_func = token_free_func;
	self->token_id_func = token_id_func;
	self->token_pchar_func = token_pchar_func;
	self->token_utf_func = token_utf_func;
	self->token_double_func = token_double_func;
	return self;
}

void b_syntax_free(struct b_syntax_t* self) {
	free(self);
}

struct b_treemap_entry_t {
	void* key;
	void* value;
	struct b_treemap_entry_t* left;
	struct b_treemap_entry_t* right;
	struct b_treemap_entry_t* parent;
	unsigned char color;
};

struct b_treemap_t {
	int (*compareFunc)(void*, void*);
	void (*freeKeyFunc)(void*);
	void (*freeValueFunc)(void*);
	struct b_treemap_entry_t* root;
	int size;
};

#define b_treemap_entry_red  0
#define b_treemap_entry_black 1
#define b_treemap_entry_set_color(p,c) if (p) { p->color = c; }
#define b_treemap_entry_color(p) (!p ? b_treemap_entry_black: p->color)
#define b_treemap_entry_right(p) (!p ? 0: p->right)
#define b_treemap_entry_left(p) (!p ? 0: p->left)
#define b_treemap_entry_parent(p) (!p ? 0: p->parent)

struct b_treemap_t* b_treemap_new(int (*compareFunc)(void*, void*), void (*freeKeyFunc)(void*), void (*freeValueFunc)(void*)) {
	struct b_treemap_t* data = (struct b_treemap_t*) calloc(1, sizeof(struct b_treemap_t));
	if (!data) {
		return 0;
	}
	data->compareFunc = compareFunc;
	data->freeKeyFunc = freeKeyFunc;
	data->freeValueFunc = freeValueFunc;
	return data;
}

void b_treemap_free(struct b_treemap_t* data) {
	b_treemap_clear(data);
	free(data);
}

static int b_treemap_int_compare(void* o1, void* o2) {
	return o1 - o2;
}

struct b_treemap_t* b_treemap_new_int(void (*freeValueFunc)(void*)) {
	return b_treemap_new(b_treemap_int_compare, 0, freeValueFunc);
}

static int b_treemap_vchar_compare(void* o1, void* o2) {
	return strcmp(o1, o2);
}

struct b_treemap_t* b_treemap_new_vchar(unsigned char freeKey, void (*freeValueFunc)(void*)) {
	return b_treemap_new(b_treemap_vchar_compare, freeKey ? free : 0, freeValueFunc);
}

static void b_treemap_rotate_left(struct b_treemap_t* data, struct b_treemap_entry_t* p) {
	if (p) {
		struct b_treemap_entry_t* r = p->right;
		p->right = r->left;
		if (r->left) {
			r->left->parent = p;
		}
		r->parent = p->parent;
		if (!p->parent) {
			data->root = r;
		} else if (p->parent->left == p) {
			p->parent->left = r;
		} else {
			p->parent->right = r;
		}
		r->left = p;
		p->parent = r;
	}
}

static void b_treemap_rotate_right(struct b_treemap_t* data, struct b_treemap_entry_t* p) {
	if (p) {
		struct b_treemap_entry_t* l = p->left;
		p->left = l->right;
		if (l->right) {
			l->right->parent = p;
		}
		l->parent = p->parent;
		if (!p->parent) {
			data->root = l;
		} else if (p->parent->right == p) {
			p->parent->right = l;
		} else {
			p->parent->left = l;
		}
		l->right = p;
		p->parent = l;
	}
}

static void b_treemap_fix_after_insertion(struct b_treemap_t* data, struct b_treemap_entry_t* x) {
	x->color = b_treemap_entry_red;
	while (x && x != data->root && x->parent->color == b_treemap_entry_red) {
		if (b_treemap_entry_parent(x) == b_treemap_entry_left(b_treemap_entry_parent(b_treemap_entry_parent(x)))) {
			struct b_treemap_entry_t* y = b_treemap_entry_right(b_treemap_entry_parent(b_treemap_entry_parent(x)));
			if (b_treemap_entry_color(y) == b_treemap_entry_red) {
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_black);
				b_treemap_entry_set_color(y, b_treemap_entry_black);
				b_treemap_entry_set_color( b_treemap_entry_parent(b_treemap_entry_parent(x)), b_treemap_entry_red);
				x = b_treemap_entry_parent(b_treemap_entry_parent(x));
			} else {
				if (x == b_treemap_entry_right(b_treemap_entry_parent(x))) {
					x = b_treemap_entry_parent(x);
					b_treemap_rotate_left(data, x);
				}
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_black);
				b_treemap_entry_set_color( b_treemap_entry_parent(b_treemap_entry_parent(x)), b_treemap_entry_red);
				b_treemap_rotate_right(data, b_treemap_entry_parent(b_treemap_entry_parent(x)));
			}
		} else {
			struct b_treemap_entry_t* y = b_treemap_entry_left(b_treemap_entry_parent(b_treemap_entry_parent(x)));
			if (b_treemap_entry_color(y) == b_treemap_entry_red) {
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_black);
				b_treemap_entry_set_color(y, b_treemap_entry_black);
				b_treemap_entry_set_color( b_treemap_entry_parent(b_treemap_entry_parent(x)), b_treemap_entry_red);
				x = b_treemap_entry_parent(b_treemap_entry_parent(x));
			} else {
				if (x == b_treemap_entry_left(b_treemap_entry_parent(x))) {
					x = b_treemap_entry_parent(x);
					b_treemap_rotate_right(data, x);
				}
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_black);
				b_treemap_entry_set_color( b_treemap_entry_parent(b_treemap_entry_parent(x)), b_treemap_entry_red);
				b_treemap_rotate_left(data, b_treemap_entry_parent(b_treemap_entry_parent(x)));
			}
		}
	}
	data->root->color = b_treemap_entry_black;
}

b_treemap_error_t b_treemap_put(struct b_treemap_t* data, void* key, void* value) {
	struct b_treemap_entry_t* aux = data->root;
	if (!aux) {
		struct b_treemap_entry_t* entry = (struct b_treemap_entry_t*) calloc(1, sizeof(struct b_treemap_entry_t));
		if (!entry) {
			return 1;
		}
		entry->key = key;
		entry->value = value;
		data->root = entry;
		data->size = 1;
		return 0;
	}
	int cmp;
	struct b_treemap_entry_t* parent;
	do {
		parent = aux;
		cmp = data->compareFunc(key, aux->key);
		if (cmp < 0) {
			aux = aux->left;
		} else if (cmp > 0) {
			aux = aux->right;
		} else {
			void* result = aux->value;
			aux->value = value;
			if (data->freeValueFunc) {
				data->freeValueFunc(result);
			}
			return 0;
		}
	} while (aux);
	struct b_treemap_entry_t* e = (struct b_treemap_entry_t*) calloc(1, sizeof(struct b_treemap_entry_t));
	if (!e) {
		return 1;
	}
	e->key = key;
	e->value = value;
	e->parent = parent;
	if (cmp < 0) {
		parent->left = e;
	} else {
		parent->right = e;
	}
	b_treemap_fix_after_insertion(data, e);
	data->size++;
	return 0;
}

b_treemap_error_t b_treemap_put_int(struct b_treemap_t* data, int key, void* value) {
	void* p = 0;
	p += key;
	return b_treemap_put(data, p, value);
}

static struct b_treemap_entry_t* b_treemap_get_entry(struct b_treemap_t* data, void* key) {
	struct b_treemap_entry_t* p = data->root;
	while (p) {
		int cmp = data->compareFunc(key, p->key);
		if (cmp < 0) {
			p = p->left;
		} else if (cmp > 0) {
			p = p->right;
		} else {
			return p;
		}
	}
	return 0;
}

void* b_treemap_get(struct b_treemap_t* data, void* key) {
	struct b_treemap_entry_t* p = b_treemap_get_entry(data, key);
	return !p ? 0 : p->value;
}

void* b_treemap_get_int(struct b_treemap_t* data, int key) {
	void* p = 0;
	p += key;
	return b_treemap_get(data, p);
}

int b_treemap_size(struct b_treemap_t* data) {
	return data->size;
}

static struct b_treemap_entry_t* successor(struct b_treemap_entry_t* t) {
	if (!t) {
		return 0;
	} else if (t->right) {
		struct b_treemap_entry_t* p = t->right;
		while (p->left)
			p = p->left;
		return p;
	} else {
		struct b_treemap_entry_t* p = t->parent;
		struct b_treemap_entry_t* ch = t;
		while (p && ch == p->right) {
			ch = p;
			p = p->parent;
		}
		return p;
	}
}

void fixAfterDeletion(struct b_treemap_t* data, struct b_treemap_entry_t* x) {
	while (x != data->root && b_treemap_entry_color(x) == b_treemap_entry_black) {
		if (x == b_treemap_entry_left(b_treemap_entry_parent(x))) {
			struct b_treemap_entry_t* sib = b_treemap_entry_right(b_treemap_entry_parent(x));
			if (b_treemap_entry_color(sib) == b_treemap_entry_red) {
				b_treemap_entry_set_color(sib, b_treemap_entry_black);
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_red);
				b_treemap_rotate_left(data, b_treemap_entry_parent(x));
				sib = b_treemap_entry_right(b_treemap_entry_parent(x));
			}
			if (b_treemap_entry_color(b_treemap_entry_left(sib)) == b_treemap_entry_black && b_treemap_entry_color(b_treemap_entry_right(sib)) == b_treemap_entry_black) {
				b_treemap_entry_set_color(sib, b_treemap_entry_red);
				x = b_treemap_entry_parent(x);
			} else {
				if (b_treemap_entry_color(b_treemap_entry_right(sib)) == b_treemap_entry_black) {
					b_treemap_entry_set_color(b_treemap_entry_left(sib), b_treemap_entry_black);
					b_treemap_entry_set_color(sib, b_treemap_entry_red);
					b_treemap_rotate_right(data, sib);
					sib = b_treemap_entry_right(b_treemap_entry_parent(x));
				}
				b_treemap_entry_set_color(sib, b_treemap_entry_color(b_treemap_entry_parent(x)));
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_black);
				b_treemap_entry_set_color(b_treemap_entry_right(sib), b_treemap_entry_black);
				b_treemap_rotate_left(data, b_treemap_entry_parent(x));
				x = data->root;
			}
		} else { // symmetric
			struct b_treemap_entry_t* sib = b_treemap_entry_left(b_treemap_entry_parent(x));
			if (b_treemap_entry_color(sib) == b_treemap_entry_red) {
				b_treemap_entry_set_color(sib, b_treemap_entry_black);
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_red);
				b_treemap_rotate_right(data, b_treemap_entry_parent(x));
				sib = b_treemap_entry_left(b_treemap_entry_parent(x));
			}
			if (b_treemap_entry_color(b_treemap_entry_right(sib)) == b_treemap_entry_black && b_treemap_entry_color(b_treemap_entry_left(sib)) == b_treemap_entry_black) {
				b_treemap_entry_set_color(sib, b_treemap_entry_red);
				x = b_treemap_entry_parent(x);
			} else {
				if (b_treemap_entry_color(b_treemap_entry_left(sib)) == b_treemap_entry_black) {
					b_treemap_entry_set_color(b_treemap_entry_right(sib), b_treemap_entry_black);
					b_treemap_entry_set_color(sib, b_treemap_entry_red);
					b_treemap_rotate_left(data, sib);
					sib = b_treemap_entry_left(b_treemap_entry_parent(x));
				}
				b_treemap_entry_set_color(sib, b_treemap_entry_color(b_treemap_entry_parent(x)));
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_black);
				b_treemap_entry_set_color(b_treemap_entry_left(sib), b_treemap_entry_black);
				b_treemap_rotate_right(data, b_treemap_entry_parent(x));
				x = data->root;
			}
		}
	}

	b_treemap_entry_set_color(x, b_treemap_entry_black);
}

static void deleteEntry(struct b_treemap_t* data, struct b_treemap_entry_t* p) {
	data->size--;
	if (p->left && p->right) {
		struct b_treemap_entry_t* s = successor(p);
		p->key = s->key;
		p->value = s->value;
		p = s;
	}
	struct b_treemap_entry_t* replacement = (p->left ? p->left : p->right);
	if (replacement) {
		replacement->parent = p->parent;
		if (!p->parent) {
			data->root = replacement;
		} else if (p == p->parent->left) {
			p->parent->left = replacement;
		} else {
			p->parent->right = replacement;
		}
		p->left = p->right = p->parent = 0;
		if (p->color == b_treemap_entry_black) {
			b_treemap_fix_after_insertion(data, replacement);
		}
	} else if (!p->parent) {
		data->root = 0;
	} else {
		if (p->color == b_treemap_entry_black) {
			b_treemap_fix_after_insertion(data, p);
		}
		if (p->parent) {
			if (p == p->parent->left) {
				p->parent->left = 0;
			} else if (p == p->parent->right) {
				p->parent->right = 0;
			}
			p->parent = 0;
		}
	}
}

void b_treemap_remove(struct b_treemap_t* data, void* key) {
	struct b_treemap_entry_t* p = b_treemap_get_entry(data, key);
	if (p) {
		deleteEntry(data, p);
		if (data->freeKeyFunc) {
			data->freeKeyFunc(p->key);
		}
		if (data->freeValueFunc) {
			data->freeValueFunc(p->value);
		}
		free(p);
	}
}

void b_treemap_remove_int(struct b_treemap_t* data, int key) {
	void* p = 0;
	p += key;
	b_treemap_remove(data, p);
}

static void b_treemap_remove_entry(struct b_treemap_t* data, struct b_treemap_entry_t* e) {
	if (data->freeKeyFunc) {
		data->freeKeyFunc(e->key);
	}
	if (data->freeValueFunc) {
		data->freeValueFunc(e->value);
	}
	if (e->left) {
		b_treemap_remove_entry(data, e->left);
	}
	if (e->right) {
		b_treemap_remove_entry(data, e->right);
	}
	free(e);
}

void b_treemap_clear(struct b_treemap_t* data) {
	if (data->root) {
		b_treemap_remove_entry(data, data->root);
	}
	data->root = 0;
	data->size = 0;
}

struct b_vm_t {
	int version;
	int maxStack;
};

struct b_vm_t* b_vm_new_default() {
	struct b_vm_t* self = (struct b_vm_t*) calloc(1, sizeof(struct b_vm_t));
	if (!self) {
		return 0;
	}
	self->maxStack = 1024;
	return self;
}

static unsigned int b_vm_read_index(unsigned char** bytes) {
	unsigned int i1 = *((*bytes)++);
	if (i1 <= 0x7F) {
		return i1;
	} else if ((i1 >> 5) == 0x6) {
		int i2 = *((*bytes)++);
		return ((i1 & 0x1F) << 6) + (i2 & 0x3F);
	} else if ((i1 >> 4) == 0xE) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		return ((i1 & 0xF) << 12) + ((i2 & 0x3F) << 6) + (i3 & 0x3F);
	} else if ((i1 >> 3) == 0x1E) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		return ((i1 & 0x7) << 18) + ((i2 & 0x3F) << 12) + ((i3 & 0x3F) << 6)
				+ (i4 & 0x3F);
	} else if ((i1 >> 2) == 0x3E) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		int i5 = *((*bytes)++);
		return ((i1 & 0x3) << 24) + ((i2 & 0x3F) << 18) + ((i3 & 0x3F) << 12)
				+ ((i4 & 0x3F) << 6) + (i5 & 0x3F);
	} else if ((i1 >> 1) == 0x7E) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		int i5 = *((*bytes)++);
		int i6 = *((*bytes)++);
		return ((i1 & 0x1) << 30) + ((i2 & 0x3F) << 24) + ((i3 & 0x3F) << 18)
				+ ((i4 & 0x3F) << 12) + ((i5 & 0x3F) << 6) + (i6 & 0x3F);
	} else {
		return 0;
	}
}

unsigned int b_vm_execute(unsigned char* opcodes, int length, void* result,
		void* exception) {

	switch (*opcodes++) {
	case B_VM_OPCODE_STACK_INC: {
		unsigned int index = b_vm_read_index(&opcodes);
		break;
	}
	case B_VM_OPCODE_STACK_DEC: {
		unsigned int index = b_vm_read_index(&opcodes);
		break;
	}
	case B_VM_OPCODE_STACK_STRING: {
		break;
	}
	case B_VM_OPCODE_STACK_DOUBLE: {
		break;
	}
	case B_VM_OPCODE_STACK_INTEGER: {
		break;
	}
	case B_VM_OPCODE_STACK_TRUE: {
		break;
	}
	case B_VM_OPCODE_STACK_FALSE: {
		break;
	}
	case B_VM_OPCODE_STACK_NULL: {
		break;
	}
	case B_VM_OPCODE_NUMBER_SUM: {
		break;
	}
	case B_VM_OPCODE_NUMBER_SUB: {
		break;
	}
	case B_VM_OPCODE_NUMBER_MUL: {
		break;
	}
	case B_VM_OPCODE_NUMBER_DIV: {
		break;
	}
	case B_VM_OPCODE_NUMBER_EQ: {
		break;
	}
	case B_VM_OPCODE_NUMBER_NEQ: {
		break;
	}
	case B_VM_OPCODE_NUMBER_COMPARE: {
		break;
	}
	case B_VM_OPCODE_NUMBER_GT: {
		break;
	}
	case B_VM_OPCODE_NUMBER_GE: {
		break;
	}
	case B_VM_OPCODE_NUMBER_LT: {
		break;
	}
	case B_VM_OPCODE_NUMBER_LE: {
		break;
	}
	case B_VM_OPCODE_NUMBER_MOD: {
		break;
	}
	case B_VM_OPCODE_NUMBER_INT_DIV: {
		break;
	}
	case B_VM_OPCODE_NUMBER_TO_STRING: {
		break;
	}
	case B_VM_OPCODE_NUMBER_IS_NAN: {
		break;
	}
	case B_VM_OPCODE_NUMBER_IS_INFINITY: {
		break;
	}
	case B_VM_OPCODE_NUMBER_HASH: {
		break;
	}
	case B_VM_OPCODE_NUMBER_NEG: {
		break;
	}
	case B_VM_OPCODE_NUMBER_INC: {
		break;
	}
	case B_VM_OPCODE_NUMBER_DEC: {
		break;
	}
	case B_VM_OPCODE_INTEGER_SUM: {
		break;
	}
	case B_VM_OPCODE_INTEGER_SUB: {
		break;
	}
	case B_VM_OPCODE_INTEGER_MUL: {
		break;
	}
	case B_VM_OPCODE_INTEGER_DIV: {
		break;
	}
	case B_VM_OPCODE_INTEGER_EQ: {
		break;
	}
	case B_VM_OPCODE_INTEGER_NEQ: {
		break;
	}
	case B_VM_OPCODE_INTEGER_COMPARE: {
		break;
	}
	case B_VM_OPCODE_INTEGER_GT: {
		break;
	}
	case B_VM_OPCODE_INTEGER_GE: {
		break;
	}
	case B_VM_OPCODE_INTEGER_LT: {
		break;
	}
	case B_VM_OPCODE_INTEGER_LE: {
		break;
	}
	case B_VM_OPCODE_INTEGER_AND: {
		break;
	}
	case B_VM_OPCODE_INTEGER_OR: {
		break;
	}
	case B_VM_OPCODE_INTEGER_MOD: {
		break;
	}
	case B_VM_OPCODE_INTEGER_TO_STRING: {
		break;
	}
	case B_VM_OPCODE_INTEGER_HASH: {
		break;
	}
	case B_VM_OPCODE_INTEGER_NEG: {
		break;
	}
	case B_VM_OPCODE_INTEGER_INC: {
		break;
	}
	case B_VM_OPCODE_INTEGER_DEC: {
		break;
	}
	case B_VM_OPCODE_BOOLEAN_NOT: {
		break;
	}
	case B_VM_OPCODE_BOOLEAN_AND: {
		break;
	}
	case B_VM_OPCODE_BOOLEAN_OR: {
		break;
	}
	case B_VM_OPCODE_STRING_SUM: {
		break;
	}
	case B_VM_OPCODE_CONTROL_JUMP: {
		break;
	}
	case B_VM_OPCODE_CONTROL_JUMP_TRUE: {
		break;
	}
	case B_VM_OPCODE_CONTROL_JUMP_FALSE: {
		break;
	}
	case B_VM_OPCODE_CONTROL_JUMP_INT: {
		break;
	}
	case B_VM_OPCODE_CONTROL_RETURN: {
		break;
	}
	case B_VM_OPCODE_HALF: {
		break;
	}
	default: {
		return 1;
	}
	}
	return 0;
}
