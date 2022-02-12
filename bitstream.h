#ifndef _BARTOSZS_BITSTREAM_
#define _BARTOSZS_BITSTREAM_

#include <stdint.h>

struct bitstream {
	uint32_t max_size;
	uint32_t cur_size;
	uint8_t* stream;
};

struct bitstream* bitstream_create(uint32_t max_size);
struct bitstream* bitstream_deepCopy(struct bitstream* stream);
void bitstream_write(struct bitstream* stream, uint8_t value, uint8_t size);
void bitstream_copy(struct bitstream* stream_from, struct bitstream* stream_to);
void bitstream_free(struct bitstream* stream);

#endif
