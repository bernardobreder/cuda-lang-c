#ifndef BREDER_SYNTAX_H
#define BREDER_SYNTAX_H

/**
 * Type of syntax
 */
typedef struct b_syntax_t b_syntax_t;

/**
 * Create a new syntax struct with extern function.
 * 'data' argument is the lexical struct. This argument will be pass to the 'read_func' argument.
 * 'read_func' argument is a function that read a token.
 * 'token_free_func' argument is a free function for token struct
 * 'token_id_func' argument return the id of a token
 * 'token_pchar_func' argument return the char* of a token
 * 'token_utf_func' argument return the wchar_t* of a token
 * 'token_double_func' argument return the double of a token
 * If this function return null, same error happended.
 */
struct b_syntax_t* b_syntax_new(void* data, void* (*read_func)(void* data), void (*token_free_func)(void* token), int (*token_id_func)(void* token), const char* (*token_pchar_func)(void*),
		const wchar_t*(*token_utf_func)(void* token), double (*token_double_func)(void* token));

/**
 * Free the syntax struct.
 * This method not free the 'data' of lexical.
 */
void b_syntax_free(struct b_syntax_t* self);

/**
 * Test the library
 */
void b_lexical_test();

#endif
