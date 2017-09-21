#ifndef BREDER_LEXICAL_H
#define BREDER_LEXICAL_H

#include <wchar.h>

/**
 * Type of lexical
 */
typedef struct b_lexical_t b_lexical_t;

/**
 * Type of token
 */
typedef struct b_token_t b_token_t;

/**
 * Build a new lexical.
 * Return null if same error happended.
 */
struct b_lexical_t* b_lexical_new(void* data, int (*readFunc)(void*), void (*closeFunc)(void*));

/**
 * Free the memory of lexical
 */
void b_lexical_free(struct b_lexical_t* self);

/**
 * Free the memory of token
 */
void b_token_free(struct b_token_t* self);

/**
 * Read a token in the lexical.
 * Return null if same error happended.
 * The result of this method need to be free for the user of this method.
 */
struct b_token_t* b_lexical_read(struct b_lexical_t* self);

/**
 * Return the id of the token
 */
int b_lexical_token_id(struct b_token_t* self);

/**
 * Return the image of the token.
 * Can not free the result of this method.
 */
const char* b_lexical_token_pchar(struct b_token_t* self);

/**
 * Return the image of the token in utf-8.
 * You have to compare the result with the function of 'wchar.h' like 'wcscmp'.
 * Can not free the result of this method.
 */
const wchar_t* b_lexical_token_utf(struct b_token_t* self);

/**
 * Return the value of the number token
 */
double b_lexical_token_double(struct b_token_t* self);

/**
 * Testador
 */
void b_lexical_test();

#define B_LEXICAL_AND_ID 256
#define B_LEXICAL_CONTINUE_ID 257
#define B_LEXICAL_BREAK_ID 258
#define B_LEXICAL_DO_ID 259
#define B_LEXICAL_ELSE_ID 260
#define B_LEXICAL_EQ_ID 261
#define B_LEXICAL_FALSE_ID 262
#define B_LEXICAL_GE_ID 263
#define B_LEXICAL_ID_ID 264
#define B_LEXICAL_IF_ID 265
#define B_LEXICAL_LE_ID 267
#define B_LEXICAL_NE_ID 269
#define B_LEXICAL_NUM_ID 270
#define B_LEXICAL_OR_ID 271
#define B_LEXICAL_TRUE_ID 274
#define B_LEXICAL_WHILE_ID 275
#define B_LEXICAL_END_ID 276
#define B_LEXICAL_REPEAT_ID 277
#define B_LEXICAL_FOR_ID 278
#define B_LEXICAL_STR_ID 290
#define B_LEXICAL_THIS_ID 291
#define B_LEXICAL_DEC_ID 292
#define B_LEXICAL_INC_ID 293
#define B_LEXICAL_RETURN_ID 294

#endif
