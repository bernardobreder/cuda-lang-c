#include "b_sdl.h"
#include <sdl/SDL.h>
#include <sdl/SDL_ttf.h>
#include <math.h>

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
