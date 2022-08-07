CFLAGS=-std=c11 -g -static # -g: additional symbol table for debugger

mycc: mycc.c

test: mycc
	./test.sh

clean:
	rm -f mycc *.o *~ tmp*

.PHONY: test clean