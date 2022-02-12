#ifndef _BARTOSZS_HUFFMAN_
#define _BARTOSZS_HUFFMAN_

struct huff_treeNode {
	struct huff_treeNode* left;
	struct huff_treeNode* right;
	uint8_t symbol;
	uint32_t freq;
};

void huff_treeNodeFree(struct huff_treeNode* node);
void huff_countChars(const char* text, uint32_t* freq);
void huff_encodeChar(struct huff_treeNode* encoder, char symbol, uint32_t* pos);

uint8_t huff_getCodeLength(struct huff_treeNode* node, char symbol);

#endif
