CFLAGS=-std=c11 -g -static # -g: output debug information
SRCS=$(wildcard *.c) # all c files
OBJS=$(SRCS:.c=.o) # all object files

mycc: $(OBJS)
	$(CC) -o mycc $(OBJS) $(LDFLAGS)

$(OBJS): mycc.h

test: mycc
	./test.sh

clean:
	rm -f mycc *.o *~ tmp*

.PHONY: test clean # "test" and "clean" are not for making files 