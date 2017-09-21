#include <stdlib.h>
#include <stdio.h>
#include "b_consolestream.h"

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
