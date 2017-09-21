#ifndef STRINGBUILDER_T
#define STRINGBUILDER_T

struct stringbuilder_t {
	struct stringbuilder_node_t* first;
	struct stringbuilder_node_t* last;
	size_t size;
};

struct stringbuilder_node_t {
	unsigned char* buffer;
	unsigned char can_free;
	size_t size;
	struct stringbuilder_node_t* next;
};

struct stringbuilder_t* stringbuilder_new();

void stringbuilder_free(struct stringbuilder_t* self);

unsigned char stringbuilder_append_chars(struct stringbuilder_t* self, unsigned char* buffer, size_t length, unsigned char clone);

unsigned char stringbuilder_append_uint(struct stringbuilder_t* self, unsigned int value);

unsigned char* stringbuilder_tostring(struct stringbuilder_t* self, size_t* length);

unsigned char stringbuilder_trim(struct stringbuilder_t* self);

#endif
