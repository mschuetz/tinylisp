LIBFL = /opt/local/lib/libfl.a

CFLAGS+=-D_GNU_SOURCE -I/opt/local/include -Weverything -Wno-padded -Werror -Wno-gnu-zero-variadic-macro-arguments -Wno-gnu-statement-expression #-DDEBUG

flex:
	flex lisp_scanner.rl

.c.o:
	$(CC) $(CFLAGS) -c $<

# separate target because it would fail with -Werror
lex.yy.o: flex
	$(CC) -c lex.yy.c

tinylisp: flex tinylisp.o symbol_table.o base.o lex.yy.o parser.o hashmap.o
	$(CC) -g -lreadline -o tinylisp tinylisp.o symbol_table.o base.o lex.yy.o parser.o $(LIBFL)

run: tinylisp
	./tinylisp

clean: 
	rm tinylisp *.o || true

all: tinylisp

test: tinylisp test/hashmap_test.o
	$(CC) -o test/test hashmap.o hashmap_test.o base.o symbol_table.o -lcheck -L/opt/local/lib 
	./test/test
