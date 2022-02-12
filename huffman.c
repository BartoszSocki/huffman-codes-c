#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bitstream.h"
#include "huffman.h"
#include "huff_minheap.h"

static uint8_t huff_getCodeLengthHelper(struct huff_treeNode* encoder, char symbol, uint8_t counter) {
	if (encoder == NULL)
		return 0;
	if (encoder->symbol == symbol)
		return counter;
	return huff_getCodeLengthHelper(encoder->left, symbol, counter + 1) + huff_getCodeLengthHelper(encoder->right, symbol, counter + 1);
}

void huff_treeNodeFree(struct huff_treeNode* node) {
	if (node == NULL)
		return;

	huff_treeNodeFree(node->left);
	huff_treeNodeFree(node->right);
	free(node);
}

void huff_countChars(const char* text, uint32_t* freq) {
	for (int i = 0; text[i] != '\0'; i++)
		freq[text[i]]++;
}

uint8_t huff_getCodeLength(struct huff_treeNode* encoder, char symbol) {
	return huff_getCodeLengthHelper(encoder, symbol, 0);
}

static int8_t huff_getCharBitstreamHelper(struct huff_treeNode* encoder, struct bitstream* stream, char symbol) {
	if (encoder == NULL)
		return 0;
	if (encoder->symbol == symbol) {
		return 1;
	}

	bitstream_appendBit(stream, 1);
	int8_t symbolFound = huff_getCharBitstreamHelper(encoder->left, stream, symbol);
	if (symbolFound)
		return 1;

	bitstream_removeLastBit(stream);
	bitstream_appendBit(stream, 0);
	symbolFound = huff_getCharBitstreamHelper(encoder->right, stream, symbol);
	if (symbolFound)
		return 1;

	bitstream_removeLastBit(stream);
	return 0;
}

struct bitstream* huff_getCharBitstream(struct huff_treeNode* encoder, char symbol) {
	uint8_t len = huff_getCodeLength(encoder, symbol);
	if (len == 0)
		return NULL;

	struct bitstream* stream = bitstream_init(len);
	if (stream == NULL)
		abort();

	huff_getCharBitstreamHelper(encoder, stream, symbol);

	return stream;
}

/* void huff_encodeChar(struct huff_treeNode* encoder, char symbol, uint32_t* pos) { */

/* } */
