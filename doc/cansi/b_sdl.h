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
