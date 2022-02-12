#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "bitstream.h"

static uint32_t roundToNext8(uint32_t a) {
	return (a + 7) & ~7;
}

static uint8_t doubleTheSizeFailed(struct bitstream* stream) {
	/* this probably can be rewritten to something nicer */
	stream->max_size = roundToNext8(stream->max_size << 1);
	uint8_t* bits = realloc(stream->bits, sizeof(*(stream->bits)) * (stream->max_size >> 3));
	if (bits == NULL)
		return 1;

	stream->bits = bits;
	return 0;
}

static void printU8(uint8_t a) {
	for (int i = 7; i >= 0; i--) {
		printf("%d", (a & (1 << i)) != 0);
	}
	putchar('\n');
}

struct bitstream* bitstream_init(uint32_t max_size) {
	struct bitstream* stream = malloc(sizeof(*stream));
	/* round to multiple of 8 */
	stream->max_size = roundToNext8(max_size);
	stream->cur_pos = 0;

	/* ceil max_size in bits to bytes*/
	stream->bits = malloc(sizeof(*(stream->bits)) * (stream->max_size >> 3));
	if (stream->bits == NULL) {
		free(stream);
		return NULL;
	}
	return stream;
}

struct bitstream* bitstream_deepCopy(struct bitstream* stream) {
	if (stream == NULL)
		return NULL;

	struct bitstream* copied_stream = bitstream_init(stream->max_size);
	copied_stream->cur_pos = stream->cur_pos;

	if (copied_stream == NULL)
		return NULL;

	memcpy(copied_stream->bits, stream->bits, sizeof(*(stream->bits)) * ((stream->max_size + 7) >> 3));
	/* can memcpy fail? */
	return copied_stream;
}

uint8_t bitstream_getBit(struct bitstream* stream, uint32_t pos) {
	if (pos >= stream->cur_pos)
		abort();

	return (stream->bits[pos >> 3] & (1 << (~pos & 7))) != 0;
}

void bitstream_copy(struct bitstream* src, struct bitstream* dest) {
	if (src == NULL || dest == NULL)
		abort();

	for (int i = 0; i < src->cur_pos; i++)
		bitstream_writeBit(dest, bitstream_getBit(src, i));
}

void bitstream_writeBit(struct bitstream* stream, uint8_t value) {
	if (stream->cur_pos >= stream->max_size)
		if (doubleTheSizeFailed(stream))
			abort();

	stream->bits[stream->cur_pos >> 3] |= ((value != 0) << (~(stream->cur_pos) & 7));
	stream->cur_pos++;
}

uint8_t bitstream_removeBit(struct bitstream* stream) {
	if (stream->cur_pos == 0)
		abort();

	uint8_t bit = bitstream_getBit(stream, stream->cur_pos - 1);
	stream->cur_pos--;
	stream->bits[stream->cur_pos >> 3] &= ~(1 << (~(stream->cur_pos) & 7));
	return bit;
}

void bitstream_free(struct bitstream* stream) {
	free(stream->bits);
	free(stream);
}

void bitstream_print(struct bitstream* stream) {
	for (int i = 0; i < (stream->max_size) >> 3; i++)
		printU8(stream->bits[i]);
}

int main() {
	struct bitstream* stream1 = bitstream_init(4);
	struct bitstream* stream2 = bitstream_init(4);

	for (int i = 0; i < 7; i++)
		bitstream_writeBit(stream1, 1);

	for (int i = 0; i < 7; i++)
		bitstream_writeBit(stream2, 0);

	bitstream_copy(stream2, stream1);
	bitstream_print(stream1);

	/* while (stream->cur_pos != 0) { */
	/* 	printf("%d\n", bitstream_getBit(stream, stream->cur_pos - 1)); */
	/* 	bitstream_removeBit(stream); */
	/* } */
	bitstream_free(stream1);
	bitstream_free(stream2);
}
