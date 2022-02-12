#ifndef _BARTOSZS_HUFF_MINHEAP_
#define _BARTOSZS_HUFF_MINHEAP_

#include "huffman.h"

struct huff_minHeap {
	struct huff_treeNode** arr;
	uint32_t cur_size;
	uint32_t max_size;
};

struct huff_minHeap* huff_heap(uint32_t max_size);
struct huff_treeNode* huff_heapPop(struct huff_minHeap* heap);
void huff_heapInsert(struct huff_minHeap* heap, struct huff_treeNode* node);
void huff_heapFree(struct huff_minHeap* heap);

#endif
