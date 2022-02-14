#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bitstream.h"
#include "huffman.h"
#include "huff_minheap.h"

int main(int argc, char** argv) {
	char* message = (argc > 1) ? argv[1] : "this is a default message";
	uint32_t freq[MAX] = {0}; 

	huff_countChars(message, freq);
	struct huff_treeNode* encoder = huffman(message, freq);
	struct bitstream** char_to_bits = huff_getEncoding(encoder, freq);
	struct bitstream* encoded_message = huff_encodeMessage(encoder, message, freq);
	
	bitstream_print(encoded_message);

	huff_decodeMessage(encoded_message, encoder);
}
