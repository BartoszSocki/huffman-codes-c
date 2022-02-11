#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct huff_treeNode {
	struct huff_treeNode* left;
	struct huff_treeNode* right;
	uint8_t symbol;
	uint32_t freq;
};

struct huff_minHeap {
	struct huff_treeNode** arr;
	uint32_t cur_size;
	uint32_t max_size;
};

void huff_preorderPrint(struct huff_treeNode* node) {
	if (node == NULL)
		return;

	printf("%c: %d\n", node->symbol, node->freq);
	huff_preorderPrint(node->left);
	huff_preorderPrint(node->right);
}

static void swap(struct huff_treeNode* a, struct huff_treeNode* b) {
	struct huff_treeNode tmp = *a;
	*a = *b;
	*b = tmp;
}

void huff_heapify(struct huff_treeNode** arr, uint32_t size, uint32_t i) {
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
		huff_heapify(arr, size, small);
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

void huff_heapInsert(struct huff_minHeap* heap, struct huff_treeNode* node) {
	uint32_t child = heap->cur_size;

	heap->arr[child] = node;
	while (child > 0 && heap->arr[child]->freq < heap->arr[child >> 1]->freq) {
		swap(heap->arr[child], heap->arr[child >> 1]);
		child >>= 1;
	}
	heap->cur_size += 1;
}

struct huff_treeNode* huff_heapPop(struct huff_minHeap* heap) {
	if (heap == NULL || heap->arr == NULL || heap->cur_size < 1)
		abort();

	struct huff_treeNode* value = heap->arr[0];

	heap->arr[0] = heap->arr[heap->cur_size - 1];
	heap->cur_size -= 1;
	huff_heapify(heap->arr, heap->cur_size, 0);

	return value;
}

void huff_heapPrint(struct huff_minHeap* heap) {
	for (int i = 0; i < heap->cur_size; i++)
		printf("%c: %d\n", heap->arr[i]->symbol, heap->arr[i]->freq);
}

void huff_countChars(const char* text, uint32_t* freq) {
	for (int i = 0; text[i] != '\0'; i++)
		freq[text[i]]++;
}

uint8_t huff_getCodeLength(struct huff_treeNode* node, char symbol, uint8_t counter) {
	if (node == NULL)
		return 0;
	if (node->symbol == symbol)
		return counter;
	return huff_getCodeLength(node->left, symbol, counter + 1) + huff_getCodeLength(node->right, symbol, counter + 1);
}

void huff_encodeChar(struct huff_treeNode* encoder, char symbol, uint32_t* pos) {

}

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

	while (heap->cur_size > 1) {
		struct huff_treeNode* a = huff_heapPop(heap);
		struct huff_treeNode* b = huff_heapPop(heap);
		struct huff_treeNode* c = malloc(sizeof(*c));
		*c = (struct huff_treeNode){a, b, 0, a->freq + b->freq};
		huff_heapInsert(heap, c);
	}

	huff_heapPrint(heap);
	huff_preorderPrint(heap->arr[0]);

	struct huff_treeNode* encoder = heap->arr[0];
	uint32_t encoded_message_length = 0;
	/* printf("%p\n", encoder); */
	for (int i = 0; i < MAX; i++) {
		if (freq[i] != 0) {
			/* printf("%c: %d\n", i, huff_getCodeLength(encoder, i, 0)); */
			encoded_message_length += freq[i] * huff_getCodeLength(encoder, i, 0);
		}
	}
	printf("total length: %d\n", encoded_message_length);
	uint8_t* encoded_message = malloc(sizeof(*encoded_message) * (encoded_message_length >> 3));
	uint32_t pos = 0;

	for (int i = 0; message[i] != '\0'; i++) {
		
	}
}
