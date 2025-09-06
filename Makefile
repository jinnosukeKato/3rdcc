CFLAGS=-std=c11 -g -static

3rdcc: 3rdcc.c

test: 3rdcc
	./test.sh

clean:
	rm -f 3rdcc *.o *~ tmp*

.PHONY: test clean
