#ifndef BREDER_ARRAYLIST_H
#define BREDER_ARRAYLIST_H

/**
 * Define the type of arraylist
 */
typedef struct b_arraylist_t b_arraylist_t;

/**
 * Define the type of error
 */
typedef unsigned char b_arraylist_error_t;

/**
 * Create a new arraylist with default configuration
 */
struct b_arraylist_t* b_arraylist_new_default();

/**
 * Create a new arraylist with custom configuration
 * The inicial size of array is define by parameter 'size'
 * The free function (can be null) is used for remove element or clear the arraylist.
 */
struct b_arraylist_t* b_arraylist_new_custom(int size, void (*freeFunc)(void*));

/**
 * Free the arraylist struct
 */
void b_arraylist_free(struct b_arraylist_t* self) ;

/**
 * Add a new element
 */
b_arraylist_error_t b_arraylist_add(struct b_arraylist_t* data, void* element);

/**
 * Return a element of arraylist
 */
void* b_arraylist_get(struct b_arraylist_t* data, int index);

/**
 * Remove a element of arraylist.
 * After remove, put the last element in the index
 * This method remove the order of elements
 */
void b_arraylist_remove_and_last(struct b_arraylist_t* data, int index);

/**
 * Remove a element of arraylist
 * After remove, move the nexts element to left
 * This method maintain the order but need to move the memory
 */
void b_arraylist_remove_and_shift(struct b_arraylist_t* data, int index);

/**
 * Change the element of arraylist
 */
void b_arraylist_set(struct b_arraylist_t* data, int index, void* element);

/**
 * Return the size of arraylist
 */
int b_arraylist_size(struct b_arraylist_t* data);

/**
 * Remove and Return a element of arraylist
 */
void b_arraylist_clear(struct b_arraylist_t* data);

/**
 * Return a iterator of arraylist. The size of iterator defined by 'b_arraylist_size'
 * The return can not free of memomry
 */
void** b_arraylist_iterator(struct b_arraylist_t* data);

#endif
#ifndef BREDER_ARRAYQUEUE_H
#define BREDER_ARRAYQUEUE_H

/**
 * Define the type of arrayqueue
 */
typedef struct b_arrayqueue_t b_arrayqueue_t;

/**
 * Define the type of error
 */
typedef unsigned char b_arrayqueue_error_t;

/**
 * Create a new arrayqueue with default configuration
 */
struct b_arrayqueue_t* b_arrayqueue_new_default();

/**
 * Create a new arrayqueue with custom configuration
 * The inicial size of array is define by parameter 'size'
 * The free function (can be null) is used for remove element or clear the arrayqueue.
 */
struct b_arrayqueue_t* b_arrayqueue_new_custom(int size);

/**
 * Free the memory
 */
void b_arrayqueue_free(struct b_arrayqueue_t* self);

/**
 * Add a new element in the end of queue
 */
b_arrayqueue_error_t b_arrayqueue_add(struct b_arrayqueue_t* data, void* element);

/**
 * Return the first element of arrayqueue
 */
void* b_arrayqueue_get(struct b_arrayqueue_t* data);

/**
 * Remove the first element of arrayqueue.
 * After remove, put the last element in the index
 * This method remove the order of elements
 */
void* b_arrayqueue_remove(struct b_arrayqueue_t* data);

/**
 * Return the size of arrayqueue
 */
int b_arrayqueue_size(struct b_arrayqueue_t* data);

/**
 * Remove and Return a element of arrayqueue
 */
void b_arrayqueue_clear(struct b_arrayqueue_t* data);

#endif
#ifndef BREDER_ASSERT_H
#define BREDER_ASSERT_H

#include <assert.h>

/**
 * Assert the true value. If the value is not true, the process will finish.
 * This implementation will be ignore by pre-processor if the environment is Release (not Debug).
 */
#define b_assert(TRUE) assert(TRUE)

#endif
#ifndef BREDER_BASE64_H
#define BREDER_BASE64_H

/**
 * Coding the bytes of 'from' parameter to 'to' parameter.
 * The parameter 'to' need already allocated with enough space.
 * The parameter 'len' indicate how many bytes will convert.
 * The method return the number of characters write
 */
long b_base64_encode(char* to, unsigned char* from, unsigned int len);

/**
 * Convert the base64 from parameter 'to' to bytes in the parameter 'from'.
 * The parameter 'len' indicate how many characters will convert.
 * The method return the number of bytes writed or negative value for error.
 */
long b_base64_decode(unsigned char* to, char* from, unsigned int len);

/**
 * Teste de base64 library
 */
void b_base64_test();

#endif
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
#ifndef BREDER_HASHMAP_H
#define BREDER_HASHMAP_H

/**
 * Type of hashmap
 */
typedef struct b_hashmap_t b_hashmap_t;

/**
 * Type of error
 */
typedef unsigned char b_hashmap_error_t;

/**
 * Create a map with default properties
 *
 * Parameters:
 * - hashFunc function that return the hash of key
 * - equalFunc function that return if the keys is equals
 * - freeKeyFunc function (optional) that delete the key
 * - freeValueFunc function (optional) that delete the value
 */
struct b_hashmap_t* b_hashmap_new_default(int (*hashFunc)(void*),
		int (*equalFunc)(void*, void*), void (*freeKeyFunc)(void*),
		void (*freeValueFunc)(void*));

/**
 * Create a map with default properties
 * The key is a int
 *
 * Parameters:
 * - freeValueFunc função opcional que libera da memoria o valor
 */
struct b_hashmap_t* b_hashmap_new_default_int(void (*freeValueFunc)(void*));

/**
 * Create a map with default properties
 * The key is a char*.
 *
 * Parameters:
 * - freeValueFunc função opcional que libera da memoria o valor
 */
struct b_hashmap_t* b_hashmap_new_default_vchar(void (*freeValueFunc)(void*));

/**
 * Create a map with custom properties
 *
 * Parameters:
 * - initialCapactity valor da capacidade inicial
 * - loadFactor fator de carga de novos espaços internamente
 * - hashFunc function that return the hash of key
 * - equalFunc function that return if the keys is equals
 * - freeKeyFunc function (optional) that delete the key
 * - freeValueFunc function (optional) that delete the value
 */
struct b_hashmap_t* b_hashmap_new_custom(int initialCapacity, float loadFactor,
		int (*hashFunc)(void*), int (*equalFunc)(void*, void*),
		void (*freeKeyFunc)(void*), void (*freeValueFunc)(void*));

/**
 * Free the hashmap struct
 */
void b_hashmap_free(struct b_hashmap_t* self);

/**
 * Return a value with a key. The key can not be null.
 * Return null if the map not contain the key.
 *
 * Parameters:
 * - self hashmap
 * - key key to find
 */
void* b_hashmap_get(struct b_hashmap_t* self, void* key);

/**
 * Return a value with a key. The key can not be null.
 * Return null if the map not contain the key.
 *
 * Parameters:
 * - self hashmap
 * - key key to find
 */
void* b_hashmap_get_int(struct b_hashmap_t* self, int key);

/**
 * Add or replace a new element in the hashmap. The key can not be null.
 * Return the old value if the the key already exist and replace the value.
 * Return null if same memory error happened.
 *
 * Parameters:
 * - self hashmap
 * - key key to find and put
 * - value value to put
 * - oldValue pointer (optional) to register the old value
 */
b_hashmap_error_t b_hashmap_put(struct b_hashmap_t* self, void* key,
		void* value, void** oldValue);

/**
 * Add or replace a new element in the hashmap. The key can not be null.
 * Return the old value if the the key already exist and replace the value.
 * Return null if same memory error happened.
 *
 * Parameters:
 * - self hashmap
 * - key key to find and put
 * - value value to put
 * - oldValue pointer (optional) to register the old value
 */
b_hashmap_error_t b_hashmap_put_int(struct b_hashmap_t* self, int key,
		void* value, void** oldValue);

/**
 * Remove a element from the hashmap.
 * If a free function registered, the key and the value will free.
 *
 * Parameters:
 * - self hashmap
 * - key key to find and remove
 */
void* b_hashmap_remove(struct b_hashmap_t* self, void* key);

/**
 * Remove a element from the hashmap.
 * If a free function registered, the key and the value will free.
 *
 * Parameters:
 * - self hashmap
 * - key key to find and remove
 */
void* b_hashmap_remove_int(struct b_hashmap_t* self, int key);

/**
 * Return the size of hashmap
 *
 * Parameters:
 * - self hashmap
 */
int b_hashmap_size(struct b_hashmap_t* self);

/**
 * Clean the hashmap
 * If a free function registered, the all keys and all values will free.
 *
 * Parameters:
 * - self hashmap
 */
void b_hashmap_clear(struct b_hashmap_t* self);

/**
 * Test the hashmap library
 */
void b_hashmap_test();

#endif
#ifndef BREDER_IOSTREAM_H
#define BREDER_IOSTREAM_H

unsigned char b_vm_read_byte(unsigned char** bytes);

/**
 * Read a index in the bytes.
 * Return number of bytes readed or zero for same error.
 */
int b_vm_read_index(unsigned char** bytes, unsigned int* result);

/**
 * Read a string in the bytes
 * Return number of bytes readed or zero for same error.
 */
int b_vm_read_utf(unsigned char** bytes, wchar_t** result, int* length);

/**
 * Read a integer with 32 bits between values -2147483647 and +2147483647.<br/>
 * The read use this mask:<br/>
 * 1. 0+xxxxxx <br/>
 * 2. 10+xxxxx xxxxxxxx <br/>
 * 3. 110+xxxx xxxxxxxx xxxxxxxx <br/>
 * 4. 1110+xxx xxxxxxxx xxxxxxxx xxxxxxxx <br/>
 * 5. 11110000 +xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx <br/>
 * Which the plus bit mean a bit for positivo for 0 and negative for 1.<br/>
 * For read: <br/>
 * If first byte >> 7 == 0, than the item is 1. <br/>
 * If first byte >> 6 == 2, than the item is 2. <br/>
 * If first byte >> 5 == 6, than the item is 3. <br/>
 * If first byte >> 4 == 14, than the item is 4. <br/>
 * If first byte >> 3 == 30, than the item is 5. <br/>
 * Para escrita: <br/>
 * If the value is <= +-63, than the item is 1. <br/>
 * If the value is <= +-8191, than the item is 2. <br/>
 * If the value is <= +-1048575, than the item is 3. <br/>
 * If the value is <= +-134217727, than the item is 4. <br/>
 * If the value is <= +-2147483647, than the item is 5. <br/>
 */
int b_vm_read_int(unsigned char** bytes, int* result);

#endif
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
#ifndef BREDER_MEMORYSTREAM_H
#define BREDER_MEMORYSTREAM_H

/**
 * Type of Memory Input Stream
 */
typedef struct b_memoryinputstream_t b_memoryinputstream_t;

/**
 * Type of Memory Output Stream
 */
typedef struct b_memoryoutputstream_t b_memoryoutputstream_t;

/**
 * Type of Error
 */
typedef unsigned char b_memorystream_error_t;

/**
 * Create a Memory Input Stream with a path of memorysystem.
 * Return null if the memory not exist.
 * Return null if same error happended.
 */
struct b_memoryinputstream_t* b_memoryinputstream_new(unsigned char* bytes, int size);

/**
 * Return the next byte between 0-255.
 * Return -1 if the end of memory.
 */
int b_memoryinputstream_read(struct b_memoryinputstream_t* self);

/**
 * End of file of memory
 */
unsigned char b_memoryinputstream_is_eof(struct b_memoryinputstream_t* self) ;

/**
 * Close the memory
 */
void b_memoryinputstream_close(struct b_memoryinputstream_t* self);

/**
 * Create a Memory Output Stream with a path of memorysystem.
 * If the memory not exist, it will create a new.
 * Return null if same error happended.
 */
struct b_memoryoutputstream_t* b_memoryoutputstream_new(const char* name);

/**
 * Write a byte.
 * Return a number different of zero if same error happended.
 */
b_memorystream_error_t b_memoryoutputstream_write(struct b_memoryoutputstream_t* self, unsigned char byte);

/**
 * Close the memory
 */
void b_memoryoutputstream_close(struct b_memoryoutputstream_t* self);

#endif
#ifndef BREDER_SDL_H
#define BREDER_SDL_H

/**
 * Type of error
 */
typedef int b_sdl_error_t;

typedef struct b_sdl_surface_t b_sdl_surface_t;

/**
 * Init the SDL. If same error happened, this will return a number different of zero.
 */
b_sdl_error_t b_sdl_init(int width, int height);

/**
 * Register a video callback
 */
void b_sdl_init_video_func(void (*paint)());

/**
 * Register a mouse callback
 */
void b_sdl_init_mouse_func(void (*)());

/**
 * Register a keyboard callback
 */
void b_sdl_init_keyboard_func(void (*key_pressed)(), void (*key_released)());

/**
 * Return the state of all keys of keyboard.
 * This method return a pointer which you can not free.
 */
unsigned char* b_sdl_keyboard_state();

/**
 * Return if the shift is pressed
 */
unsigned char b_sdl_keyboard_is_shift_pressed();

/**
 * Return if the ctrl is pressed
 */
unsigned char b_sdl_keyboard_is_ctrl_pressed();

/**
 * Return if the alt is pressed
 */
unsigned char b_sdl_keyboard_is_alt_pressed();

/**
 * Return if the meta is pressed
 */
unsigned char b_sdl_keyboard_is_meta_pressed();

/**
 * Draw a circle. The point x and y is the middle of the circle
 */
void b_sdl_graphic_draw_circle(int x, int y, int radium);

/**
 * Draw the rectangle with no fill. The point x and y is the top and left of the retangle.
 */
void b_sdl_graphic_draw_rect(int x, int y, int width, int height);

/**
 * Set the current color
 */
void b_sdl_graphic_color(int r, int g, int b);

/**
 * Return the last error
 */
const char* b_sdl_error();

/**
 * Start the main loop
 */
b_sdl_error_t b_sdl_loop();

#define B_SDL_KEY_BACKSPACE 8
#define B_SDL_KEY_TAB 9
#define B_SDL_KEY_CLEAR 12
#define B_SDL_KEY_RETURN 13
#define B_SDL_KEY_PAUSE 19
#define B_SDL_KEY_ESCAPE 27
#define B_SDL_KEY_SPACE 32
#define B_SDL_KEY_EXCLAIM 33
#define B_SDL_KEY_QUOTEDBL 34
#define B_SDL_KEY_HASH 35
#define B_SDL_KEY_DOLLAR 36
#define B_SDL_KEY_AMPERSAND 38
#define B_SDL_KEY_QUOTE 39
#define B_SDL_KEY_LEFTPAREN 40
#define B_SDL_KEY_RIGHTPAREN 41
#define B_SDL_KEY_ASTERISK 42
#define B_SDL_KEY_PLUS 43
#define B_SDL_KEY_COMMA 44
#define B_SDL_KEY_MINUS 45
#define B_SDL_KEY_PERIOD 46
#define B_SDL_KEY_SLASH 47
#define B_SDL_KEY_0 48
#define B_SDL_KEY_1 49
#define B_SDL_KEY_2 50
#define B_SDL_KEY_3 51
#define B_SDL_KEY_4 52
#define B_SDL_KEY_5 53
#define B_SDL_KEY_6 54
#define B_SDL_KEY_7 55
#define B_SDL_KEY_8 56
#define B_SDL_KEY_9 57
#define B_SDL_KEY_COLON 58
#define B_SDL_KEY_SEMICOLON 59
#define B_SDL_KEY_LESS 60
#define B_SDL_KEY_EQUALS 61
#define B_SDL_KEY_GREATER 62
#define B_SDL_KEY_QUESTION 63
#define B_SDL_KEY_AT 64
#define B_SDL_KEY_LEFTBRACKET 91
#define B_SDL_KEY_BACKSLASH 92
#define B_SDL_KEY_RIGHTBRACKET 93
#define B_SDL_KEY_CARET 94
#define B_SDL_KEY_UNDERSCORE 95
#define B_SDL_KEY_BACKQUOTE 96
#define B_SDL_KEY_A 97
#define B_SDL_KEY_B 98
#define B_SDL_KEY_C 99
#define B_SDL_KEY_D 100
#define B_SDL_KEY_E 101
#define B_SDL_KEY_F 102
#define B_SDL_KEY_G 103
#define B_SDL_KEY_H 104
#define B_SDL_KEY_I 105
#define B_SDL_KEY_J 106
#define B_SDL_KEY_K 107
#define B_SDL_KEY_L 108
#define B_SDL_KEY_M 109
#define B_SDL_KEY_N 110
#define B_SDL_KEY_O 111
#define B_SDL_KEY_P 112
#define B_SDL_KEY_Q 113
#define B_SDL_KEY_R 114
#define B_SDL_KEY_S 115
#define B_SDL_KEY_T 116
#define B_SDL_KEY_U 117
#define B_SDL_KEY_V 118
#define B_SDL_KEY_W 119
#define B_SDL_KEY_X 120
#define B_SDL_KEY_Y 121
#define B_SDL_KEY_Z 122
#define B_SDL_KEY_DELETE 127
#define B_SDL_KEY_KP0 256
#define B_SDL_KEY_KP1 257
#define B_SDL_KEY_KP2 258
#define B_SDL_KEY_KP3 259
#define B_SDL_KEY_KP4 260
#define B_SDL_KEY_KP5 261
#define B_SDL_KEY_KP6 262
#define B_SDL_KEY_KP7 263
#define B_SDL_KEY_KP8 264
#define B_SDL_KEY_KP9 265
#define B_SDL_KEY_KP_PERIOD 266
#define B_SDL_KEY_KP_DIVIDE 267
#define B_SDL_KEY_KP_MULTIPLY 268
#define B_SDL_KEY_KP_MINUS 269
#define B_SDL_KEY_KP_PLUS 270
#define B_SDL_KEY_KP_ENTER 271
#define B_SDL_KEY_KP_EQUALS 272
#define B_SDL_KEY_UP 273
#define B_SDL_KEY_DOWN 274
#define B_SDL_KEY_RIGHT 275
#define B_SDL_KEY_LEFT 276
#define B_SDL_KEY_INSERT 277
#define B_SDL_KEY_HOME 278
#define B_SDL_KEY_END 279
#define B_SDL_KEY_PAGEUP 280
#define B_SDL_KEY_PAGEDOWN 281
#define B_SDL_KEY_F1 282
#define B_SDL_KEY_F2 283
#define B_SDL_KEY_F3 284
#define B_SDL_KEY_F4 285
#define B_SDL_KEY_F5 286
#define B_SDL_KEY_F6 287
#define B_SDL_KEY_F7 288
#define B_SDL_KEY_F8 289
#define B_SDL_KEY_F9 290
#define B_SDL_KEY_F10 291
#define B_SDL_KEY_F11 292
#define B_SDL_KEY_F12 293
#define B_SDL_KEY_F13 294
#define B_SDL_KEY_F14 295
#define B_SDL_KEY_F15 296
#define B_SDL_KEY_NUMLOCK 300
#define B_SDL_KEY_CAPSLOCK 301
#define B_SDL_KEY_SCROLLOCK 302
#define B_SDL_KEY_RSHIFT 303
#define B_SDL_KEY_LSHIFT 304
#define B_SDL_KEY_RCTRL 305
#define B_SDL_KEY_LCTRL 306
#define B_SDL_KEY_RALT 307
#define B_SDL_KEY_LALT 308
#define B_SDL_KEY_RMETA 309
#define B_SDL_KEY_LMETA 310
#define B_SDL_KEY_LSUPER 311
#define B_SDL_KEY_RSUPER 312
#define B_SDL_KEY_MODE 313
#define B_SDL_KEY_COMPOSE 314
#define B_SDL_KEY_HELP 315
#define B_SDL_KEY_PRINT 316
#define B_SDL_KEY_SYSREQ 317
#define B_SDL_KEY_BREAK 318
#define B_SDL_KEY_MENU 319
#define B_SDL_KEY_POWER 320
#define B_SDL_KEY_EURO 321
#define B_SDL_KEY_UNDO 322

#endif
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
#ifndef BREDER_TREEMAP_H
#define BREDER_TREEMAP_H

/**
 * Define the type of treemap
 */
typedef struct b_treemap_t b_treemap_t;

/**
 * Define the type of error
 */
typedef unsigned char b_treemap_error_t;

/**
 * Create a new treemap
 */
struct b_treemap_t* b_treemap_new(int (*compareFunc)(void*, void*),
		void (*freeKeyFunc)(void*), void (*freeValueFunc)(void*));

/**
 * Free the treemap struct
 */
void b_treemap_free(struct b_treemap_t* data) ;

/**
 * Create a new treemap with int key
 */
struct b_treemap_t* b_treemap_new_int(void (*freeValueFunc)(void*));

/**
 * Create a new treemap with char* key
 */
struct b_treemap_t* b_treemap_new_vchar(unsigned char freeKey,
		void (*freeValueFunc)(void*));

/**
 * Add a new element.
 */
b_treemap_error_t b_treemap_put(struct b_treemap_t* data, void* key,
		void* value);

/**
 * Add a new element.
 * The key is a int.
 */
b_treemap_error_t b_treemap_put_int(struct b_treemap_t* data, int key,
		void* value);

/**
 * Return a element of treemap
 */
void* b_treemap_get(struct b_treemap_t* data, void* key);

/**
 * Return a element of treemap
 */
void* b_treemap_get_int(struct b_treemap_t* data, int key);

/**
 * Return the size of treemap
 */
int b_treemap_size(struct b_treemap_t* data);

/**
 * Remove a element of treemap.
 */
void b_treemap_remove(struct b_treemap_t* data, void* key);

/**
 * Remove a element of treemap.
 */
void b_treemap_remove_int(struct b_treemap_t* data, int key);

/**
 * Remove and Return a element of treemap
 */
void b_treemap_clear(struct b_treemap_t* data);

#endif
#ifndef BREDER_VM_H
#define BREDER_VM_H

/**
 * Especifica um tipo para a maquina virtual
 */
typedef struct b_vm_t b_vm_t;

#define B_VM_OPCODE_STACK_INC 1
#define B_VM_OPCODE_STACK_DEC 2
#define B_VM_OPCODE_STACK_STRING 3
#define B_VM_OPCODE_STACK_DOUBLE 4
#define B_VM_OPCODE_STACK_INTEGER 5
#define B_VM_OPCODE_STACK_TRUE 6
#define B_VM_OPCODE_STACK_FALSE 7
#define B_VM_OPCODE_STACK_NULL 8
#define B_VM_OPCODE_STACK_TERNARY 9

#define B_VM_OPCODE_NUMBER_SUM 20
#define B_VM_OPCODE_NUMBER_SUB 21
#define B_VM_OPCODE_NUMBER_MUL 22
#define B_VM_OPCODE_NUMBER_DIV 23
#define B_VM_OPCODE_NUMBER_EQ 24
#define B_VM_OPCODE_NUMBER_NEQ 25
#define B_VM_OPCODE_NUMBER_COMPARE 26
#define B_VM_OPCODE_NUMBER_GT 27
#define B_VM_OPCODE_NUMBER_GE 28
#define B_VM_OPCODE_NUMBER_LT 29
#define B_VM_OPCODE_NUMBER_LE 30
#define B_VM_OPCODE_NUMBER_MOD 31
#define B_VM_OPCODE_NUMBER_INT_DIV 32
#define B_VM_OPCODE_NUMBER_TO_STRING 33
#define B_VM_OPCODE_NUMBER_IS_NAN 34
#define B_VM_OPCODE_NUMBER_IS_INFINITY 35
#define B_VM_OPCODE_NUMBER_HASH 36
#define B_VM_OPCODE_NUMBER_NEG 37
#define B_VM_OPCODE_NUMBER_INC 38
#define B_VM_OPCODE_NUMBER_DEC 39

#define B_VM_OPCODE_INTEGER_SUM 60
#define B_VM_OPCODE_INTEGER_SUB 61
#define B_VM_OPCODE_INTEGER_MUL 62
#define B_VM_OPCODE_INTEGER_DIV 63
#define B_VM_OPCODE_INTEGER_EQ 64
#define B_VM_OPCODE_INTEGER_NEQ 65
#define B_VM_OPCODE_INTEGER_COMPARE 66
#define B_VM_OPCODE_INTEGER_GT 67
#define B_VM_OPCODE_INTEGER_GE 68
#define B_VM_OPCODE_INTEGER_LT 69
#define B_VM_OPCODE_INTEGER_LE 70
#define B_VM_OPCODE_INTEGER_AND 71
#define B_VM_OPCODE_INTEGER_OR 72
#define B_VM_OPCODE_INTEGER_MOD 73
#define B_VM_OPCODE_INTEGER_TO_STRING 74
#define B_VM_OPCODE_INTEGER_HASH 75
#define B_VM_OPCODE_INTEGER_NEG 76
#define B_VM_OPCODE_INTEGER_INC 77
#define B_VM_OPCODE_INTEGER_DEC 78

#define B_VM_OPCODE_BOOLEAN_NOT 90
#define B_VM_OPCODE_BOOLEAN_AND 91
#define B_VM_OPCODE_BOOLEAN_OR 92

#define B_VM_OPCODE_STRING_SUM 100

#define B_VM_OPCODE_CONTROL_JUMP 150
#define B_VM_OPCODE_CONTROL_JUMP_TRUE 151
#define B_VM_OPCODE_CONTROL_JUMP_FALSE 152
#define B_VM_OPCODE_CONTROL_JUMP_INT 153
#define B_VM_OPCODE_CONTROL_RETURN 154
#define B_VM_OPCODE_HALF 255

/**
 * Constroi uma máquina virtual com as configurações padrões
 */
struct b_vm_t* b_vm_new_default();

/**
 * Executa um conjunto de opcodes e retorna um objeto ou nulo caso não tenha que retornar
 */
unsigned int b_vm_execute(unsigned char* opcodes, int length, void* result,
		void* exception);

#endif
