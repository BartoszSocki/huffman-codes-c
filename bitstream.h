#ifndef _BITSTREAM_IS_INCLUDED_
#define _BITSTREAM_IS_INCLUDED_

#include <stdint.h>

struct bitstream {
	uint32_t max_size;
	uint32_t cur_pos;
	uint8_t* bits;
};

struct bitstream* bitstream_init(uint32_t max_size);
struct bitstream* bitstream_deepCopy(struct bitstream* stream);
void bitstream_copy(struct bitstream* src, struct bitstream* dest);
void bitstream_free(struct bitstream* stream);
void bitstream_appendBit(struct bitstream* stream, uint8_t value);
void bitstream_print(struct bitstream* stream);
uint8_t bitstream_removeLastBit(struct bitstream* stream);
uint8_t bitstream_getBit(struct bitstream* stream, uint32_t pos);

#endif
