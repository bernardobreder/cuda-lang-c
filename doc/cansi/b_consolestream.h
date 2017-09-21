#ifndef BREDER_CONSOLESTREAM_H
#define BREDER_CONSOLESTREAM_H

/**
 * Type of Console Input Stream
 */
typedef struct b_consoleinputstream_t b_consoleinputstream_t;

/**
 * Type of Console Output Stream
 */
typedef struct b_consoleoutputstream_t b_consoleoutputstream_t;

/**
 * Type of Error
 */
typedef unsigned char b_consolestream_error_t;

/**
 * Create a Console Input Stream with a path of consolesystem.
 * Return null if the console not exist.
 * Return null if same error happended.
 */
struct b_consoleinputstream_t* b_consoleinputstream_new();

/**
 * Return the next byte between 0-255.
 * Return -1 if the end of console.
 */
int b_consoleinputstream_read(struct b_consoleinputstream_t* self);

/**
 * Return false (zero) always
 */
unsigned char b_consoleinputstream_is_eof(struct b_consoleinputstream_t* self);

/**
 * Close the console
 */
void b_consoleinputstream_close(struct b_consoleinputstream_t* self);

/**
 * Create a Console Output Stream with a path of consolesystem.
 * If the console not exist, it will create a new.
 * Return null if same error happended.
 */
struct b_consoleoutputstream_t* b_consoleoutputstream_new();

/**
 * Write a byte.
 * Return a number different of zero if same error happended.
 */
b_consolestream_error_t b_consoleoutputstream_write(
		struct b_consoleoutputstream_t* self, unsigned char byte);

/**
 * Close the console
 */
void b_consoleoutputstream_close(struct b_consoleoutputstream_t* self);

#endif
