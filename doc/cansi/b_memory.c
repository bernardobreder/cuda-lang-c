#include "b_memory.h"
#include <stdlib.h>

static long long size = 1024 * 1024 * 1024;

static int working = 0;

static struct b_memory_data_t* datas = 0;

static int dataSize = 0;

static int dataLength = 0;

struct b_memory_data_t {
	void* data;
	long long size;
};

void b_memory_debug(int flag, long long length) {
	working = flag;
	if (flag) {
		size = length;
		free(datas);
		datas = (struct b_memory_data_t*) calloc(dataLength = 16,
				sizeof(struct b_memory_data_t));
		dataSize = 0;
	}
}

void b_memory_free(void* data) {
	if (working) {
		int n;
		for (n = 0; n < dataSize; n++) {
			if (datas[n].data == data) {
				size += datas[n].size;
				datas[n] = datas[--dataSize];
				break;
			}
		}
	}
	free(data);
}

void* b_memory_alloc(long long length) {
	void* data = calloc(1, length);
	if (working) {
		if (size - length < 0) {
			return 0;
		}
		if (dataSize == dataLength) {
			dataLength *= 2;
			realloc(datas, dataLength * sizeof(struct b_memory_data_t));
		}
		datas[dataSize].data = data;
		datas[dataSize].size = length;
		dataSize++;
		size -= length;
	}
	return data;
}

void* b_memory_realloc(void* old, long long length) {
	void* data = realloc(old, length);
	if (working) {
		int n;
		for (n = 0; n < dataSize; n++) {
			if (datas[n].data == old) {
				size += datas[n].size;
				datas[n] = datas[--dataSize];
				break;
			}
		}
		datas[dataSize].data = data;
		datas[dataSize].size = length;
		dataSize++;
	}
	size -= length;
	return data;
}

long long b_memory_size() {
	if (working) {
		return size;
	} else {
		return -1;
	}
}
