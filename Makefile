CFLAGS=-std=c11 -g -Wall -Wextra -Werror
9cc: 9cc.c lexer.c parser.c gen.c error.c
	$(CC) -o 9cc 9cc.c lexer.c parser.c gen.c error.c $(CFLAGS)

test: 9cc
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean
