#include <stdlib.h>
#include <stdio.h>
#include "b_filestream.h"

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
