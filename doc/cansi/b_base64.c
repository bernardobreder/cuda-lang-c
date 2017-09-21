#include <string.h>
#include "b_base64.h"

static char b64string[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

long b_base64_encode(char* to, unsigned char* from, unsigned int len) {
	unsigned char *fromp = from;
	char *top = to;
	unsigned char cbyte;
	unsigned char obyte;
	char end[3];
	for (; len >= 3; len -= 3) {
		cbyte = *fromp++;
		*top++ = b64string[(int) (cbyte >> 2)];
		obyte = (cbyte << 4) & 0x30; /* 0011 0000 */
		cbyte = *fromp++;
		obyte |= (cbyte >> 4); /* 0000 1111 */
		*top++ = b64string[(int) obyte];
		obyte = (cbyte << 2) & 0x3C; /* 0011 1100 */
		cbyte = *fromp++;
		obyte |= (cbyte >> 6); /* 0000 0011 */
		*top++ = b64string[(int) obyte];
		*top++ = b64string[(int) (cbyte & 0x3F)];/* 0011 1111 */
	}
	if (len) {
		end[0] = *fromp++;
		if (--len) {
			end[1] = *fromp++;
		} else {
			end[1] = 0;
		}
		end[2] = 0;
		cbyte = end[0];
		*top++ = b64string[(int) (cbyte >> 2)];
		obyte = (cbyte << 4) & 0x30; /* 0011 0000 */
		cbyte = end[1];
		obyte |= (cbyte >> 4);
		*top++ = b64string[(int) obyte];
		obyte = (cbyte << 2) & 0x3C; /* 0011 1100 */
		if (len) {
			*top++ = b64string[(int) obyte];
		} else {
			*top++ = '=';
		}
		*top++ = '=';
	}
	*top = 0;
	return top - to;
}

long b_base64_decode(unsigned char* to, char* from, unsigned int len) {
	char *fromp = from;
	unsigned char *top = to;
	char *p;
	unsigned char cbyte;
	unsigned char obyte;
	int padding = 0;
	for (; len >= 4; len -= 4) {
		if ((cbyte = *fromp++) == '=') {
			cbyte = 0;
		} else {
			if ((!(p = (char*) memchr(b64string, cbyte, 64)))) {
				return -1;
			}
			cbyte = (p - b64string);
		}
		obyte = cbyte << 2; /* 1111 1100 */
		if ((cbyte = *fromp++) == '=') {
			cbyte = 0;
		} else {
			if ((!(p = (char*) memchr(b64string, cbyte, 64)))) {
				return -1;
			}
			cbyte = p - b64string;
		}
		obyte |= cbyte >> 4; /* 0000 0011 */
		*top++ = obyte;
		obyte = cbyte << 4; /* 1111 0000 */
		if ((cbyte = *fromp++) == '=') {
			cbyte = 0;
			padding++;
		} else {
			padding = 0;
			if ((!(p = (char*) memchr(b64string, cbyte, 64)))) {
				return -1;
			}
			cbyte = p - b64string;
		}
		obyte |= cbyte >> 2; /* 0000 1111 */
		*top++ = obyte;

		obyte = cbyte << 6; /* 1100 0000 */
		if ((cbyte = *fromp++) == '=') {
			cbyte = 0;
			padding++;
		} else {
			padding = 0;
			if ((!(p = (char*) memchr(b64string, cbyte, 64)))) {
				return -1;
			}
			cbyte = p - b64string;
		}
		obyte |= cbyte; /* 0011 1111 */
		*top++ = obyte;
	}
	*top = 0;
	if (len) {
		return -1;
	}
	return (top - to) - padding;
}
