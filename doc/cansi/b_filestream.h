#ifndef BREDER_FILESTREAM_H
#define BREDER_FILESTREAM_H

/**
 * Type of File Input Stream
 */
typedef struct b_fileinputstream_t b_fileinputstream_t;

/**
 * Type of File Output Stream
 */
typedef struct b_fileoutputstream_t b_fileoutputstream_t;

/**
 * Type of Error
 */
typedef unsigned char b_filestream_error_t;

/**
 * Create a File Input Stream with a path of filesystem.
 * Return null if the file not exist.
 * Return null if same error happended.
 */
struct b_fileinputstream_t* b_fileinputstream_new(const char* path);

/**
 * Return the next byte between 0-255.
 * Return -1 if the end of file.
 */
int b_fileinputstream_read(struct b_fileinputstream_t* self);

/**
 * Return if is end of file
 */
unsigned char b_fileinputstream_is_eof(struct b_fileinputstream_t* self);

/**
 * Close the file
 */
void b_fileinputstream_close(struct b_fileinputstream_t* self);

/**
 * Create a File Output Stream with a path of filesystem.
 * If the file not exist, it will create a new.
 * Return null if same error happended.
 */
struct b_fileoutputstream_t* b_fileoutputstream_new_create(const char* path);

/**
 * Append in the File Output Stream with a path of filesystem.
 * If the file not exist, it will create a new.
 * Return null if same error happended.
 */
struct b_fileoutputstream_t* b_fileoutputstream_new_append(const char* path) ;

/**
 * Write a byte.
 * Return a number different of zero if same error happended.
 */
b_filestream_error_t b_fileoutputstream_write(struct b_fileoutputstream_t* self,
		unsigned char byte);

/**
 * Flush the bytes
 */
void b_fileoutputstream_flush(struct b_fileoutputstream_t* self);

/**
 * Close the file
 */
void b_fileoutputstream_close(struct b_fileoutputstream_t* self);

#endif
