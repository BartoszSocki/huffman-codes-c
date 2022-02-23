#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "huffman.h"
#include "huff_minheap.h"

static void swap(struct huff_treeNode* a, struct huff_treeNode* b) {
	struct huff_treeNode tmp = *a;
	*a = *b;
	*b = tmp;
}

static void heapify(struct huff_treeNode** arr, uint32_t size, uint32_t i) {
	if (arr == NULL)
		exit(EXIT_FAILURE);
	if (i >= size)
		return;

	uint32_t small = i;
	uint32_t left  = 2 * i + 1;
	uint32_t right = 2 * i + 2;

	if (left < size && arr[left]->freq < arr[i]->freq)
		small = left;
	if (right < size && arr[right]->freq < arr[i]->freq)
		small = right;
	
	if (small != i) {
		swap(arr[small], arr[i]);
		heapify(arr, size, small);
	}
}

struct huff_minHeap* huff_heap(uint32_t max_size) {
	if (max_size == 0)
		return NULL;

	struct huff_minHeap* heap = malloc(sizeof(*heap));
	struct huff_treeNode** arr = malloc(sizeof(*arr) * max_size);

	heap->arr = arr;
	heap->max_size = max_size;
	heap->cur_size = 0;

	return heap;
}

struct huff_treeNode* huff_heapPop(struct huff_minHeap* heap) {
	if (heap == NULL || heap->arr == NULL || heap->cur_size < 1)
		exit(EXIT_FAILURE);

	struct huff_treeNode* value = heap->arr[0];

	heap->arr[0] = heap->arr[heap->cur_size - 1];
	heap->cur_size -= 1;
	heapify(heap->arr, heap->cur_size, 0);

	return value;
}

void huff_heapInsert(struct huff_minHeap* heap, struct huff_treeNode* node) {
	if (heap == NULL || heap->arr == NULL)
		exit(EXIT_FAILURE);

/* TODO: double size */
	uint32_t child = heap->cur_size;

	heap->arr[child] = node;
	while (child > 0 && heap->arr[child]->freq < heap->arr[child >> 1]->freq) {
		swap(heap->arr[child], heap->arr[child >> 1]);
		child >>= 1;
	}
	heap->cur_size += 1;
}

void huff_heapFree(struct huff_minHeap* heap) {
	for (int i = 0; i < heap->max_size; i++) {
		huff_treeNodeFree(heap->arr[i]);
	}
	free(heap->arr);
	free(heap);
}
