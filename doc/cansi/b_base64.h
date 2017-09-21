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
