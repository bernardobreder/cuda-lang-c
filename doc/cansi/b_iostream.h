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
