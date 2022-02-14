#ifndef _HUFFMAN_IS_INCLUDED_
#define _HUFFMAN_IS_INCLUDED_

#include <stdint.h>
#include "bitstream.h"

#define MAX 256

struct huff_treeNode {
	struct huff_treeNode* left;
	struct huff_treeNode* right;
	uint8_t symbol;
	uint32_t freq;
};

void huff_treeNodeFree(struct huff_treeNode* node);
void huff_countChars(const char* text, uint32_t* freq);
struct bitstream* huff_getCharBitstream(struct huff_treeNode* encoder, char symbol);
/* void huff_encodeChar(struct huff_treeNode* encoder, char symbol, uint32_t* pos); */

uint8_t huff_getCodeLength(struct huff_treeNode* node, char symbol);
struct huff_treeNode* huffman(char* message, uint32_t freq[MAX]);
uint32_t huff_getTotalMessageLength(struct huff_treeNode* encoder, uint32_t freq[MAX]);
struct bitstream** huff_getEncoding(struct huff_treeNode* encoder, uint32_t freq[MAX]);
struct bitstream* huff_encodeMessage(struct huff_treeNode* encoder, const char* message, uint32_t freq[MAX]);
void huff_decodeMessage(struct bitstream* message, struct huff_treeNode* encoder);

#endif
