CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

3rdcc: $(OBJS)
				$(CC) -o 3rdcc $(OBJS) $(LDFLAGS)

$(OBJS): 3rdcc.h

test: 3rdcc
				./test.sh

clean:
				rm -f 3rdcc *.o *~ tmp*

.PHONY: test clean
