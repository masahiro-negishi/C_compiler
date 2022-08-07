CFLAGS=-std=c11 -g -static # -g: additional symbol table for debugger

9cc: 9cc.c

test: 9cc
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean