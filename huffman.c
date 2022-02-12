#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "huffman.h"
#include "huff_minheap.h"

static uint8_t huff_getCodeLengthHelper(struct huff_treeNode* node, char symbol, uint8_t counter) {
	if (node == NULL)
		return 0;
	if (node->symbol == symbol)
		return counter;
	return huff_getCodeLength(node->left, symbol, counter + 1) + huff_getCodeLength(node->right, symbol, counter + 1);
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

static uint8_t huff_getCodeLength(struct huff_treeNode* node, char symbol) {
	return huff_getCodeLengthHelper(node, symbol, 0);
}

void huff_encodeChar(struct huff_treeNode* encoder, char symbol, uint32_t* pos) {

}
