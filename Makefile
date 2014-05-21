LIBFL = /opt/local/lib/libfl.a

CFLAGS+=-D_GNU_SOURCE -I/opt/local/include -Weverything -Wno-padded -Werror -Wno-gnu-zero-variadic-macro-arguments -Wno-gnu-statement-expression #-DDEBUG

.c.o:
	$(CC) $(CFLAGS) -c $<

# separate target because it would fail with -Werror
lisp_scanner.o: lisp_scanner.c
	$(CC) -c lisp_scanner.c

tinylisp: tinylisp.o symbol_table.o base.o lisp_scanner.o parser.o hashmap.o
	$(CC) -g -lreadline -o tinylisp tinylisp.o symbol_table.o base.o lisp_scanner.o parser.o $(LIBFL)

run: tinylisp
	./tinylisp

clean: 
	rm tinylisp *.o lisp_scanner.c || true

all: tinylisp

test: tinylisp test/hashmap_test.o
	$(CC) -o test/test hashmap.o hashmap_test.o base.o symbol_table.o -lcheck -L/opt/local/lib 
	./test/test
