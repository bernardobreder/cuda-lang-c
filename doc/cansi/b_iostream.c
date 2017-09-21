#include <stdlib.h>
#include <wchar.h>
#include "b_iostream.h"

unsigned char b_vm_read_byte(unsigned char** bytes) {
	return *((*bytes)++);
}

int b_vm_read_index(unsigned char** bytes, unsigned int* result) {
	unsigned int i1 = *((*bytes)++);
	if (i1 <= 0x7F) {
		if (result) {
			*result = i1;
		}
		return 1;
	} else if ((i1 >> 5) == 0x6) {
		int i2 = *((*bytes)++);
		if (result) {
			*result = ((i1 & 0x1F) << 6) + (i2 & 0x3F);
		}
		return 2;
	} else if ((i1 >> 4) == 0xE) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		if (result) {
			*result = ((i1 & 0xF) << 12) + ((i2 & 0x3F) << 6) + (i3 & 0x3F);
		}
		return 3;
	} else if ((i1 >> 3) == 0x1E) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		if (result) {
			*result = ((i1 & 0x7) << 18) + ((i2 & 0x3F) << 12)
					+ ((i3 & 0x3F) << 6) + (i4 & 0x3F);
		}
		return 4;
	} else if ((i1 >> 2) == 0x3E) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		int i5 = *((*bytes)++);
		if (result) {
			*result = ((i1 & 0x3) << 24) + ((i2 & 0x3F) << 18)
					+ ((i3 & 0x3F) << 12) + ((i4 & 0x3F) << 6) + (i5 & 0x3F);
		}
		return 5;
	} else if ((i1 >> 1) == 0x7E) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		int i5 = *((*bytes)++);
		int i6 = *((*bytes)++);
		if (result) {
			*result = ((i1 & 0x1) << 30) + ((i2 & 0x3F) << 24)
					+ ((i3 & 0x3F) << 18) + ((i4 & 0x3F) << 12)
					+ ((i5 & 0x3F) << 6) + (i6 & 0x3F);
		}
		return 6;
	} else {
		(*bytes)--;
		return 0;
	}
}

int b_vm_read_utf(unsigned char** bytes, wchar_t** result, int* length) {
	unsigned int len;
	int count = b_vm_read_index(bytes, &len);
	if (!count) {
		return 0;
	}
	wchar_t* data = *result = (wchar_t*) calloc(len + 1, sizeof(wchar_t));
	if (!data) {
		*bytes -= count;
		return 0;
	}
	int n;
	for (n = 0; n < len; n++) {
		int m = b_vm_read_index(bytes, (unsigned int*) data);
		if (!m) {
			free(data);
			*bytes -= count;
			return 0;
		}
		count += m;
		data++;
	}
	return count;
}

int b_vm_read_int(unsigned char** bytes, int* result) {
	int i1 = *((*bytes)++);
	if (i1 >> 7 == 0) {
		if (result) {
			if (i1 > 63) {
				*result = -(i1 & 63);
			} else {
				*result = +(i1 & 63);
			}
		}
		return 1;
	} else if (i1 >> 6 == 2) {
		int i2 = *((*bytes)++);
		if (result) {
			if ((i1 & 32) == 32) {
				*result = -(((i1 & 31) << 8) + i2);
			} else {
				*result = +(((i1 & 31) << 8) + i2);
			}
		}
		return 2;
	} else if (i1 >> 5 == 6) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		if (result) {
			if ((i1 & 16) == 16) {
				*result = -(((i1 & 15) << 16) + (i2 << 8) + i3);
			} else {
				*result = +(((i1 & 15) << 16) + (i2 << 8) + i3);
			}
		}
		return 3;
	} else if (i1 >> 4 == 14) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		if (result) {
			if ((i1 & 8) == 8) {
				*result = -(((i1 & 7) << 24) + (i2 << 16) + (i3 << 8) + i4);
			} else {
				*result = +(((i1 & 7) << 24) + (i2 << 16) + (i3 << 8) + i4);
			}
		}
		return 4;
	} else if (i1 >> 3 == 30) {
		i1 = *((*bytes)++);
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		if (result) {
			if (i1 > 127) {
				*result = -(((i1 - 128) << 24) + (i2 << 16) + (i3 << 8) + i4);
			} else {
				*result = +((i1 << 24) + (i2 << 16) + (i3 << 8) + i4);
			}
		}
		return 5;
	} else {
		(*bytes)--;
		return 0;
	}

}
