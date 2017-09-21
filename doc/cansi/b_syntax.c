#include <stdlib.h>
#include "b_syntax.h"

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
