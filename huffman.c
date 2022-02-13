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

struct huff_treeNode* huffman(char* message, uint32_t freq[MAX]) {
	huff_countChars(message, freq);
	struct huff_minHeap* heap = huff_heap(MAX);

	for (int i = 0; i < MAX; i++) {
		if (freq[i] != 0) {
			struct huff_treeNode* elem = malloc(sizeof(*elem));
			*elem = (struct huff_treeNode){NULL, NULL, i, freq[i]};
			huff_heapInsert(heap, elem);
		}
	}

	/* merging two binary trees with the lowest probability (in this case with the lowest frequency) */
	while (heap->cur_size > 1) {
		struct huff_treeNode* a = huff_heapPop(heap);
		struct huff_treeNode* b = huff_heapPop(heap);
		struct huff_treeNode* c = malloc(sizeof(*c));
		*c = (struct huff_treeNode){a, b, 0, a->freq + b->freq};
		huff_heapInsert(heap, c);
	}

	return heap->arr[0];
}

uint32_t huff_getTotalMessageLength(struct huff_treeNode* encoder, uint32_t freq[MAX]) {
	uint32_t encoded_message_length = 0;
	for (int i = 0; i < MAX; i++) {
		if (freq[i] != 0)
			encoded_message_length += freq[i] * huff_getCodeLength(encoder, i);
	}
}

struct bitstream* huff_getEncoding(struct huff_treeNode* encoder, uint32_t freq[MAX]) {
	/* creating encoder: A -> 01, B -> 00, ... */
	struct bitstream* encoded_chars[MAX] = {NULL};
	for (int i = 0; i < MAX; i++) {
		if (freq[i] != 0) {
			encoded_chars[i] = huff_getCharBitstream(encoder, i);
			/* printf("%c: ", i); */
			/* bitstream_print(encoded_chars[i]); */
		}
	}
	return encoded_chars;
}

struct bitstream* huff_encodeMessage(struct huff_treeNode* encoder, const char* message, uint32_t freq[MAX]) {
	/* encoding message */
	struct bitstream* encoded_message = bitstream_init(huff_getTotalMessageLength(encoder, freq));
	for (int i = 0; message[i] != '\0'; i++) {
		bitstream_copy(encoded_chars[message[i]], encoded_message);
	}
	return encoded_message;
}
