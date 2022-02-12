#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bitstream.h"
#include "huffman.h"
#include "huff_minheap.h"

#define MAX 256

int main() {
	uint32_t freq[MAX] = {0}; 
	char* message = "aaaabbbbcccc";

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

	struct huff_treeNode* encoder = heap->arr[0];
	uint32_t encoded_message_length = 0;
	/* calculating total message length*/
	for (int i = 0; i < MAX; i++) {
		if (freq[i] != 0)
			encoded_message_length += freq[i] * huff_getCodeLength(encoder, i);
	}

	printf("total length: %d\n", encoded_message_length);

	struct bitstream* encoded_chars[MAX] = {NULL};
	for (int i = 0; i < MAX; i++) {
		if (freq[i] != 0) {
			encoded_chars[i] = huff_getCharBitstream(encoder, i);
		}
	}
	/* printf("%p\n", encoded_chars['a']); */
	bitstream_print(encoded_chars['a']);
	bitstream_print(encoded_chars['b']);
	bitstream_print(encoded_chars['c']);
	bitstream_print(encoded_chars['d']);
}

