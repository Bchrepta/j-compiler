all: jc

jc: clean hash.o token.o tokenizer.o jc.c
	clang -Wall -g jc.c tokenizer.o token.o hash.o -o jc

hash.o: hash.c hash.h
	#
	#CIS 240 TODO: update this target to produce LC4.o
	#
	clang -Wall -g -c hash.c

token.o: token.c token.h
	#
	#CIS 240 TODO: update this target to produce loader.o
	#
	clang -Wall -g -c token.c
tokenizer.o: tokenizer.c tokenizer.h
	#
	#CIS 240 TODO: update this target to produce loader.o
	#
	clang -Wall -g -c tokenizer.c

clean:
	rm -rf *.o
	
clobber: clean
	rm -rf trace
