#include <stdlib.h>
#include <string.h>
#include "b_lexical.h"

struct b_lexical_t {
	int (*readFunc)(void*);
	void (*closeFunc)(void*);
	void* data;
	int look;
};

struct b_token_t {
	int tag;
};

struct b_token_pchar_t {
	int tag;
	char* value;
};

struct b_token_double_t {
	int tag;
	double value;
};

struct b_token_pwchar_t {
	int tag;
	wchar_t* value;
};

static struct b_token_t *AND_TOKEN = 0, *BREAK_TOKEN = 0, *CONTINUE_TOKEN = 0, *DO_TOKEN = 0, *ELSE_TOKEN = 0, *END_TOKEN = 0, *FALSE_TOKEN = 0, *FOR_TOKEN = 0, *IF_TOKEN = 0, *OR_TOKEN = 0,
		*REPEAT_TOKEN = 0, *THIS_TOKEN = 0, *TRUE_TOKEN = 0, *WHILE_TOKEN = 0, *RETURN_TOKEN = 0;

static struct b_token_t *DEC_TOKEN = 0, *INC_TOKEN = 0, *GE_TOKEN = 0, *LE_TOKEN = 0, *NE_TOKEN = 0, *EQ_TOKEN = 0, *NOT_TOKEN = 0, *ASSIGN_TOKEN = 0, *GT_TOKEN = 0, *LT_TOKEN = 0, *SUM_TOKEN = 0,
		*SUB_TOKEN = 0;

struct b_lexical_t* b_lexical_new(void* data, int (*readFunc)(void*), void (*closeFunc)(void*)) {
	struct b_lexical_t* self = (struct b_lexical_t*) calloc(1, sizeof(struct b_lexical_t));
	if (!self) {
		return 0;
	}
	self->readFunc = readFunc;
	self->closeFunc = closeFunc;
	self->data = data;
	self->look = -2;
	return self;
}

void b_lexical_free(struct b_lexical_t* self) {
	if (self->closeFunc) {
		self->closeFunc(self->data);
	}
	free(self);
}

void b_token_free(struct b_token_t* self) {
	if (self == AND_TOKEN || self == BREAK_TOKEN || self == CONTINUE_TOKEN || self == DO_TOKEN || self == ELSE_TOKEN || self == END_TOKEN || self == FALSE_TOKEN || self == FOR_TOKEN
			|| self == IF_TOKEN || self == OR_TOKEN || self == REPEAT_TOKEN || self == THIS_TOKEN || self == TRUE_TOKEN || self == WHILE_TOKEN || self == RETURN_TOKEN || self == DEC_TOKEN
			|| self == INC_TOKEN || self == GE_TOKEN || self == LE_TOKEN || self == NE_TOKEN || self == EQ_TOKEN || self == NOT_TOKEN || self == ASSIGN_TOKEN || self == GT_TOKEN || self == LT_TOKEN
			|| self == SUM_TOKEN || self == SUB_TOKEN) {
	} else {
		if (self->tag == B_LEXICAL_STR_ID) {
			free(((struct b_token_pwchar_t*) self)->value);
		} else if (self->tag == B_LEXICAL_NUM_ID) {
		} else {
			free(((struct b_token_pchar_t*) self)->value);
		}
		free(self);
	}
}

static unsigned char b_lexical_is_word_start(int c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$';
}

static unsigned char b_lexical_is_word_part(int c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$';
}

static unsigned char b_lexical_is_string_start(int c) {
	return c == '\"';
}

static unsigned char b_lexical_is_number_start(int c) {
	return c >= '0' && c <= '9';
}

static int b_lexical_read_char(struct b_lexical_t* self) {
	int c = self->readFunc(self->data);
	if (c <= 0x7F) {
		return c;
	} else if ((c >> 5) == 0x6) {
		int i2 = self->readFunc(self->data);
		if (i2 < 0) {
			return -1;
		}
		return (((c & 0x1F) << 6) + (i2 & 0x3F));
	} else {
		int i2 = self->readFunc(self->data);
		if (i2 < 0) {
			return -1;
		}
		int i3 = self->readFunc(self->data);
		if (i3 < 0) {
			return -1;
		}
		return (((c & 0xF) << 12) + ((i2 & 0x3F) << 6) + (i3 & 0x3F));
	}
}

static int b_lexical_look(struct b_lexical_t* self) {
	if (self->look == -2) {
		self->look = b_lexical_read_char(self);
	}
	return self->look;
}

static int b_lexical_next(struct b_lexical_t* self) {
	return self->look = b_lexical_read_char(self);
}

static unsigned char b_lexical_is_word(struct b_lexical_t* self) {
	int c = b_lexical_look(self);
	while (c <= 32) {
		if (c < 0) {
			return -1;
		}
		c = b_lexical_next(self);
	}
	return b_lexical_is_word_start(c);
}

static unsigned char b_lexical_is_number(struct b_lexical_t* self) {
	int c = b_lexical_look(self);
	while (c <= 32) {
		if (c < 0) {
			return -1;
		}
		c = b_lexical_next(self);
	}
	return b_lexical_is_number_start(c);
}

static unsigned char b_lexical_is_string(struct b_lexical_t* self) {
	int c = b_lexical_look(self);
	while (c <= 32) {
		if (c < 0) {
			return -1;
		}
		c = b_lexical_next(self);
	}
	return b_lexical_is_string_start(c);
}

static struct b_token_t* b_lexical_token_pchar_new(int tag, char* text) {
	struct b_token_pchar_t* self = (struct b_token_pchar_t*) calloc(1, sizeof(struct b_token_pchar_t));
	if (!self) {
		return 0;
	}
	self->tag = tag;
	self->value = text;
	return (b_token_t*) self;
}

static struct b_token_t* b_lexical_token_pwchar_new(wchar_t* text) {
	struct b_token_pwchar_t* self = (struct b_token_pwchar_t*) calloc(1, sizeof(struct b_token_pwchar_t));
	if (!self) {
		return 0;
	}
	self->tag = B_LEXICAL_STR_ID;
	self->value = text;
	return (b_token_t*) self;
}

static struct b_token_t* b_lexical_token_double_new(double text) {
	struct b_token_double_t* self = (struct b_token_double_t*) calloc(1, sizeof(struct b_token_double_t));
	if (!self) {
		return 0;
	}
	self->tag = B_LEXICAL_NUM_ID;
	self->value = text;
	return (b_token_t*) self;
}

static struct b_token_t* b_lexical_read_token(int id, const char* text) {
	char* image = strdup(text);
	if (!image) {
		return 0;
	}
	struct b_token_t* data = b_lexical_token_pchar_new(id, image);
	if (!data) {
		free(image);
		return 0;
	}
	return data;
}

static struct b_token_t* b_lexical_read_word(struct b_lexical_t* self) {
	int c = b_lexical_look(self);
	while (c <= 32) {
		if (c < 0) {
			return 0;
		}
		c = b_lexical_next(self);
	}
	int length = 1;
	char* text = (char*) calloc(length + 1, sizeof(char));
	char* aux = text;
	if (!text) {
		return 0;
	}
	*aux++ = c;
	c = b_lexical_next(self);
	while (b_lexical_is_word_part(c)) {
		if (text + length == aux) {
			length *= 2;
			char* data = (char*) realloc(text, (length + 1) * sizeof(char));
			if (!data) {
				free(text);
				return 0;
			}
			text = data;
			aux = text + length / 2;
		}
		*aux++ = c;
		c = b_lexical_next(self);
	}
	*aux = 0;
	aux = text;
	switch (*aux++) {
	case 'a': {
		if (*aux++ == 'n' && *aux++ == 'd' && *aux++ == 0) {
			if (!AND_TOKEN) {
				AND_TOKEN = b_lexical_read_token(B_LEXICAL_AND_ID, "and");
			}
			free(text);
			return AND_TOKEN;
		}
		break;
	}
	case 'b': {
		if (*aux++ == 'r' && *aux++ == 'e' && *aux++ == 'a' && *aux++ == 'k' && *aux++ == 0) {
			if (!BREAK_TOKEN) {
				BREAK_TOKEN = b_lexical_read_token(B_LEXICAL_BREAK_ID, "break");
			}
			free(text);
			return BREAK_TOKEN;
		}
		break;
	}
	case 'c': {
		if (*aux++ == 'o' && *aux++ == 'n' && *aux++ == 't' && *aux++ == 'i' && *aux++ == 'n' && *aux++ == 'u' && *aux++ == 'e' && *aux++ == 0) {
			if (!CONTINUE_TOKEN) {
				CONTINUE_TOKEN = b_lexical_read_token(B_LEXICAL_CONTINUE_ID, "continue");
			}
			free(text);
			return CONTINUE_TOKEN;
		}
		break;
	}
	case 'd': {
		if (*aux++ == 'o' && *aux++ == 0) {
			if (!DO_TOKEN) {
				DO_TOKEN = b_lexical_read_token(B_LEXICAL_DO_ID, "do");
			}
			free(text);
			return DO_TOKEN;
		}
		break;
	}
	case 'e': {
		switch (*aux++) {
		case 'n': {
			if (*aux++ == 'd' && *aux++ == 0) {
				if (!END_TOKEN) {
					END_TOKEN = b_lexical_read_token(B_LEXICAL_END_ID, "end");
				}
				free(text);
				return END_TOKEN;
			}
			break;
		}
		}
		case 'l':
		{
			if (*aux++ == 's' && *aux++ == 'e' && *aux++ == 0) {
				if (!ELSE_TOKEN) {
					ELSE_TOKEN = b_lexical_read_token(B_LEXICAL_ELSE_ID, "else");
				}
				free(text);
				return ELSE_TOKEN;
			}
			break;
		}
		break;
	}
	case 'f': {
		switch (*aux++) {
		case 'a': {
			if (*aux++ == 'l' && *aux++ == 's' && *aux++ == 'e' && *aux++ == 0) {
				if (!FALSE_TOKEN) {
					FALSE_TOKEN = b_lexical_read_token(B_LEXICAL_FALSE_ID, "false");
				}
				free(text);
				return FALSE_TOKEN;
			}
			break;
		}
		case 'o': {
			if (*aux++ == 'r' && *aux++ == 0) {
				if (!FOR_TOKEN) {
					FOR_TOKEN = b_lexical_read_token(B_LEXICAL_FOR_ID, "for");
				}
				free(text);
				return FOR_TOKEN;
			}
			break;
		}
		}
		break;
	}
	case 'i': {
		if (*aux++ == 'f' && *aux++ == 0) {
			if (!IF_TOKEN) {
				IF_TOKEN = b_lexical_read_token(B_LEXICAL_IF_ID, "if");
			}
			free(text);
			return IF_TOKEN;
		}
		break;
	}
	case 'o': {
		if (*aux++ == 'r' && *aux++ == 0) {
			if (!OR_TOKEN) {
				OR_TOKEN = b_lexical_read_token(B_LEXICAL_OR_ID, "or");
			}
			free(text);
			return OR_TOKEN;
		}
		break;
	}
	case 'r': {
		if (*aux++ == 'e') {
			switch (*aux++) {
			case 'p': {
				if (*aux++ == 'e' && *aux++ == 'a' && *aux++ == 't' && *aux++ == 0) {
					if (!REPEAT_TOKEN) {
						REPEAT_TOKEN = b_lexical_read_token(B_LEXICAL_REPEAT_ID, "repeat");
					}
					free(text);
					return REPEAT_TOKEN;
				}
				break;
			}
			case 't': {
				if (*aux++ == 'u' && *aux++ == 'r' && *aux++ == 'n' && *aux++ == 0) {
					if (!RETURN_TOKEN) {
						RETURN_TOKEN = b_lexical_read_token(B_LEXICAL_RETURN_ID, "return");
					}
					free(text);
					return RETURN_TOKEN;
				}
				break;
			}
			}

		}
		break;
	}
	case 't': {
		switch (*aux++) {
		case 'h': {
			if (*aux++ == 'i' && *aux++ == 's' && *aux++ == 0) {
				if (!THIS_TOKEN) {
					THIS_TOKEN = b_lexical_read_token(B_LEXICAL_THIS_ID, "this");
				}
				free(text);
				return THIS_TOKEN;
			}
			break;
		}
		case 'r': {
			if (*aux++ == 'u' && *aux++ == 'e' && *aux++ == 0) {
				if (!TRUE_TOKEN) {
					TRUE_TOKEN = b_lexical_read_token(B_LEXICAL_TRUE_ID, "true");
				}
				free(text);
				return TRUE_TOKEN;
			}
			break;
		}
		}
		break;
	}
	case 'w': {
		if (*aux++ == 'h' && *aux++ == 'i' && *aux++ == 'l' && *aux++ == 'e' && *aux++ == 0) {
			if (!WHILE_TOKEN) {
				WHILE_TOKEN = b_lexical_read_token(B_LEXICAL_WHILE_ID, "while");
			}
			free(text);
			return WHILE_TOKEN;
		}
		break;
	}
	}
	struct b_token_t* token = b_lexical_token_pchar_new(B_LEXICAL_ID_ID, text);
	if (!token) {
		free(text);
		return 0;
	}
	return token;
}

static struct b_token_t* b_lexical_read_number(struct b_lexical_t* self) {
	int c = b_lexical_look(self);
	while (c <= 32) {
		if (c < 0) {
			return 0;
		}
		c = b_lexical_next(self);
	}
	double value = 0;
	int dot = 10;
	value = c - '0';
	c = b_lexical_next(self);
	while (b_lexical_is_number_start(c)) {
		value = 10 * value + (c - '0');
		c = b_lexical_next(self);
	}
	if (c == '.') {
		c = b_lexical_next(self);
		while (b_lexical_is_number_start(c)) {
			value += (double) (c - '0') / dot;
			dot *= 10;
			c = b_lexical_next(self);
		}
	}
	struct b_token_t* token = b_lexical_token_double_new(value);
	if (!token) {
		return 0;
	}
	return token;
}

static struct b_token_t* b_lexical_read_string(struct b_lexical_t* self) {
	int c = b_lexical_look(self);
	while (c <= 32) {
		if (c < 0) {
			return 0;
		}
		c = b_lexical_next(self);
	}
	int length = 1;
	wchar_t* text = (wchar_t*) calloc(length + 1, sizeof(wchar_t));
	wchar_t* aux = text;
	if (!text) {
		return 0;
	}
	c = b_lexical_next(self);
	while (!b_lexical_is_string_start(c)) {
		if (text + length == aux) {
			length *= 2;
			wchar_t* data = (wchar_t*) realloc(text, (length + 1) * sizeof(wchar_t));
			if (!data) {
				free(text);
				return 0;
			}
			text = data;
			aux = text + length / 2;
		}
		if (c == '\\') {
			switch (b_lexical_next(self)) {
			case 'n': {
				*aux++ = '\n';
				break;
			}
			case 'r': {
				*aux++ = '\r';
				break;
			}
			case 't': {
				*aux++ = '\t';
				break;
			}
			case 'f': {
				*aux++ = '\f';
				break;
			}
			case 'b': {
				*aux++ = '\b';
				break;
			}
			case '\\': {
				*aux++ = '\\';
				break;
			}
			default: {
				free(text);
				return 0;
			}
			}
		} else {
			*aux++ = c;
		}
		c = b_lexical_next(self);
	}
	*aux = 0;
	b_lexical_next(self);
	struct b_token_t* token = b_lexical_token_pwchar_new(text);
	if (!token) {
		free(text);
		return 0;
	}
	return token;
}

static struct b_token_t* b_lexical_read_symbol(struct b_lexical_t* self) {
	int c = b_lexical_look(self);
	while (c <= 32) {
		if (c < 0) {
			return 0;
		}
		c = b_lexical_next(self);
	}
	switch (c) {
	case '!': {
		c = b_lexical_next(self);
		if (c == '=') {
			c = b_lexical_next(self);
			if (!NE_TOKEN) {
				NE_TOKEN = b_lexical_read_token(B_LEXICAL_NE_ID, "!=");
			}
			return NE_TOKEN;
		} else {
			if (!NOT_TOKEN) {
				NOT_TOKEN = b_lexical_read_token('!', "!");
			}
			return NOT_TOKEN;
		}
	}
	case '=': {
		c = b_lexical_next(self);
		if (c == '=') {
			c = b_lexical_next(self);
			if (!EQ_TOKEN) {
				EQ_TOKEN = b_lexical_read_token(B_LEXICAL_EQ_ID, "==");
			}
			return EQ_TOKEN;
		} else {
			if (!ASSIGN_TOKEN) {
				ASSIGN_TOKEN = b_lexical_read_token('=', "=");
			}
			return ASSIGN_TOKEN;
		}
	}
	case '-': {
		c = b_lexical_next(self);
		if (c == '-') {
			c = b_lexical_next(self);
			if (!DEC_TOKEN) {
				DEC_TOKEN = b_lexical_read_token(B_LEXICAL_DEC_ID, "--");
			}
			return DEC_TOKEN;
		} else {
			if (!SUB_TOKEN) {
				SUB_TOKEN = b_lexical_read_token('-', "-");
			}
			return SUB_TOKEN;
		}
	}
	case '+': {
		c = b_lexical_next(self);
		if (c == '+') {
			c = b_lexical_next(self);
			if (!INC_TOKEN) {
				INC_TOKEN = b_lexical_read_token(B_LEXICAL_INC_ID, "++");
			}
			return INC_TOKEN;
		} else {
			if (!SUM_TOKEN) {
				SUM_TOKEN = b_lexical_read_token('+', "+");
			}
			return SUM_TOKEN;
		}
	}
	case '>': {
		c = b_lexical_next(self);
		if (c == '=') {
			c = b_lexical_next(self);
			if (!GE_TOKEN) {
				GE_TOKEN = b_lexical_read_token(B_LEXICAL_GE_ID, ">=");
			}
			return GE_TOKEN;
		} else {
			if (!GT_TOKEN) {
				GT_TOKEN = b_lexical_read_token('>', ">");
			}
			return GT_TOKEN;
		}
	}
	case '<': {
		c = b_lexical_next(self);
		if (c == '=') {
			c = b_lexical_next(self);
			if (!LE_TOKEN) {
				LE_TOKEN = b_lexical_read_token(B_LEXICAL_LE_ID, "<=");
			}
			return LE_TOKEN;
		} else {
			if (!LT_TOKEN) {
				LT_TOKEN = b_lexical_read_token('<', "<");
			}
			return LT_TOKEN;
		}
	}
	}
	b_lexical_next(self);
	char* data = (char*) calloc(2, sizeof(char));
	if (!data) {
		return 0;
	}
	data[0] = c;
	return b_lexical_token_pchar_new(c, data);
}

struct b_token_t* b_lexical_read(struct b_lexical_t* self) {
	if (b_lexical_look(self) == -1) {
		return 0;
	} else if (b_lexical_is_word(self)) {
		return b_lexical_read_word(self);
	} else if (b_lexical_is_number(self)) {
		return b_lexical_read_number(self);
	} else if (b_lexical_is_string(self)) {
		return b_lexical_read_string(self);
	} else {
		return b_lexical_read_symbol(self);
	}
}

int b_lexical_token_id(struct b_token_t* self) {
	return self->tag;
}

const char* b_lexical_token_pchar(struct b_token_t* self) {
	return ((struct b_token_pchar_t*) self)->value;
}

const wchar_t* b_lexical_token_utf(struct b_token_t* self) {
	return ((struct b_token_pwchar_t*) self)->value;
}

double b_lexical_token_double(struct b_token_t* self) {
	return ((struct b_token_double_t*) self)->value;
}
