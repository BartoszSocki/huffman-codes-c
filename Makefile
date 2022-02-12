# bitstream.o: bitstream.c bitstream.h
# 	$(CC) -c bitstream.c
# huffman.o: huffman.c huffman.h
# 	$(CC) -c huffman.c
# huff_minheap.o: huff_minheap.c huff_minheap.h
# 	$(CC) -c huff_minheap.c
# main: main.o bitstream.o huffman.o huff_minheap.o
# 	$(CC) main.o bitstream.o huffman.o huff_minheap.o

main: main.c bitstream.c huffman.c huff_minheap.c
	$(CC) main.c bitstream.c huffman.c huff_minheap.c

.PHONY: clean

clean:
	rm *.out *.o
